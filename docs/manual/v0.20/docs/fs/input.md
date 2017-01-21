## Input Events

### UV_FS

```ceu
input _uv_fs_t&& UV_FS;
```

- Occurrence:
    - Whenever a filesystem operation completes.
- Payload:
    - `_uv_fs_t&&`: pointer to the operation request

libuv reference: <http://docs.libuv.org/en/v1.x/fs.html>
