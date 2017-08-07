## Code Abstractions

### UV_FS_Open

Opens a file.

```ceu
code/await UV_FS_Open (var _char&& path, var int flags, var int mode)
                        -> (var& UV_FS_File file)
                            -> int
```

- Parameters
    - `path`:  path to the file
    - `flags`: access mode flags
    - `mode`:  file permission mode
- Initialization
    - `file`: created [file handle](#uv_fs_file)
- Return
    - `int`: open error
        - returns only case of error (always `<0`)

The file is only ready for use after `UV_FS_Open` triggers `file.ok`.

Céu-libuv references:
    [`UV_FS`](#uv_fs).

libuv references:
    [`ceu_uv_fs_open`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_open),
    [`uv_fs_close`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_close),
    [`uv_fs_req_cleanup`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_req_cleanup).

*Note: all allocated libuv resources are automatically released on termination.*

#### Example

Opens `file.txt` and prints *open ok* after the file is ready for use.
In case of failure, prints *open error* along with the error code:

```ceu
#include "uv/fs.ceu"

var& UV_FS_File file;

var int? err =
    watching UV_FS_Open("file.txt", _O_RDONLY, 0) -> (&file) do
        await file.ok;
        // file is ready for use
        _printf("open ok\n");
    end;
if err? then
    _printf("open error: %d\n", err!);
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_FS_Read

Reads bytes from a file.

```ceu
code/await UV_FS_Read (var& UV_FS_File file, vector&[] byte buf, var usize size, var usize offset)
                        -> ssize
```

- Parameters
    - `file`:   [file handle](#uv_fs_file) to read from
    - `buf`:    destination buffer
    - `size`:   number of bytes to read
    - `offset`: starting file offset
- Return
    - `ssize`: actual number of bytes read
        - `>=0`: number of bytes
        - `<0`:  read error

Céu-libuv references:
    [`ceu_uv_fs_read`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_read),
    [`UV_FS`](#uv_fs).

libuv references:
    [`uv_buf_init`](http://docs.libuv.org/en/v1.x/misc.html#c.uv_buf_init),
    [`uv_fs_req_cleanup`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_req_cleanup).

*Note: all allocated libuv resources are automatically released on termination.*

#### Example

Prints the contents of `file.txt` in a loop that reads the file in chunks of 10
bytes:

```ceu
#include "uv/fs.ceu"

var& UV_FS_File file;

var int? err =
    watching UV_FS_Open("file.txt", _O_RDONLY, 0) -> (&file) do
        await file.ok;

        var usize offset = 0;
        loop do
            vector[11] byte buf;
            var ssize n = await UV_FS_Read(&file,&buf,$$buf-1,offset);
            if n == 0 then
                break;
            end
            buf = buf .. [{'\0'}];
            _printf("%s", &&buf[0]);
            offset = offset + ($$buf-1);
        end
    end;
_ceu_dbg_assert(not err?);

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_FS_ReadLine

Reads a line from a file.

```ceu
code/await UV_FS_ReadLine (var& UV_FS_File file, vector&[] byte buf, var usize offset)
                            -> ssize
```

- Parameters
    - `file`:   [file handle](#uv_fs_file) to read from
    - `buf`:    destination buffer (excludes the leading `\n`)
    - `offset`: starting file offset
- Return
    - `ssize`: actual number of bytes read
        - `>=0`: number of bytes (includes the leading `\n`)
        - `<0`:  read error

`TODO: the file is currently read byte by byte.`

Céu-libuv references:
    [`UV_FS_Read`](#uv_fs_read).

#### Example

Prints the contents of `file.txt` in a loop that reads the file line by line:

```ceu
#include "uv/fs.ceu"

var& UV_FS_File file;

watching UV_FS_Open("file.txt", _O_RDONLY, 0) -> (&file) do
    await file.ok;

    var usize off = 0;
    loop do
        vector[] byte line;
        var ssize n = await UV_FS_ReadLine(&file,&line,off);
        if n <= 0 then
            break;
        end
        _printf("line = %s [%d]\n", &&line[0], n as int);
        off = off + (n as usize);
    end
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_FS_Write

Write bytes from a file.

```ceu
code/await UV_FS_Write (var& UV_FS_File file, vector&[] byte buf, var usize size, var usize offset)
                        -> ssize
```

- Parameters
    - `file`:   [file handle](#uv_fs_file) to write to
    - `buf`:    source buffer
    - `size`:   number of bytes to write
    - `offset`: starting file offset
- Return
    - `ssize`: actual number of bytes written
        - `>=0`: number of bytes
        - `<0`:  write error

Céu-libuv references:
    [`ceu_uv_fs_write`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_write),
    [`UV_FS`](#uv_fs).

libuv references:
    [`uv_buf_init`](http://docs.libuv.org/en/v1.x/misc.html#c.uv_buf_init),
    [`uv_fs_req_cleanup`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_req_cleanup).

*Note: all allocated libuv resources are automatically released on termination.*

#### Example

Writes the string *Hello World* to `hello.txt`:

```ceu
#include "uv/fs.ceu"

var& UV_FS_File file;

var _mode_t mode = _S_IRUSR|_S_IWUSR|_S_IRGRP|_S_IWGRP|_S_IROTH;

var int? err =
    watching UV_FS_Open("hello.txt", _O_CREAT|_O_WRONLY, mode) -> (&file) do
        await file.ok;
        vector[] byte buf = [] .. "Hello World!\n";
        var ssize n = await UV_FS_Write(&file,&buf,$buf,0);
        if (n<0) or (n as usize)!=$buf then
            _printf("write error\n");
        end
    end;
if err? then
    _printf("open error: %d\n", err!);
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_FS_Fstat

Reads information about a file.

```ceu
code/await UV_FS_Fstat (var& UV_FS_File file, var& _uv_stat_t stat)
                        -> int
```

- Parameters
    - `file`: [file handle](#uv_fs_file) to write to
    - `stat`: destination buffer
- Return
    - `int`: operation status
        -  `0`: success
        - `<0`: error

Céu-libuv references:
    [`ceu_uv_fs_fstat`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_fstat),
    [`UV_FS`](#uv_fs).

libuv references:
    [`uv_fs_req_cleanup`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_req_cleanup).

*Note: all allocated libuv resources are automatically released on termination.*

#### Example

Prints the size of `file.txt` in bytes:

```ceu
#include "uv/fs.ceu"

var& UV_FS_File file;

var int? err =
    watching UV_FS_Open("file.txt", _O_RDONLY, 0) -> (&file)
    do
        await file.ok;

        var _uv_stat_t stat = _;
        await UV_FS_Fstat(&file, &stat);
        _printf("size = %ld\n", stat.st_size);
    end;

if err? then
    _printf("open error: %d\n", err!);
end

escape 0;
```
