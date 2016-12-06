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
    - `file`: alias to managed [file](#TODO)
- Return
    - `int` (`<0`): opening error

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
        _printf("opening ok\n");
    end;
if err? then
    _printf("opening error: %d\n", err!);
end

escape 0;
```

Opens `file.txt` and prints *opening ok* after the file is ready for use.
In case of failure, prints *opening error* along with the error code.

libuv References:
    [`uv_fs_open`](#TODO),
    [`ceu_uv_fs_close`](#TODO),
    [`uv_fs_req_cleanup`](#TODO).
