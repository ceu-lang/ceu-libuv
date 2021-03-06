## Input Events

### UV_STREAM_LISTEN

```ceu
input (_uv_stream_t&&, int) UV_STREAM_LISTEN;
```

- Occurrence:
    - Whenever a stream server receives an incoming connection.
- Payload:
    - `_uv_stream_t&&`: pointer to the stream server

libuv reference: <http://docs.libuv.org/en/v1.x/stream.html#c.uv_connection_cb>

### UV_STREAM_CONNECT

```ceu
input (_uv_connect_t&&, int) UV_STREAM_CONNECT;
```

- Occurrence:
    - Whenever a connection opens.
- Payload:
    - `_uv_connect_t&&`: pointer to the connection
    - `int`: open status
        -  `0`: success
        - `<0`: error

libuv reference: <http://docs.libuv.org/en/v1.x/stream.html#c.uv_connect_cb>

### UV_STREAM_READ

```ceu
input (_uv_stream_t&&, ssize) UV_STREAM_READ;
```

- Occurrence:
    - Whenever data is available on a stream.
- Payload:
    - `_uv_stream_t&&`: pointer to the stream
    - `ssize`: number of bytes available
        - `>0`: data available
        - `<0`: error

libuv reference: <http://docs.libuv.org/en/v1.x/stream.html#c.uv_read_cb>

### UV_STREAM_WRITE

```ceu
input (_uv_write_t&&, int) UV_STREAM_WRITE;
```

- Occurrence:
    - Whenever writing to a stream completes.
- Payload:
    - `_uv_write_T&&`: pointer to the write request
    - `int`: completion status
        -  `0`: success
        - `<0`: error

libuv reference: <http://docs.libuv.org/en/v1.x/stream.html#c.uv_write_cb>

### UV_STREAM_ERROR

```ceu
input (_uv_stream_t&&, int) UV_STREAM_ERROR;
```

- Occurrence:
    - Whenever a read or write error occurs in a stream.
- Payload:
    - `_uv_stream_t&&`: pointer to the stream
    - `int`: error code

`UV_STREAM_ERROR` always occurs before the corresponding [`UV_STREAM_READ`](#uv_stream_read) or
[`UV_STREAM_WRITE`](#uv_stream_write).

libuv reference: <http://docs.libuv.org/en/v1.x/errors.html>
