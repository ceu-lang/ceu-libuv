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
    [`ceu_uv_listen`](#TODO),
    [`UV_STREAM_LISTEN`](#TODO).

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
    [`ceu_uv_read_start`](#TODO),
    [`UV_STREAM_READ`](#TODO).

libuv references:
    [`uv_read_stop`](#TODO).

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
    [`UV_Stream_Read`](#TODO).

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
    [`ceu_uv_write`](#TODO),
    [`UV_STREAM_WRITE`](#TODO).

*Note: all allocated libuv resources are automatically released on termination.*

<!---------------------------------------------------------------------------->
