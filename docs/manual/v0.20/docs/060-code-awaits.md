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
    - `file`: alias to [file](#TODO) handle
- Return
    - `int` (`<0`): open error

The file is only ready for use after `UV_FS_Open` triggers `file.ok`.

All allocated resources are released on termination.

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
    [`ceu_uv_fs_close`](#TODO),
    [`uv_fs_req_cleanup`](#TODO).

UV_FS_Read
----------

Read bytes from a file.

```ceu
code/await UV_FS_Read (var& UV_FS_File file, vector&[] byte buf,
                       var usize size, var usize offset)
                            -> ssize
```

- Parameters
    - `file`:   [file](#TODO) handle to read from
    - `buf`:    destination buffer
    - `size`:   number of bytes to read
    - `offset`: starting file offset
- Return
    - `ssize`: actual number of bytes read
        - `>=0`: number of bytes
        - `<0`:  read error

All allocated resources are released on termination.

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
