## Socket creation and setup

**`socket()`** Creates a new socket and returns a file descriptor. You call this once at startup to create your server's listening socket. You pass it three args — address family (`AF_INET` for IPv4), type (`SOCK_STREAM` for TCP), and protocol (usually 0).

```c
int fd = socket(AF_INET, SOCK_STREAM, 0);
```

**`setsockopt()`** Sets options on a socket. The one you'll use constantly is `SO_REUSEADDR` — without it, restarting your server while testing will fail with "address already in use" for about a minute while the OS cleans up the old socket.

```c
setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

**`getsockname()`** Gets the local address that a socket is bound to. Useful for confirming which port you're actually listening on, especially if you let the OS pick one.

---

## Address resolution

**`getprotobyname()`** Looks up a protocol by name and returns its number. You'd call `getprotobyname("tcp")` to get the TCP protocol number instead of hardcoding it. Older style — mostly replaced by `getaddrinfo()` in modern code.

**`gethostbyname()`** Resolves a hostname to an IP address. Old, deprecated, not thread-safe. You may see it in older IRC code but `getaddrinfo()` is the modern replacement.

**`getaddrinfo()`** The modern all-in-one address resolver. Give it a hostname and port, it gives you back a linked list of `addrinfo` structs ready to use with `bind()` or `connect()`. Handles both IPv4 and IPv6 transparently.

```c
getaddrinfo("irc.libera.chat", "6667", &hints, &result);
```

**`freeaddrinfo()`** Frees the linked list that `getaddrinfo()` allocated. Always call this after you're done with the result — straightforward memory cleanup.

---

## Binding and listening

**`bind()`** Attaches your socket to a specific address and port on your machine. This is what reserves port 6667 for your server. Must be called before `listen()`.

```c
bind(fd, (struct sockaddr*)&addr, sizeof(addr));
```

**`listen()`** Marks the socket as passive — ready to accept incoming connections. The second argument is the backlog — how many pending connections the OS will queue before refusing new ones.

```c
listen(fd, 10);
```

**`accept()`** Waits for and accepts an incoming connection. Returns a brand new file descriptor for that specific client. Your `poll()` loop calls this when the listening socket has activity. Each client gets their own fd from this point on.

```c
int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
```

**`connect()`** Used by clients to initiate a connection to a server. Your server won't call this — your clients (irssi) call it. Worth knowing it exists but you won't write it in your server code.

---

## Byte order conversion

These exist because different CPU architectures store multi-byte numbers differently (big-endian vs little-endian). The network always uses big-endian. These functions convert between your machine's order and network order.

**`htons()`** — Host TO Network Short. Converts a 16-bit number (like a port) from your CPU's order to network order. Use this when setting the port in your `sockaddr_in`.

```c
addr.sin_port = htons(6667);
```

**`htonl()`** — Host TO Network Long. Same but for 32-bit numbers (like IP addresses).

**`ntohs()`** — Network TO Host Short. Reverse of `htons()`. Use when reading a port number received from the network.

**`ntohl()`** — Network TO Host Long. Reverse of `htonl()`.

---

## IP address conversion

**`inet_addr()`** Converts an IP address string like `"127.0.0.1"` to a 32-bit number in network byte order. Old and limited — can't signal errors well. Mostly replaced by `inet_pton()`.

**`inet_ntoa()`** Converts a 32-bit network address back to a string like `"127.0.0.1"`. Old, not thread-safe. Replaced by `inet_ntop()`.

**`inet_ntop()`** Modern version of `inet_ntoa()`. Converts a binary network address to a human-readable string. Works with both IPv4 and IPv6. You'll use this to log which IP a client connected from.

```c
inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, sizeof(ip_str));
```

---

## Sending and receiving data

**`send()`** Sends data through a socket. Returns how many bytes were actually sent — critical point: it may send **less** than you asked. You must loop until all bytes are sent. Non-blocking sockets can return immediately with a partial write.

```c
send(client_fd, buffer, length, 0);
```

**`recv()`** Receives data from a socket. Returns how many bytes were read — again, may be less than the buffer size. Returns 0 when the client disconnected cleanly, -1 on error. This is where your partial read buffer logic lives.

```c
recv(client_fd, buffer, sizeof(buffer), 0);
```

---

## Signal handling

Signals are the OS's way of notifying your process of events — like the user pressing Ctrl+C, or a child process dying.

**`signal()`** The old, simple way to set a signal handler. You give it a signal number and a function pointer. Limited and has portability issues — `sigaction()` is preferred.

**`sigaction()`** The modern, proper way to handle signals. More control over behavior, safer in multi-threaded environments. You'll use this to handle `SIGINT` (Ctrl+C) cleanly so your server shuts down gracefully instead of leaving sockets in a broken state.

**`sigemptyset()`** Initializes a signal set to empty — no signals in it. Always call this before using a `sigset_t`.

**`sigfillset()`** Fills a signal set with all signals. Used when you want to block everything.

**`sigaddset()`** Adds a specific signal to a set.

```c
sigaddset(&set, SIGINT);
```

**`sigdelset()`** Removes a specific signal from a set.

**`sigismember()`** Checks if a signal is in a set. Returns 1 if yes, 0 if no.

---

## File descriptor operations

**`lseek()`** Moves the read/write position in a file. Not directly used for sockets — sockets don't support seeking. You'll encounter it in general systems programming but it's less relevant for your IRC server.

**`fstat()`** Gets information about a file descriptor — size, type, permissions, timestamps. Useful for checking if an fd is a socket vs a regular file, or for getting file sizes when serving content.

**`fcntl()`** The Swiss army knife of file descriptor control. For your IRC server, the critical use is setting a socket to **non-blocking mode** — which is mandatory given your constraints:

```c
fcntl(fd, F_SETFL, O_NONBLOCK);
```

Without this, `recv()` and `send()` would block your entire server waiting for one slow client.

---

## The event loop

**`poll()`** The heart of your entire server. Takes an array of file descriptors and a timeout, blocks until at least one fd has activity, then tells you which ones are ready. You put your server's listening socket and every client's socket in this array. When `poll()` returns, you loop through and handle whichever ones are ready — new connections on the server socket, incoming data on client sockets, disconnections.

```c
poll(fds_array, nfds, timeout);
```

The `events` field on each entry tells poll what to watch for — `POLLIN` means "tell me when there's data to read", `POLLOUT` means "tell me when I can write". After `poll()` returns, the `revents` field tells you what actually happened.

This single function replaces the need for threads or forking. One `poll()` loop, one process, thousands of clients.