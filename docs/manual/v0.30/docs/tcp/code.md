## Code Abstractions

<!---------------------------------------------------------------------------->

### UV_TCP_Open

Opens a [TCP](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t)
[stream](../stream/#uv_stream).

```ceu
code/await UV_TCP_Open (var int? buffer_size) -> (var UV_Stream stream) -> int
```

- Parameters
    - `buffer_size`:    size of the read & write ring buffer (default: `1024`)
- Public fields
    - `stream`:         opened and uninitialized [TCP](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t) [stream](../stream/#uv_stream)
- Return
    - `int`: TCP error
        - returns only in case of error (always `<0`)

Céu-libuv references:
    [`ceu_uv_tcp_init`](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_init),
    [`ceu_uv_close`](http://docs.libuv.org/en/v1.x/handle.html#c.uv_close),
    [`UV_STREAM_ERROR`](../stream/#uv_stream_error).

#### Example

```ceu
#include "uv/tcp.ceu"

var&? UV_TCP_Open tcp = spawn UV_TCP_Open(_);
var int? err =
    watching tcp do
        <...>   // use the raw `tcp` stream
    end;
if err? then
    _fprintf(_stderr, "%s\n", _uv_strerror(err!));
end

escape 0;
```

<!---------------------------------------------------------------------------->

Opens a [TCP](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t)
[stream](../stream/#uv_stream) and connects it.

```ceu
code/await UV_TCP_Connect (var _char&& ip, var int port, var int? buffer_size)
                            -> (var& UV_Stream stream, event void ok)
                                -> int
```

- Parameters
    - `ip`:          remote host
    - `port`:        remote port
    - `buffer_size`: size of the read & write [stream](../stream/#uv_stream) ring buffer (default: `1024`)
- Public fields
    - `stream`:      [TCP](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t) [stream](../stream/#uv_stream)
    - `ok`:          event signalled when `stream` connects and is ready for use
- Return
    - `int`: TCP error
        - returns only in case of error (always `<0`)

Céu-libuv references:
    [`ceu_uv_tcp_connect`](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_connect),
    [`UV_STREAM_CONNECT`](../stream/#uv_stream_connect).

#### Example

```ceu
#include "uv/tcp.ceu"

var&? UV_TCP_Connect c = spawn UV_TCP_Connect("127.0.0.1", 7000, _);
watching c do
    await c.ok;
    <...>   // use the connected TCP `c.stream`
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_TCP_Open_Bind_Listen

Opens a [TCP](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t)
[stream](../stream/#uv_stream), binds it to an IP and port, and listens for incoming
connections.

```ceu
code/await UV_TCP_Open_Bind_Listen (var _char&&? ip, var int port, var int? backlog, var int? buffer_size)
                                    -> (var& UV_Stream stream, event& void ok)
                                        -> int
```

- Parameters
    - `ip`:          local host (default: `"0.0.0.0"`)
    - `port`:        local port
    - `backlog`:     number of connections the kernel might queue (default: `128`)
    - `buffer_size`: size of the read & write [stream](../stream/#uv_stream) ring buffer (default: `1024`)
- Public fields
    - `stream`:  [TCP](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t) [stream](../stream/#uv_stream)
    - `ok`:      event signalled on every new incoming connection
- Return
    - `int`: TCP error
        - returns only in case of error (always `<0`)

Céu-libuv references:
    [`UV_TCP_Open`](#uv_tcp_open),
    [`UV_Stream_Listen`](../stream/../stream/#uv_stream_listen_1).

#### Example

Listen on port `7000`:

```ceu
#include "uv/tcp.ceu"

var&? UV_TCP_Open_Bind_Listen tcp = spawn UV_TCP_Open_Bind_Listen("0.0.0.0", 7000, _,_);
watching tcp do
    every tcp.ok do
        <...>   // handle incoming connections
    end
end

escape 0;
```

<!---------------------------------------------------------------------------->

Opens a [TCP](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t)
[stream](../stream/#uv_stream), binds it to an IP and port, listens for incoming
connections, and spawns a handler on every new connection.

```ceu
code/await UV_TCP_Server (var _char&&? ip, var int port,
                          var int? backlog, var int? buffer_size,
                          var&? UV_TCP_Server_Data shared)
                            -> int

```

- Parameters
    - `ip`:          local host (default: `"0.0.0.0"`)
    - `port`:        local port
    - `backlog`:     number of connections the kernel might queue (default: `128`)
    - `buffer_size`: size of the read & write [stream](../stream/#uv_stream) ring buffer (default: `1024`)
    - `shared`:      an optional payload to be shared with all handlers
- Return
    - `int`: TCP error
        - returns only in case of error (always `<0`)

The handler is a user-defined `code/await` with the fixed identifier
`UV_TCP_Server_Handler`, which must be declared in between the includes for
`uv/tcp.ceu` and `uv/tcp-server.ceu`, as follows:

```ceu
#include "uv/tcp.ceu"
code/await UV_TCP_Server_Handler (var& UV_Stream stream, var&? UV_TCP_Server_Data shared) -> void do
    <...>       // handles a new client connection
end
#include "uv/tcp-server.ceu"
<...>
```

The handler receives a [TCP](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t)
[stream](../stream/#uv_stream) of the connected client.

If the macro `UV_TCP_SERVER_HANDLER_MAX` is defined, the server uses a bounded
pool of `UV_TCP_Server_Handler` of that size.

Céu-libuv references:
    [`UV_TCP_Open_Bind_Listen`](#uv_tcp_open_bind_listen),
    [`UV_TCP_Open`](#uv_tcp_open).

libuv references:
    [`_uv_accept`].

#### Example:

Executes a server on `0.0.0.0:7000` and handles each connection inside
`UV_TCP_Server_Handler`:

```ceu
#include "uv/tcp.ceu"

data UV_TCP_Server_Data;    // empty data

code/await UV_TCP_Server_Handler (var& UV_Stream stream, var&? UV_TCP_Server_Data shared) -> void do
    <...>                   // handles a new client connection
end

#include "uv/tcp-server.ceu"

await UV_TCP_Server("0.0.0.0", 7000, _,_,_);
```

<!---------------------------------------------------------------------------->
