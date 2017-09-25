## Code Abstractions

### UV_FS_Open

Opens a file.

```ceu
code/await UV_FS_Open (var _char&& path, var usize? buffer_size, var int? flags, var int? mode)
                        -> (var UV_FS_File file)
                            -> int

```

- Parameters
    - `path`:           path to the file
    - `buffer_size`:    size of the read & write ring buffer (default: `1024`)
    - `flags`:          access mode flags (default: `_O_RDONLY`)
    - `mode`:           file permission mode (default: `0`)
- Public fields
    - `file`:           [file](#uv_fs_file)
- Return
    - `int`: open error
        - returns only in case of an error (always `<0`)

The file is only ready for use after [`file.ok`](#uv_fs_file) is triggered.

Céu-libuv references:
    [`UV_FS`](#uv_fs).

libuv references:
    [`ceu_uv_fs_open`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_open),
    [`uv_fs_close`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_close),
    [`uv_fs_req_cleanup`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_req_cleanup).

#### Example

Opens `file.txt` and prints *open ok* after the file is ready for use.
In case of failure, prints *open error* along with the error code:

```ceu
#include "uv/fs.ceu"

var&? UV_FS_Open o = spawn UV_FS_Open("file.txt",_,_,_);
var int? err =
    watching o do
        await o.file.ok;
        _printf("open ok\n");   // file is ready for use
    end;
if err? then
    _printf("open error: %d\n", err!);
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_FS_Read_N

Reads a specified number of bytes from the [file](#uv_fs_file) to its buffer.

```ceu
code/await UV_FS_Read_N (var& UV_FS_File file, var usize n) -> ssize
```

- Parameters
    - `file`:   [file](#uv_fs_file) to read
    - `n`:      number of bytes to read
- Return
    - `ssize`: number of bytes read from `file`
        - `>=0`: number of bytes (less than or equal to `n`)
        - `<0`:  read error

Céu-libuv references:
    [`ceu_uv_fs_read`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_read),
    [`UV_FS`](#uv_fs).

libuv references:
    [`uv_buf_init`](http://docs.libuv.org/en/v1.x/misc.html#c.uv_buf_init),
    [`uv_fs_req_cleanup`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_req_cleanup).

#### Example

Prints the contents of `file.txt` in a loop that reads the file in chunks of 10
bytes:

```ceu
#include "uv/fs.ceu"

var&? UV_FS_Open o = spawn UV_FS_Open("file.txt", 11, _,_);
var int? err =
    watching o do
        await o.file.ok;

        loop do
            var ssize n = await UV_FS_Read_N(&o.file, $$o.file.buffer-1);
            if n == 0 then
                break;
            end
            o.file.buffer = o.file.buffer .. [{'\0'}];
            _printf("%s", &&o.file.buffer[0]);
            $o.file.buffer = 0;
        end
    end;
_ceu_dbg_assert(not err?);

escape 0;

```

<!---------------------------------------------------------------------------->

### UV_FS_Read_Line

Reads a line from a [file](#uv_fs_file).

```ceu
code/await UV_FS_Read_Line (var& UV_FS_File file, var&[] byte line, var usize? by) -> ssize
```

- Parameters
    - `file`:   [file](#uv_fs_file) to read
    - `line`:   alias to destination buffer (excludes the leading `\n`)
    - `by`:     size of read chunks in bytes (default: `128`)
- Return
    - `ssize`: number of bytes read from `file`
        - `>=0`: number of bytes (includes the leading `\n` and extra bytes)
        - `<0`:  read error

The [file](#uv_fs_file) buffer advances to the byte after the `\n`.

Céu-libuv references:
    [`UV_FS_Read_N`](#uv_fs_read_n).

#### Example

Prints the contents of `file.txt` in a loop that reads the file line by line:

```ceu
#include "uv/fs.ceu"

var&? UV_FS_Open o = spawn UV_FS_Open("file.txt",_,_,_);
watching o do
    await o.file.ok;
    loop do
        var[] byte line;
        var ssize n = await UV_FS_Read_Line(&o.file,&line,_);
        if n <= 0 then
            break;
        end
        line = line .. [{'\0'}];
        _printf("%s\n", &&line[0], n);
    end
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_FS_Write_N

Writes a specified number of bytes to the [file](#uv_fs_file) from its buffer.

```ceu
code/await UV_FS_Write_N (var& UV_FS_File file, var usize? n) -> ssize
```

- Parameters
    - `file`:   [file](#uv_fs_file) to write
    - `n`:      number of bytes to write (default: current size of the `file` buffer)
- Return
    - `ssize`: number of bytes written
        - `>=0`: number of bytes
        - `<0`:  write error

The written bytes are removed from the file buffer.

Céu-libuv references:
    [`ceu_uv_fs_write`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_write),
    [`UV_FS`](#uv_fs).

libuv references:
    [`uv_buf_init`](http://docs.libuv.org/en/v1.x/misc.html#c.uv_buf_init),
    [`uv_fs_req_cleanup`](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_req_cleanup).

#### Example

Writes the string *Hello World* to `hello.txt`:

```ceu
#include "uv/fs.ceu"

var& UV_FS_File file;

var _mode_t mode = _S_IRUSR|_S_IWUSR|_S_IRGRP|_S_IWGRP|_S_IROTH;
var&? UV_FS_Open o = spawn UV_FS_Open("hello.txt", _, _O_CREAT|_O_WRONLY, mode);
watching o do
    await o.file.ok;
    o.file.buffer = [] .. "Hello World!\n";
    var usize n1 = $o.file.buffer;
    var ssize n2 = await UV_FS_Write_N(&o.file,$o.file.buffer);
    _ceu_dbg_assert(n2>=0 and n2==n1);
end;

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
    - `file`: [file](#uv_fs_file) to read
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
