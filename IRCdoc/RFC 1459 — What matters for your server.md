focused summary of RFC 1459 — everything needed to build a basic server.

---
### 1. The message format

Every single IRC message follows this structure:

```
[:prefix] <command> [params] <CRLF>
```

- The prefix (`:servername` or `:nick!user@host`) is optional — clients don't send it, servers add it when broadcasting
- The command is either a word (`JOIN`, `PRIVMSG`) or a 3-digit number (`001`, `353`)
- Parameters are space-separated. The last parameter can contain spaces if prefixed with `:`
- Every message ends with `\r\n` — not just `\n`
- Maximum message length is 512 characters including the `\r\n`

This is the most important thing in the entire RFC. Get parsing right first.

---

### 2. Connection registration — the handshake

The client must send these three commands in this order:

```
PASS <password>
NICK <nickname>
USER <username> <hostname> <servername> :<realname>
```

`PASS` must come before `NICK` and `USER`. The user is not registered until both `NICK` and `USER` have been received. Only then do you send the welcome sequence back.

Your server's welcome response after a successful handshake:

```
:server 001 nick :Welcome to the network
:server 002 nick :Your host is servername
:server 003 nick :This server was created ...
:server 004 nick :servername version modes
```

---

### 3. The commands

**Connection:**

|Command|What it does|
|---|---|
|`PASS`|Sets connection password — must arrive before NICK/USER|
|`NICK`|Sets or changes nickname — error if already taken (`433`)|
|`USER`|Registers username and realname — completes registration|
|`QUIT`|Client disconnects — server broadcasts to all their channels|

**Channels:**

|Command|What it does|
|---|---|
|`JOIN`|Client joins a channel — created automatically if it doesn't exist. First user becomes channel operator. Send back topic + names list|
|`PART`|Client leaves a channel|
|`PRIVMSG`|Send message to a channel or user|
|`NOTICE`|Like PRIVMSG but server must never auto-reply to it|
|`TOPIC`|View or set channel topic — only ops can set if mode `+t`|
|`NAMES`|List users in a channel|
|`KICK`|Op removes a user from a channel (forced PART)|
|`INVITE`|Invite a user to a channel|
|`MODE`|Change channel or user modes (see below)|

**Keepalive:** (PS: je pense pas on a besoin de ca)

| Command | What it does                                                        |
| ------- | ------------------------------------------------------------------- |
| `PING`  | Either side sends this to check the connection is alive             |
| `PONG`  | Required response to PING — if you don't respond, connection closes |
|         |                                                                     |

---

### 4. Modes you need to handle

**Channel modes:**

```
o  — give/take channel operator privileges
t  — only ops can change topic
i  — invite-only channel
n  — no messages from outside the channel
k  — channel key (password)
m  — moderated (only ops and voiced users can speak)
b  — ban mask
l  — user limit
```

**User modes:**

```
i  — invisible (don't show in /who)
o  — IRC operator
```

---

### 5. What to send after JOIN

When a user successfully joins a channel your server must send back in this order:

```
:nick!user@host JOIN #channel        (broadcast to everyone in channel)
:server 332 nick #channel :topic     (RPL_TOPIC — or 331 if no topic)
:server 353 nick = #channel :@op user1 user2   (RPL_NAMREPLY)
:server 366 nick #channel :End of /NAMES list  (RPL_ENDOFNAMES)
```

---

### 6. The numeric reply codes you must send

**Welcome sequence (required):**

```
001  RPL_WELCOME
002  RPL_YOURHOST
003  RPL_CREATED
004  RPL_MYINFO
```

**Channel responses:**

```
331  RPL_NOTOPIC       — no topic set
332  RPL_TOPIC         — here is the topic
353  RPL_NAMREPLY      — list of users in channel
366  RPL_ENDOFNAMES    — end of names list
```

**Error replies you must handle:**

```
401  ERR_NOSUCHNICK        — nick doesn't exist
403  ERR_NOSUCHCHANNEL     — channel doesn't exist
421  ERR_UNKNOWNCOMMAND    — command not recognized
431  ERR_NONICKNAMEGIVEN   — NICK with no parameter
432  ERR_ERRONEUSNICKNAME  — invalid characters in nick
433  ERR_NICKNAMEINUSE     — nick already taken
441  ERR_USERNOTINCHANNEL  — target not in channel
442  ERR_NOTONCHANNEL      — you're not in that channel
451  ERR_NOTREGISTERED     — command sent before completing handshake
461  ERR_NEEDMOREPARAMS    — not enough parameters
462  ERR_ALREADYREGISTRED  — tried to register twice
464  ERR_PASSWDMISMATCH    — wrong password
482  ERR_CHANOPRIVSNEEDED  — need to be chanop to do this
```

---

### 7. PING/PONG — connection liveness

The server pings each client periodically. If a client doesn't respond within a set time, the connection is closed. Any client that receives a PING must respond immediately with PONG:

```
Server sends:  PING :uranium.libera.chat
Client sends:  PONG :uranium.libera.chat
```

This goes both ways — your server must respond to client PINGs too.

---

### 8. Key implementation notes from section 8

**Read buffers:** Each client connection gets its own private input buffer. You parse it after every `recv()` call, extracting complete `\r\n` terminated messages. Partial messages stay in the buffer until the rest arrives. This is non-negotiable.

**Send queue:** Writes can be partial too. Use a per-client output buffer (FIFO queue). Never assume `send()` sent everything.

**Non-blocking IO:** All network operations must be non-blocking. This is what makes `poll()` work — you never block on one client.

**QUIT on disconnect:** If a client closes the connection without sending QUIT, your server generates a QUIT message on their behalf and broadcasts it to all their channels.

**Channel creation:** A channel is created the moment the first user JOINs it. The first user automatically becomes channel operator (`@`). The channel ceases to exist when the last user leaves.

**MOTD:** Optional but recommended. Sent right after the `001-004` welcome sequence. If you have no MOTD file, send `422 ERR_NOMOTD`.

---

### 9. What you can safely ignore for your project

Everything in section 4.3 (server queries — VERSION, STATS, LINKS, TRACE, ADMIN, INFO), section 4.1.4 (server-to-server), section 5 (optional commands — AWAY, REHASH, RESTART, WALLOPS), and section 8.6 (server-to-server connections). Your server is single-server only, so none of that applies.