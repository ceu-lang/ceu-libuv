## Code Abstractions

### UV_Stream_Listen

Starts listening for incoming connections in a stream.

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

Céu-libuv references:
    [`ceu_uv_listen`](#TODO),
    [`UV_STREAM_LISTEN`](#TODO).

#### Example

[Opens](#TODO) a `server` tcp handle, binds it to port `7000`, and then enters
in listen mode.
Each incoming connection triggers `ok_listen` whose reaction accepts the
client, prints its address, and closes the connection.

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

<!---------------------------------------------------------------------------->

### UV_Stream_Read

Reads bytes from a stream continuously.

```ceu
code/await UV_Stream_Read (var& _uv_stream_t stream, vector&[] byte buf)
                            -> (event& usize ok)
                                -> int
```

- Parameters
    - `stream`: stream to read from
    - `buf`:    destination buffer
- Initialization
    - `ok`: signalled whenever new data is read to the destination buffer
- Return
    - `int`: read error
        - returns only case of error (always `<0`)

Céu-libuv references:
    [`ceu_uv_read_start`](#TODO),
    [`UV_STREAM_READ`](#TODO).

libuv references:
    [`uv_read_stop`](#TODO).

*Note: all allocated libuv resources are automatically released on termination.*

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
