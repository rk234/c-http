# C-HTTP

A simple, zero-dependency, concurrent and extensible HTTP server written in C for Linux.

_May not be fully standards compliant! This is mainly meant as a learning exercise._

## Registering route handlers

C-HTTP supports registering custom route handlers as follows:

```c
server_add_handler(&server, "GET", "/", index_handler);
```

Where `index_handler` is a function that takes in a pointer to an `http_req_t` and `http_resp_t`.
It modifies the `http_resp_t` before returning to send a response.

More examples can be found in `main.c`.

## Building

Requires GCC and GNU Make.

```bash
$ make setup
$ make
```

The compiled binary will be located at `./dist/server`.

## TODO

- [ ] Dynamic route parameters
- [ ] Better standads compliance
