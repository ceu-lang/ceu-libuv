Code/Await Abstractions
=======================

UV_FS_Open
----------

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
    - `file`: created [file handle](#TODO)
- Return
    - `int` (`<0`): open error

The file is only ready for use after `UV_FS_Open` triggers `file.ok`.

Example:

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

Opens `file.txt` and prints *open ok* after the file is ready for use.
In case of failure, prints *open error* along with the error code.

libuv references:
    [`uv_fs_open`](#TODO),
    [`uv_fs_close`](#TODO),
    [`uv_fs_req_cleanup`](#TODO).

*Note: all allocated libuv resources are automatically released on termination.*

UV_FS_Read
----------

Reads bytes from a file.

```ceu
code/await UV_FS_Read (var& UV_FS_File file, vector&[] byte buf, var usize size, var usize offset)
                        -> ssize
```

- Parameters
    - `file`:   [file handle](#TODO) to read from
    - `buf`:    destination buffer
    - `size`:   number of bytes to read
    - `offset`: starting file offset
- Return
    - `ssize`: actual number of bytes read
        - `>=0`: number of bytes
        - `<0`:  read error

Example:

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

Prints the contents of `file.txt` in a loop that reads the file 10 in 10 bytes.

libuv references:
    [`uv_buf_init`](#TODO),
    [`uv_fs_read`](#TODO),
    [`uv_fs_req_cleanup`](#TODO).

*Note: all allocated libuv resources are automatically released on termination.*

UV_FS_Write
-----------

Write bytes from a file.

```ceu
code/await UV_FS_Write (var& UV_FS_File file, vector&[] byte buf, var usize size, var usize offset)
                        -> ssize
```

- Parameters
    - `file`:   [file handle](#TODO) to write to
    - `buf`:    source buffer
    - `size`:   number of bytes to write
    - `offset`: starting file offset
- Return
    - `ssize`: actual number of bytes written
        - `>=0`: number of bytes
        - `<0`:  write error

Example:

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

Writes the string *Hello World* to `hello.txt`.

libuv references:
    [`uv_buf_init`](#TODO),
    [`uv_fs_write`](#TODO),
    [`uv_fs_req_cleanup`](#TODO).

*Note: all allocated libuv resources are automatically released on termination.*

UV_FS_ReadLine
--------------

Reads a line from a file.

```ceu
code/await UV_FS_ReadLine (var& UV_FS_File file, vector&[] byte buf, var usize offset)
                            -> ssize
```

- Parameters
    - `file`:   [file handle](#TODO) to read from
    - `buf`:    destination buffer (excludes the leading `\n`)
    - `offset`: starting file offset
- Return
    - `ssize`: actual number of bytes read
        - `>=0`: number of bytes (includes the leading `\n`)
        - `<0`:  read error

`TODO: the file is currently read byte by byte.`

Example:

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

Prints the contents of `file.txt` in a loop that reads the file line by line.

Céu-libuv references:
    [`UV_FS_Read`](#TODO).

UV_FS_Fstat
-----------

Reads information about a file.

```ceu
code/await UV_FS_Fstat (var& UV_FS_File file, var& _uv_stat_t stat)
                        -> int
```

- Parameters
    - `file`: [file handle](#TODO) to write to
    - `stat`: destination buffer
- Return
    - `int`: operation status
        -  `0`: success
        - `<0`: error

Example:

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

Prints the size of `file.txt` in bytes.

libuv references:
    [`uv_fs_fstat`](#TODO),
    [`uv_fs_req_cleanup`](#TODO).

*Note: all allocated libuv resources are automatically released on termination.*

UV_Stream_Listen
----------------

Start listening for incoming connections.

```ceu
code/await UV_Stream_Listen (var& _uv_stream_t stream, var int backlog)
                                -> (event& void ok)
                                    -> int
```

- Parameters
    - `stream`:  stream to listen
    - `backlog`: number of connections the kernel might queue
- Initialization
    - `ok`: signalled on every new incoming connection
- Return
    - `int`: operation status
        -  `0`: success
        - `<0`: error

Example:

```ceu
#include "uv/tcp.ceu"

var& _uv_tcp_t server;
watching UV_TCP_Open() -> (&server) do
    var _sockaddr_in addr = _;
    _uv_ip4_addr("0.0.0.0", 7000, &&addr);
    _uv_tcp_bind(&&server, &&addr as _sockaddr&&, 0);

    event& void ok_listen;
    watching UV_TCP_Listen(&server,128) -> (&ok_listen) do
        every ok_listen do
            var _uv_tcp_t client = _;
            var int err = _ceu_uv_tcp_init(&&client);
            _ceu_dbg_assert(err == 0);
            var int ret = _uv_accept(&&server as _uv_stream_t&&, &&client as _uv_stream_t&&);
            _ceu_dbg_assert(ret == 0);

            vector[20] _char ip = _;
            var _sockaddr_in name = _;
            var int len = _;
            _uv_tcp_getsockname(&&client, &&name as _sockaddr&&, &&len);
            _uv_ip4_name(&&name,&&ip[0],20);
            _printf("new incoming connection from %s\n", &&ip[0]);
            _uv_close(&&client as _uv_handle_t&&, null);
        end
    end
end

escape 0;
```

[Opens](#TODO) a `server` tcp handle, binds it to port `7000`, and then enters
in listen mode.
Each incoming connection triggers `ok_listen` whose reaction accepts the
client, prints its address, and closes the connection.

libuv references:
    [`uv_listen`](#TODO).
