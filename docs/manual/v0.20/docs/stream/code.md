## Code Abstractions

<!---------------------------------------------------------------------------->

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
    [`ceu_uv_listen`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_listen),
    [`UV_STREAM_LISTEN`](#uv_stream_listen).

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
        - returns only in case of error (always `<0`)

Céu-libuv references:
    [`ceu_uv_read_start`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_read_start),
    [`UV_STREAM_READ`](#uv_stream_read).

libuv references:
    [`uv_read_stop`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_read_stop).

*Note: all allocated libuv resources are automatically released on termination.*

<!---------------------------------------------------------------------------->

### UV_Stream_ReadLine

Reads a single line from a stream.

```ceu
code/await UV_Stream_ReadLine (var& _uv_stream_t stream, vector&[] byte string)
                                -> void
```

- Parameters
    - `stream`: stream to read from
    - `string`: destination string buffer
- Return
    - `void`: nothing

Céu-libuv references:
    [`UV_Stream_Read`](uv_stream_read_1).

<!---------------------------------------------------------------------------->

### UV_Stream_Write

Write bytes to a stream.

```ceu
code/await UV_Stream_Write (var& _uv_stream_t stream, vector&[] byte buf)
                                -> int
```

- Parameters
    - `stream`: stream to write to
    - `buf`:    source buffer
- Return
    - `int`: operation status
        -  `0`: success
        - `<0`: error

Céu-libuv references:
    [`ceu_uv_write`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_write),
    [`UV_STREAM_WRITE`](#uv_stream_write).

*Note: all allocated libuv resources are automatically released on termination.*

<!---------------------------------------------------------------------------->
