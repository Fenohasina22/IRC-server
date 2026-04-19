*This project has been created as part of the 42 curriculum by mratsima, fsamy-an*

## Description

IRC (Internet Relay Chat) is a real-time text-based communication protocol where clients exchange messages through servers using simple, line-based commands.

The objective of this project is to implement a basic IRC server that follows core protocol behaviors. The reference client used for testing and interaction is irssi.

## Instructions

To compile the project, run:

```bash
make
```
This command will generate a bunch of object file and the ./ft_irc program


```bash
make clean
```
This command will remove the object files created except the program


```bash
make fclean
```
This command removes both object files and the program


```bash
make re
```
This command removes object files and the program , then recompiles it


## Usage

To run the IRC server:

```bash
./ft_irc <port 1024-65535> <password>
```

Port requirements

- Use ports `1024–65535` (ports <1024 are privileged and require root; 65535 is the protocol maximum).


## Basic Usage Example

Build and run the server:

```bash
make
./ft_irc 6667 mySecretPassword
```

Quick manual test with `netcat` (each command on its own line):

```bash
nc 127.0.0.1 6667
PASS mySecretPassword
NICK mynick
USER myuser 0 * :Real Name
JOIN #test
PRIVMSG #test :Hello from nc
QUIT :Bye
```

Connect with `irssi` (interactive):

```
/connect 127.0.0.1 6667 mySecretPassword
# then use /nick, /join, /msg inside irssi
```

## Registration Sequence

Clients must complete registration before they can fully interact with the server. Recommended sequence (each line terminated with CRLF):

```text
# optional: capability negotiation
CAP LS

# server password (server binary requires a password argument)
PASS <password>

# choose a nickname
NICK <nick>

# provide username and real name
USER <username> 0 * :Real Name
```

If the server was started with a password, the client must send `PASS` with the same password before or during registration.

## Commands

| Command  | Description                                      |
|----------|--------------------------------------------------|
| CAP      | Negotiates client/server capabilities before full registration |
| PASS     | Provides server password for authentication     |
| NICK     | Sets or changes the client's nickname           |
| USER     | Supplies username and real name to complete registration |
| PING     | Checks connection liveness (expects PONG reply) |
| PONG     | Server response to PING, confirming connection is alive |
| KICK     | Forcibly removes a user from a channel          |
| TOPIC    | Sets or retrieves the channel's topic           |
| MODE     | Changes or queries user/channel modes (permissions and flags) |
| JOIN     | Makes a user enter a channel (creates it if needed) |
| PART     | Makes a user leave a channel                    |
| PRIVMSG  | Sends a message to a user or channel            |
| INVITE   | Invites a user to join a channel                |
| QUIT     | Disconnects a client from the server with a message |
| DISCONNECT | Non-standard command to forcibly close a connection (server-specific) |

## Supported Modes

Modes are changed with the `MODE` command. Syntax (channel modes):

```
MODE <#channel> {[+|-]<modes>} [params...]
```

- `+` adds modes, `-` removes modes. Modes that require parameters (like `k` and `l`) consume parameters in left-to-right order.

| Mode | Type     | Description                          | Example                                       |
|------|----------|--------------------------------------|-----------------------------------------------|
| i    | Channel  | Invite-only channel                  | `MODE #channel +i` / `MODE #channel -i`       |
| t    | Channel  | Topic restricted to operators        | `MODE #channel +t` / `MODE #channel -t`       |
| k    | Channel  | Channel requires a password          | `MODE #channel +k secretpass` / `MODE #channel -k` |
| l    | Channel  | User limit on channel                | `MODE #channel +l 50` / `MODE #channel -l`    |
| o    | Channel  | Channel operator status              | `MODE #channel +o nick` / `MODE #channel -o nick` |

## Behavior & Limits

- Password: the server binary requires a password argument; clients should send `PASS <password>` during registration if the server enforces it.
- Message length: the IRC protocol limits messages to 512 bytes including CRLF; the server aims to follow this—check `parser.cpp` for exact enforcement.
- Nicknames and channels: channel names typically start with `#`. Exact nickname and channel length/character limits are implemented in the parser — consult `parser.cpp` and `Client.cpp` for specifics.
- Modes: channel modes `i`, `t`, `k`, `l`, `o` are supported; mode parameters (for `k` and `l`) are consumed left-to-right.
- Concurrency: the server uses a single process with non-blocking sockets/select or poll (see `Server.cpp`) — stress-test with multiple clients if needed.

## Stopping the Server

- Ctrl+C (SIGINT): stop the server cleanly from the terminal where it was started.
- `DISCONNECT`: a non-standard, server-specific command that may be implemented to forcibly close a connection; behavior depends on server code and privileges.

## Resources

### Documentation
- [RFC 1459](https://www.rfc-editor.org/rfc/rfc1459)
- [GeeksforGeeks: Internet Relay Chat (IRC)](https://www.geeksforgeeks.org/computer-networks/internet-relay-chat-irc/)
- [GeeksforGeeks: Socket programming](https://www.geeksforgeeks.org/c/socket-programming-cc/)

### AI Usage

AI was used as a support tool in the following areas:

- Understanding IRC fundamentals through practical examples (client/server interaction, usage flow)
- Breaking down IRC server components to assist with task organization
- Referencing IRC numeric replies with their syntax and their meanings
- Improving productivity for simple utility implementations (e.g., helper functions like toLower)
- Creating full reference documentation for IRC
