## Data Abstractions

### UV_Stream

A stream handle.

```ceu
data UV_Stream with
    var&[] byte         buffer;
    var&   _uv_stream_t handle;
end
```

- Fields:
    - `buffer`: alias to the read & write buffer
    - `handle`: underlying operating system handle
