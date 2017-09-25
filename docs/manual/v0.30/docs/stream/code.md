## Code Abstractions

<!---------------------------------------------------------------------------->

### UV_Stream_Listen

Starts listening for incoming connections in a [stream](#uv_stream).

```ceu
code/await UV_Stream_Listen (var& UV_Stream stream, var int? backlog)
                                -> (event void ok)
                                    -> int
```

- Parameters
    - `stream`:  [stream](#uv_stream) to listen
    - `backlog`: number of connections the kernel might queue (default: `128`)
- Public fields
    - `ok`: event signalled on every new incoming connection
- Return
    - `int`: operation status
        -  `0`: success
        - `<0`: error

Céu-libuv references:
    [`ceu_uv_listen`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_listen),
    [`UV_STREAM_LISTEN`](#uv_stream_listen).

#### Example

Opens a [TCP stream](../tcp/#uv_tcp_open), binds it to port `7000`, and then
enters in listen mode.
Each incoming connection triggers the event `ok`.

```ceu
#include "uv/tcp.ceu"

var&? UV_TCP_Open tcp = spawn UV_TCP_Open(_);
watching tcp do
    var _sockaddr_in addr = _;
    _uv_ip4_addr("0.0.0.0", 7000, &&addr);
    _uv_tcp_bind(&&tcp.stream.handle as _uv_tcp_t&&, &&addr as _sockaddr&&, 0);

    var&? UV_Stream_Listen listen = spawn UV_Stream_Listen(&tcp.stream,_);
    watching listen do
        every listen.ok do
            <...>   // handle incoming connections
        end
    end
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_Stream_Read_N

Reads a specified number of bytes from the [stream](#uv_stream) to its buffer.

```ceu
code/await UV_Stream_Read_N (var& UV_Stream stream, var usize? n) -> ssize
```

- Parameters
    - `stream`: [stream](#uv_stream) to read
    - `n`:      number of bytes to read (default: whatever arrives in the stream)
- Return
    - `ssize`: number of bytes read from `stream`
        - `>=0`: number of bytes (not related to `n`)
        - `<0`:  read error

After returning, if no errors occur, the [stream](#uv_stream) buffer will
contain at least `n` bytes.
If the buffer already contains `n` bytes in the beginning, no read occurs and
`0` is returned.

Céu-libuv references:
    [`ceu_uv_read_start`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_read_start),
    [`UV_STREAM_READ`](#uv_stream_read).

libuv references:
    [`uv_read_stop`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_read_stop).

#### Example

Connects to `127.0.0.1:7000` and reads and writes in a loop:

```ceu
#include "uv/tcp.ceu"

var&? UV_TCP_Connect c = spawn UV_TCP_Connect("127.0.0.1", 7000, _);
watching c do
    await c.ok;

    loop do
        await UV_Stream_Read_N(&c.stream,_);    // reads anything
        _printf("%s\n", &&c.stream.buffer[0]);  // shows it in the screen
        await UV_Stream_Write_N(&c.stream,_);   // writes it back
    end
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_Stream_Read_Line

Reads a line from a [stream](#uv_stream).

```ceu
code/await UV_Stream_Read_Line (var& UV_Stream stream, var&[] byte line) -> ssize
```

- Parameters
    - `stream`: [stream](#uv_stream) to read
    - `line`:   alias to destination buffer (excludes the leading `\n`)
- Return
    - `ssize`: number of bytes read from `stream`
        - `>=0`: number of bytes (not related to `n`)
        - `<0`:  read error

Céu-libuv references:
    [`UV_Stream_Read_N`](uv_stream_read_n).

#### Example

Connects to `127.0.0.1:7000` and reads and writes in a loop:

```ceu
#include "uv/tcp.ceu"

var&? UV_TCP_Connect c = spawn UV_TCP_Connect("127.0.0.1", 7000, _);
watching c do
    await c.ok;

    loop do
        var[] byte line;
        await UV_Stream_Read_Line(&c.stream,&line);     // reads a line
        _printf("%s\n", &&line[0]);                     // shows it in the screen
        line = line .. "\n" .. c.stream.buffer;
        c.stream.buffer = [] .. line;
        await UV_Stream_Write_N(&c.stream,_);           // writes it back
    end
end

escape 0;
```

<!---------------------------------------------------------------------------->

### UV_Stream_Write_N


Writes a specified number of bytes to the [stream](#uv_stream) from its
buffer.

```ceu
code/await UV_Stream_Write_N (var& UV_Stream stream, var usize? n) -> ssize
```

- Parameters
    - `stream`: [stream](#uv_stream) to write
    - `n`:      number of bytes to write (default: current size of the `stream` buffer)
- Return
    - `ssize`: number of bytes written
        - `>=0`: number of bytes
        - `<0`:  write error

The written bytes are removed from the stream buffer.

Céu-libuv references:
    [`ceu_uv_write`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_write),
    [`UV_STREAM_WRITE`](#uv_stream_write).

#### Example

Connects to `127.0.0.1:7000` and reads and writes in a loop:

```ceu
#include "uv/tcp.ceu"

var&? UV_TCP_Connect c = spawn UV_TCP_Connect("127.0.0.1", 7000, _);
watching c do
    await c.ok;

    loop do
        await UV_Stream_Read_N(&c.stream,_);    // reads anything
        _printf("%s\n", &&c.stream.buffer[0]);  // shows it in the screen
        await UV_Stream_Write_N(&c.stream,_);   // writes it back
    end
end

escape 0;
```

<!---------------------------------------------------------------------------->
