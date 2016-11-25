# HTTP/1.1 Web Server

## Installation

```bash
make
```

## In Progress
- Determine an approach where `http_digest_initial_line()` and
  `http_digest_header_line()` may back out of request parsing and terminate the
  connection in the event of both client and server error (i.e. 400 Bad Request
  or 500 Internal Server Error)
- Finalize `http_request` data structure.
- Research IO Multiplexing performance advantages.
  - epoll(7)
  - kqueue()
- Transition away from `str*` functions and instead use bounded `strn*`
  functions.
- Terminating the server (Ctrl-C) with a client connected causes a memory leak
  (remains available).
- Implement connection time-outs.
  - `setsockopt()`
    - `SO_SNDTIMEO`
    - `SO_RCVTIMEO`
- Implement a minimum transfer speed to attempt to prevent slowloris attacks.
