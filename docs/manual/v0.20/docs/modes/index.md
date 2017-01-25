Mode of Operation
=================

The mode of operation specifies how Céu-libuv captures events from the
environment (e.g., timers and incoming network traffic) and redirects them to
the Céu application.
It is implemented in C and is part of Céu-libuv.

Céu-libuv maps each libuv request/callback to a corresponding request/input in
Céu.
As an example, instead of reading from a stream with `uv_read_start`, Céu-libuv
uses `ceu_uv_read_start` which generates
[`UV_STREAM_READ`](../stream/#uv_stream_read) input events back to the
application, as follows:

```ceu
#define ceu_uv_read_start(stream) uv_read_start(stream,...,ceu_uv_read_start_cb);

void ceu_uv_read_start_cb(uv_stream_t* stream, ...) {
    <...>
    ceu_input(CEU_INPUT_UV_STREAM_READ, <stream>);
}
```

Under the hood, Céu-libuv uses one *event loop*, one *timer*, and one *async*
libuv handles.
The timer manages Céu timers.
The async manages Céu asyncs and threads.
The main event loop makes continuous calls to `uv_run` passing `UV_RUN_ONCE`:

```c
int main (void) {
    ceu_start();
    while (<program-is-running>) {
        uv_run(&loop, UV_RUN_ONCE);         // handles all libuv callbacks
        ceu_input(CEU_INPUT__ASYNC, NULL);  // handles timers and asyncs
    }
    ceu_stop();
}
```
