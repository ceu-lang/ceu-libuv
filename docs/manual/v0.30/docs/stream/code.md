## Code Abstractions

<!---------------------------------------------------------------------------->

### UV_Stream_Listen

Starts listening for incoming connections in a [stream handle](#uv_stream).

```ceu
code/await UV_Stream_Listen (var& UV_Stream stream, var int? backlog)
                                -> (event void ok)
                                    -> int
```

- Parameters
    - `stream`:  [stream handle](#uv_stream) to listen
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

<!---------------------------------------------------------------------------->

### UV_Stream_Read_N

Reads a specified number of bytes in the [stream handle](#uv_stream) to its buffer.

```ceu
code/await UV_Stream_Read_N (var& UV_Stream stream, var usize? n) -> ssize
```

- Parameters
    - `stream`: [stream handle](#uv_stream) to read
    - `n`:      number of bytes to read (default: whatever arrives in the stream)
- Return
    - `ssize`: number of bytes read from `stream`
        - `>=0`: number of bytes (not related to `n`)
        - `<0`:  read error

After returning, if no errors occur, the [stream handle](#uv_stream) buffer
will contain at least `n` bytes.
If the buffer already contains `n` bytes in the beginning, no read occurs and
`0` is returned.

Céu-libuv references:
    [`ceu_uv_read_start`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_read_start),
    [`UV_STREAM_READ`](#uv_stream_read).

libuv references:
    [`uv_read_stop`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_read_stop).

<!---------------------------------------------------------------------------->

### UV_Stream_Read_Line

Reads a line from a [stream handle](#uv_stream).

```ceu
code/await UV_Stream_Read_Line (var& UV_Stream stream, var&[] byte line) -> ssize
```

- Parameters
    - `stream`: [stream handle](#uv_stream) to read
    - `line`:   alias to destination buffer (excludes the leading `\n`)
- Return
    - `ssize`: number of bytes read from `stream`
        - `>=0`: number of bytes (not related to `n`)
        - `<0`:  read error

Céu-libuv references:
    [`UV_Stream_Read_N`](uv_stream_read_n).

<!---------------------------------------------------------------------------->

### UV_Stream_Write_N

Writes a specified number of bytes in the [stream handle](#uv_stream) from its buffer.

```ceu
code/await UV_Stream_Write_N (var& UV_Stream stream, var usize? n) -> ssize
```

- Parameters
    - `stream`: [stream handle](#uv_stream) to write
    - `n`:      number of bytes to write (default: current size of the `stream` buffer)
- Return
    - `ssize`: number of bytes written
        - `>=0`: number of bytes
        - `<0`:  write error

Céu-libuv references:
    [`ceu_uv_write`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_write),
    [`UV_STREAM_WRITE`](#uv_stream_write).

<!---------------------------------------------------------------------------->
