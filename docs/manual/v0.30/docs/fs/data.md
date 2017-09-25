## Data Abstractions

### UV_FS_File

A file abstraction.

```ceu
data UV_FS_File with
    var&[] byte  buffer;
    var    usize offset =  0;
    var    int   handle = -1;
    event  void  ok;
end
```

- Fields:
    - `buffer`: alias to the read & write buffer
    - `offset`: current offset for read & write operations
    - `handle`: underlying operating system handle
    - `ok`:     event signalled when the file is opened successfully
