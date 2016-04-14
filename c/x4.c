#include <assert.h>
#include <stdio.h>
#include <uv.h>

uv_loop_t loop;
uv_fs_t open_req, read_req, close_req;

static char buffer[1024];
static uv_buf_t iov;

void on_close (uv_fs_t *req) {
    assert(req == &close_req);
    printf("CLOSED\n");
}

void on_read (uv_fs_t *req) {
    static int offset = 0;
    assert(req == &read_req);
    assert(read_req.result >= 0);
    if (read_req.result == 0) {
        uv_fs_req_cleanup(&read_req);
        uv_fs_req_cleanup(&open_req);
        uv_fs_close(&loop, &close_req, open_req.result, on_close);
    }
    else if (read_req.result > 0) {
        iov.len = read_req.result;
        iov.base[iov.len] = '\0';
        printf("%s", iov.base);
        //uv_fs_write(uv_default_loop(), &write_req, 1, &iov, 1, -1, on_write);
        uv_fs_req_cleanup(&read_req);

        offset += read_req.result;
        uv_fs_read(&loop, &read_req, open_req.result, &iov, 1, offset, on_read);
    }
}

void on_open (uv_fs_t* req) {
    assert(req == &open_req);
    assert(read_req.result >= 0);

    iov = uv_buf_init(buffer, sizeof(buffer)-1);
    uv_fs_read(&loop, &read_req, open_req.result, &iov, 1, -1, on_read);
}

int main() {
    uv_loop_init(&loop);
    uv_fs_open(&loop, &open_req, "x3.txt", O_RDONLY, 0, on_open);
    uv_run(&loop, UV_RUN_DEFAULT);
    uv_loop_close(&loop);
    return 0;
}

uv_loop_t loop;
uv_tcp_t server, client;
struct sockaddr_in addr;

void on_echo(uv_stream_t *server, int status) {
    assert(status >= 0);

    uv_tcp_init(&loop, &client);
    if (uv_accept(&server, (uv_stream_t*)&client) == 0) {
        uv_read_start((uv_stream_t*)&client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*)&client, NULL);
    }
}

void on_new_connection(uv_stream_t *server, int status) {
    assert(status >= 0);

    uv_tcp_init(&loop, &client);
    if (uv_accept(&server, (uv_stream_t*)&client) == 0) {
        uv_read_start((uv_stream_t*)&client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*)&client, NULL);
    }
}

int main() {
    uv_tcp_init(&loop, &server);

    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);

    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
    assert(r == 0);

    return uv_run(&loop, UV_RUN_DEFAULT);
}

