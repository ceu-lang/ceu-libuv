#include <stdio.h>
#include <uv.h>

void wait_for_a_while (uv_idle_t* handle) {
    static int64_t counter = 0;
    counter++;
    if (counter >= 10e6)
        uv_idle_stop(handle);
}

int main() {
    uv_loop_t loop;
    uv_loop_init(&loop);

    uv_idle_t idler;
    uv_idle_init(&loop, &idler);
    uv_idle_start(&idler, wait_for_a_while);

    printf("Idling...\n");
    uv_run(&loop, UV_RUN_DEFAULT);

    uv_loop_close(&loop);
    return 0;
}
