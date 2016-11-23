# HTTP/1.1 Web Server

## Installation

```bash
make
```

## In Progress
- Research IO Multiplexing performance advantages.
  - epoll(7)
  - kqueue()
- Transition away from `str*` functions and instead use bounded `strn*`
  functions.
- Finalize `http_request` data structure.
- Terminating the server (Ctrl-C) with a client connected causes a memory leak
  (remains available).
- Implement connection time-outs.
- Implement a minimum transfer speed to attempt to prevent slowloris attacks.
