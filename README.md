Céu-libuv supports the development of libuv applications in the programming
language Céu:

- Source Code:   https://github.com/fsantanna/ceu-libuv/
- Documentation: http://fsantanna.github.io/ceu-libuv/
- Chat:          https://gitter.im/fsantanna/ceu

[libuv](http://www.libuv.org/) libuv is a multi-platform C library that
provides support for asynchronous I/O based on event loops.

Céu is a reactive language that aims to offer a higher-level and safer
alternative to C:

- Home Page:   http://www.ceu-lang.org/
- Source code: https://github.com/fsantanna/ceu/

Céu-libuv empowers the development of libuv applications with the following
extensions:

- Awaiting events in direct/sequential style.
  <!-- (e.g., timers, key presses, mouse motion, etc).-->
- Parallel lines of execution with
    - safe abortion;
    - deterministic behavior (in contrast with threads).
- Asynchronous loops for heavy computations.
- Seamless integration with standard C (e.g., `random`, `strlen`, etc).

Install
=======

## Install libuv:

```
$ sudo apt-get install libuv1-dev
```

(Assuming a Linux/Ubuntu machine.)

## Install Céu:

https://github.com/fsantanna/ceu/

## Clone Céu-libuv:

```
$ git clone https://github.com/fsantanna/ceu-libuv
$ cd ceu-libuv/
```

## Compile and Run

Edit the `Makefile` to point to your `ceu` directory and then run `make`:

```
$ gedit Makefile
$ make
```

If necessary, configure the variables in the `Makefile`.
The default example prints the time elapsing during 5 seconds.

To compile and run another application, run `make` and set `CEU_SRC`:

```
$ make CEU_SRC=<path-to-ceu-application>
```

Examples
========

The `samples/` directory contains a number of examples.

To run all examples one after another, run `make samples`:

```
$ make samples
```

Opening a File
--------------

The exmple `fs-00.ceu` opens `file.txt` and prints *opening ok* after the file
is ready for use.
In case of failure, prints *opening error* along with the error code:

```
#include "uv/fs.ceu"

var& UV_FS_File file;

var int? err =
    watching UV_FS_Open("file.txt", _O_RDONLY, 0) -> (&file) do
        await file.ok;
        // file is ready for use
        _printf("opening ok\n");
    end;
if err? then
    _printf("opening error: %d\n", err!);
end

escape 0;
```
