Mode of Operation
=================

`TODO`

<!--

The mode of operation specifies how Céu-libuv captures events from the
environment (e.g., timers and incoming network traffic) and redirects them to
the Céu application.
It is implemented in C and is part of Céu-libuv.

Céu-libuv maps each libuv callback to 

Each mode is described in pseudo-code as follows.

- FS
- tcp
- thread
- timers

callback -> event

Waiting Mode
------------

The *waiting mode* of Céu-SDL continually waits for SDL input events in an
infinite loop:

```c
int main (void) {
    ceu_start();
    while (<program-is-running>) {
        int timeout = (<program-has-pending-async>) ? 0 : <next-timer-in-ms>;
        if (SDL_WaitEventTimeout(<...>, timeout);
            ceu_input(CEU_INPUT__*, <...>);     /* input SDL_QUIT, SDL_KEY*, SDL_MOUSE*, etc */
        }
        ceu_input(CEU_INPUT__NONE, <time>);     /* input async and timer */
        ceu_input(CEU_INPUT_SDL_REDRAW, <...>); /* input SDL_REDRAW after every input */
    }
    ceu_stop();
    return <program-escape-value>;
}
```

The inputs are polled on each loop iteration and changes are notified to the
Céu application through `ceu_input` calls.

-->
