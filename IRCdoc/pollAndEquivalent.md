These are all **I/O multiplexing** functions — they all solve the same problem: watching multiple file descriptors at once without blocking. Here's each one explained, then a comparison.

---

## The problem they all solve

Without these, you'd have to either:

- Block on one client at a time (useless for a server)
- Fork or thread per client (forbidden in your project)

All four functions let a **single process** watch hundreds of sockets and react only when one is ready.

---

## The four functions

### `poll()`

The one your subject recommends. You give it an array of `pollfd` structs, each containing a file descriptor and the events you're watching for. It blocks until something happens, then tells you which fds are ready.

```c
struct pollfd fds[MAX_CLIENTS];

fds[0].fd = server_fd;
fds[0].events = POLLIN;   // watch for incoming data / connections

int ready = poll(fds, nfds, timeout_ms);

if (fds[0].revents & POLLIN) {
    // server socket is ready — new client connecting
    accept(...);
}
```

Key flags:

- `POLLIN` — there is data to read
- `POLLOUT` — socket is ready to write
- `POLLERR` — error on the socket
- `POLLHUP` — client hung up / disconnected

**The main limitation:** every call you pass the full array, and poll() scans all of it linearly. Slows down with thousands of fds.

---

### `select()`

The oldest of the four, from the original BSD sockets. Uses **bit sets** (`fd_set`) instead of an array. You add fds to three sets — read, write, error — then call select and check which are set afterward.

```c
fd_set read_fds;
FD_ZERO(&read_fds);
FD_SET(server_fd, &read_fds);
FD_SET(client_fd, &read_fds);

select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

if (FD_ISSET(server_fd, &read_fds)) {
    // ready to accept
}
```

Key limitations:

- Maximum 1024 file descriptors hardcoded (`FD_SETSIZE`)
- The fd_set is **destroyed** by each call — you must rebuild it every loop
- You must pass `max_fd + 1` which means tracking the highest fd manually
- Scans linearly just like poll()

For your IRC server with a handful of test clients, the 1024 limit is irrelevant. But the "rebuild every loop" part is annoying code overhead.

---

### `epoll()` — Linux only

The modern Linux solution. Instead of passing the full list every call, you **register** fds with the kernel once, and the kernel tracks them. You only get notified about fds that actually have activity — no linear scan.

```c
int epfd = epoll_create1(0);

struct epoll_event ev;
ev.events = EPOLLIN;
ev.data.fd = server_fd;
epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);  // register once

struct epoll_event events[MAX_EVENTS];
int n = epoll_wait(epfd, events, MAX_EVENTS, timeout);

for (int i = 0; i < n; i++) {
    // events[i].data.fd is ready
}
```

Three separate calls:

- `epoll_create1()` — creates the epoll instance
- `epoll_ctl()` — add, modify, or remove fds from the watch list
- `epoll_wait()` — block until something is ready, returns only ready fds

**Big advantage:** scales to tens of thousands of connections without slowing down. Only returns ready fds — no scanning a 10,000-entry array to find the 3 active ones.

**Big limitation:** Linux only. Won't compile on Mac.

---

### `kqueue()` — BSD / Mac only

The BSD/macOS equivalent of epoll. Same philosophy — register events once, get notified only when they fire. More flexible than epoll because it watches not just sockets but files, processes, signals, timers, all through one interface.

```c
int kq = kqueue();

struct kevent change;
EV_SET(&change, server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
kevent(kq, &change, 1, NULL, 0, NULL);  // register

struct kevent events[MAX_EVENTS];
int n = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);

for (int i = 0; i < n; i++) {
    // events[i].ident is the ready fd
}
```

Like epoll, two phases — register with `EV_ADD`, then wait for events. Scales just as well as epoll.

**Limitation:** Mac/BSD only. Won't compile on Linux.

---

## Direct comparison

||`select()`|`poll()`|`epoll()`|`kqueue()`|
|---|---|---|---|---|
|OS support|Everywhere|Everywhere|Linux only|Mac/BSD only|
|Max fds|1024|Unlimited|Unlimited|Unlimited|
|Scales to many fds|No|No|Yes|Yes|
|Returns only ready fds|No|No|Yes|Yes|
|Rebuild list each call|Yes|Yes|No|No|
|Complexity|Low|Low|Medium|Medium|
|Best for|Tiny projects|Your IRC server|Production Linux|Production Mac|

---

## Which one to actually use for your project

**Use `poll()`** and don't overthink it. Here's why:

Your server will have at most a few dozen test clients during development — maybe a handful in the actual evaluation. At that scale poll(), select(), epoll(), and kqueue() perform identically. The theoretical scaling advantages of epoll/kqueue are completely irrelevant.

poll() is also the simplest mental model — one array, one call, check the results. No separate registration step, no instance to manage. For a project where you're also learning sockets, IRC, and non-blocking I/O simultaneously, that simplicity has real value.

The subject mentions the others only to tell you they're acceptable alternatives, not because you should use them. Stick with poll().

```c
// Your entire event loop in skeleton form
while (true) {
    int ready = poll(fds, nfds, -1);  // -1 = wait forever
    for (int i = 0; i < nfds; i++) {
        if (fds[i].revents & POLLIN) {
            if (fds[i].fd == server_fd)
                handle_new_connection();
            else
                handle_client_data(fds[i].fd);
        }
    }
}
```

That loop, with non-blocking sockets and proper read buffers, is the entire core of your server.