## Code Abstractions

<!---------------------------------------------------------------------------->

### UV_TCP_Open

Opens an uninitialized TCP stream.

```ceu
code/await UV_TCP_Open (void) -> (var& _uv_tcp_t tcp) -> int
```

- Parameters
    - `void`: nothing
- Initialization
    - `tcp`: opened and uninitialized [TCP handle](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t)
- Return
    - `int`: TCP error
        - returns only in case of error (always `<0`)

Céu-libuv references:
    [`ceu_uv_tcp_init`](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_init),
    [`ceu_uv_close`](http://docs.libuv.org/en/v1.x/handle.html#c.uv_close),
    [`UV_STREAM_ERROR`](#uv_stream_error).

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
    - `tcp`:    [TCP handle](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t)
    - `ok`:     signalled when `tcp` connects and is ready for use
- Return
    - `int`: TCP error
        - returns only in case of error (always `<0`)

Céu-libuv references:
    [`ceu_uv_tcp_connect`](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_connect),
    [`UV_STREAM_CONNECT`](#uv_stream_connnect).

*Note: all allocated libuv resources are automatically released on termination.*

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

Starts listening for incoming connections on a TCP stream.

Defined in terms of [`UV_Stream_Listen`](#uv_stream_listen_1):

```c
#define UV_TCP_Listen(tcp, backlog) UV_Stream_Listen((tcp) as _uv_stream_t&&, backlog)
```

Céu-libuv references:
    [`UV_Stream_Listen`](#uv_stream_listen_1)

#### Example

Opens a `tcp` handle, binds it to port `7000`, and then enters
in listen mode.
Each incoming connection triggers the event `ok`.

```ceu
#include "uv/tcp.ceu"

var& _uv_tcp_t tcp;
watching UV_TCP_Open() -> (&tcp) do
    var _sockaddr_in addr = _;
    _uv_ip4_addr("0.0.0.0", 7000, &&addr);
    _uv_tcp_bind(&&tcp, &&addr as _sockaddr&&, 0);

    event& void ok;
    watching UV_TCP_Listen(&tcp,128) -> (&ok) do
        every ok do
            <...>   // handle incoming connection
        end
    end
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_TCP_Open_Bind_Listen

Opens a TCP stream, binds it to an IP and port, and listens for incoming
connections.

```ceu
code/await UV_TCP_Open_Bind_Listen (var _char&& ip, var int port, var int backlog)
                                    -> (var& _uv_tcp_t tcp, event& void ok)
                                        -> int
```

- Parameters
    - `ip`:      local host
    - `port`:    local port
    - `backlog`: number of connections the kernel might queue
- Initialization
    - `tcp`:     [TCP handle](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t)
    - `ok`:      signalled on every new incoming connection
- Return
    - `int`:     TCP error
        - returns only in case of error (always `<0`)

Céu-libuv references:
    [`UV_TCP_Open`](#uv_tcp_open),
    [`UV_TCP_Listen`](#uv_tcp_listen_1).

#### Example

Listen on port `7000`:

```ceu
#include "uv/tcp.ceu"
var&   _uv_tcp_t tcp;
event& void      ok;
watching UV_TCP_Open_Bind_Listen("0.0.0.0",7000,128) -> (&tcp,&ok) do
    every ok do
        <...>   // handle incoming connection
    end
end
```

<!---------------------------------------------------------------------------->

Opens a TCP stream, binds it to an IP and port, listens for incoming
connections, and spawns a handler on every new connection.

```ceu
code/await UV_TCP_Server (var _char&& ip, var int port, var int backlog) -> int
```

- Parameters
    - `ip`:      local host
    - `port`:    local port
    - `backlog`: number of connections the kernel might queue
- Return
    - `int`:     TCP error
        - returns only in case of error (always `<0`)

The handler is a user-defined `UV_TCP_Server_Handler`, which must be declared
in between the includes for `uv/tcp.ceu` and `uv/tcp-server.ceu`, as follows:

```ceu
#include "uv/tcp.ceu"
code/await UV_TCP_Server_Handler (var& _uv_tcp_t tcp) -> void do
    <...>       // handles the new client connection
end
#include "uv/tcp-server.ceu"
<...>
```

The handler receives the [TCP handle](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t) of the connected client.

If the macro `UV_TCP_SERVER_HANDLER_MAX` is defined, the server uses a bounded
pool of `UV_TCP_Server_Handler`

Céu-libuv references:
    [`UV_TCP_Open_Bind_Listen`](#uv_tcp_open_bind_listen),
    [`UV_TCP_Open`](#uv_tcp_open).

libuv references:
    [`_uv_accept`].

#### Example:

```ceu
#include "uv/tcp.ceu"

code/await UV_TCP_Server_Handler (var& _uv_tcp_t tcp) -> void do
    <...>       // handles the new client connection
end

#include "uv/tcp-server.ceu"

await UV_TCP_Server("0.0.0.0", 7000, 128);
```

<!---------------------------------------------------------------------------->

### UV_TCP_Read

Reads bytes from a TCP stream continuously.

Defined in terms of [`UV_Stream_Read`](#uv_stream_read_1):

```c
#define UV_TCP_Read(tcp, bytes) UV_Stream_Read((tcp) as _uv_stream_t&&, bytes)
```

Céu-libuv references:
    [`UV_Stream_Read`](#uv_stream_read_1)

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

Defined in terms of [`UV_Stream_ReadLine`](#uv_stream_readline):

```c
#define UV_TCP_ReadLine(tcp, bytes) UV_Stream_ReadLine((tcp) as _uv_stream_t&&, bytes)
```

Céu-libuv references:
    [`UV_Stream_ReadLine`](#uv_stream_readline)

#### Example

`TODO`

<!---------------------------------------------------------------------------->

### UV_TCP_Write

Write bytes to a TCP stream.

Defined in terms of [`UV_Stream_Write`](#uv_stream_write_1):

```c
#define UV_TCP_Write(tcp, bytes) UV_Stream_Write((tcp) as _uv_stream_t&&, bytes)
```

Céu-libuv references:
    [`UV_Stream_Write`](#uv_stream_write_1)

#### Example

`TODO`

<!---------------------------------------------------------------------------->
