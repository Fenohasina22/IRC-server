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

## Numeric Replies

| Code | Name              | Description                          |
|------|-------------------|--------------------------------------|
| 001  | RPL_WELCOME      | Registration complete                |
| 002  | RPL_YOURHOST     | Server info                          |
| 003  | RPL_CREATED      | Server creation date                 |
| 004  | RPL_MYINFO       | Server capabilities                  |
| 331  | RPL_NOTOPIC      | No topic set                         |
| 332  | RPL_TOPIC        | Channel topic                        |
| 341  | RPL_INVITING     | Invite sent                          |
| 353  | RPL_NAMREPLY     | List of names                        |
| 366  | RPL_ENDOFNAMES   | End of names list                    |
| 401  | ERR_NOSUCHNICK   | Nick not found                       |
| 403  | ERR_NOSUCHCHANNEL| Channel not found                    |
| 421  | ERR_UNKNOWNCOMMAND| Bad command                         |
| 431  | ERR_NONICKNAMEGIVEN| Missing nick                       |
| 432  | ERR_ERRONEUSNICKNAME| Invalid nick                      |
| 433  | ERR_NICKNAMEINUSE| Nick taken                          |
| 441  | ERR_USERNOTINCHANNEL | User not in channel                  |
| 442  | ERR_NOTONCHANNEL | Not in channel                       |
| 443  | ERR_USERONCHANNEL| Already in channel                  |
| 451  | ERR_NOTREGISTERED| Not registered                      |
| 461  | ERR_NEEDMOREPARAMS| Missing parameters                  |
| 462  | ERR_ALREADYREGISTRED | Already registered                   |
| 464  | ERR_PASSWDMISMATCH   | Password incorrect                   |
| 471  | ERR_CHANNELISFULL| Channel limit reached               |
| 473  | ERR_INVITEONLYCHAN| Invite only channel                |
| 475  | ERR_BADCHANNELKEY| Bad channel key                     |
| 482  | ERR_CHANOPRIVSNEEDED| Channel operator privileges needed |

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
