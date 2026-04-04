## **Phase 1 — Install and configure irssi**

- [x] Install irssi: `sudo apt install irssi` (or `brew install irssi` on Mac)

- [x] Launch it and understand the layout — status window, input bar, activity bar: `irssi`

- [x] Connect to Libera: ```
/connect irc.libera.chat 6667`

- [ ] Learn window navigation — this is irssi's biggest UX hurdle: `Alt+1..9` or `/window 1` to switch windows

## **Phase 2 — Observe the raw protocol**

- [x] Open the raw log to a file, keep it open in a second terminal: `/rawlog open ~/irc_raw.log`,  then `tail -f ~/irc_raw.log` in the second terminal

- [x] Disconnect and reconnect — watch the full 001 002 003 004 handshake in the raw log: `/disconnect` then `/connect libera`

- [ ] Join a busy channel and watch PING/PONG appear in the raw log: `/join #linux`

- [ ] Write down the exact handshake sequence — you will replicate this in your server


## **Phase 3 — Learn the commands**

- [ ] Change your nickname and watch the raw NICK command appear: `/nick YourNewName`

- [ ] Join, chat in, and leave a channel — observe JOIN and PART in the raw log: `/join #test` then type messages then `/part`

- [ ] List users in a channel — observe the 353 and 366 numeric replies: `/names #channel`

- [ ] Send a private message and observe PRIVMSG in the raw log: `/msg nickname hello`

- [ ] Set a channel topic and observe the TOPIC command: `/topic #channel My topic here`

  

## **Phase 4 — Simulate multi-client with two irssi instances**

- [ ] Open a second terminal and launch a second irssi instance with a different nick: `irssi` then `/nick OtherNick` then `/connect libera`

- [ ] Join the same channel from both instances and chat between them

- [ ] Watch both raw logs simultaneously — see how broadcast looks from each side: `tail -f ~/irc_raw.log` in two terminals

  

## **Phase 5 — Read the spec**

- [ ] Read RFC 1459 sections 1–4 with your raw log notes beside you: `https://www.rfc-editor.org/rfc/rfc1459`

- [ ] Cross-reference the numeric reply codes you saw (001, 353, 366...) against the RFC

- [ ] Add your localhost server to irssi — ready to test the moment your server boots: `/server add -name myserver 127.0.0.1 6667 yourpassword`

- [ ] You are ready to start building