## Code Abstractions

<!---------------------------------------------------------------------------->

### UV_TCP_Open

Opens a raw TCP stream.

```ceu
code/await UV_TCP_Open (void) -> (var& _uv_tcp_t tcp) -> int
```

- Parameters
    - `void`: nothing
- Initialization
    - `tcp`: opened [TCP handle](#TODO)
- Return
    - `int`: TCP error
        - returns only in case of error (always `<0`)

Céu-libuv references:
    [`ceu_uv_tcp_init`](#TODO),
    [`ceu_uv_close`](#TODO),
    [`UV_STREAM_ERROR`](#TODO).

*Note: all allocated libuv resources are automatically released on termination.*

#### Example

```ceu
var& _uv_tcp_t tcp;
watching UV_TCP_Open() -> (&tcp)
do
    <...>   // use the raw `tcp` handle
end
```

<!---------------------------------------------------------------------------->

Opens a connection TCP stream.

```ceu
code/await UV_TCP_Connect (var _char&& ip, var int port)
                            -> (var& _uv_tcp_t tcp, event& void ok)
                                -> int
```

- Parameters
    - `ip`:     remote host
    - `port`:   remote port
- Initialization
    - `tcp`:    disconnected [TCP handle](#TODO)
    - `ok`:     signalled when `tcp` connects and is ready for use
- Return
    - `int`: TCP error
        - returns only in case of error (always `<0`)

#### Example

```ceu
var&   _uv_tcp_t tcp;
event& void      ok_connected;
watching UV_TCP_Connect("127.0.0.1", 7000) -> (&tcp, &ok_connected) do
    await ok_connected;
    <...>   // use the connected `tcp` handle
end
```

<!---------------------------------------------------------------------------->

### UV_TCP_Listen

Starts listening for incoming connections in a TCP stream.

Defined in terms of [`UV_Stream_Listen`](#TODO):

```c
#define UV_TCP_Listen(tcp, backlog) UV_Stream_Listen((tcp) as _uv_stream_t&&, backlog)
```

Céu-libuv references:
    [`UV_Stream_Listen`](#TODO)

#### Example

[Opens](#TODO) a `server` TCP handle, binds it to port `7000`, and then enters
in listen mode.
Each incoming connection triggers the event `ok_listen`.

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
            <...>   // handle incoming connection
        end
    end
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_TCP_Read

Reads bytes from a TCP stream continuously.

Defined in terms of [`UV_Stream_Read`](#TODO):

```c
#define UV_TCP_Read(tcp, bytes) UV_Stream_Read((tcp) as _uv_stream_t&&, bytes)
```

Céu-libuv references:
    [`UV_Stream_Read`](#TODO)

#### Example

Connects to `127.0.0.1:7000` and waits reading 10 bytes in a loop:

```ceu
#include "uv/tcp.ceu"

var& _uv_tcp_t tcp;
var int? err =
    watching UV_TCP_Open() -> (&tcp) do
        var _uv_connect_t connect = _;
        var _sockaddr_in dest = _;
        _uv_ip4_addr("127.0.0.1", 7000, &&dest);
        _ceu_uv_tcp_connect(&&connect, &&tcp, (&&dest as _sockaddr&&));

        var _uv_connect_t&& c;
        var int status;
        (c,status) = await UV_STREAM_CONNECT until c==&&connect;
        _ceu_dbg_assert(status == 0);

        vector[11] byte buf;

        event& usize ok_read;
        var int? err2 =
            watching UV_TCP_Read(&tcp,&buf) -> (&ok_read) do
                loop do
                    await ok_read;
                    if $buf == 10 then  // assumes server sends exactly 10 bytes
                        break;
                    end
                end
            end;
        _ceu_dbg_assert(not err2?);

        buf = buf .. [{'\0'}];
        _printf("buf: %s\n", &&buf[0]);
    end;
_ceu_dbg_assert(not err?);

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_TCP_ReadLine

Reads a single line from a TCP stream.

Defined in terms of [`UV_Stream_ReadLine`](#TODO):

```c
#define UV_TCP_ReadLine(tcp, bytes) UV_Stream_ReadLine((tcp) as _uv_stream_t&&, bytes)
```

Céu-libuv references:
    [`UV_Stream_ReadLine`](#TODO)

#### Example

`TODO`

<!---------------------------------------------------------------------------->

### UV_TCP_Write

Write bytes to a TCP stream.

Defined in terms of [`UV_Stream_Write`](#TODO):

```c
#define UV_TCP_Write(tcp, bytes) UV_Stream_Write((tcp) as _uv_stream_t&&, bytes)
```

Céu-libuv references:
    [`UV_Stream_Write`](#TODO)

#### Example

`TODO`

<!---------------------------------------------------------------------------->
