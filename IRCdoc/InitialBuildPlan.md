**What IRC is:** A real-time group chat system from 1988. You connect a client to a server, join channels (chat rooms starting with `#`), and instantly talk with everyone else there. Think of it as the grandfather of Discord — no-frills, open, and still alive.

![[Pasted image 20260404093451.png]]

---

here's the build plan:

![[Pasted image 20260404093549.png]]
![[Pasted image 20260404093647.png]]

## Full summary in plain words

**IRC** is a 35-year-old real-time group chat system built on plain text commands over TCP. You connect a client → join a channel → messages are routed through a network of linked servers to everyone in that channel, in milliseconds.

**Your server** needs: a `poll()` loop for all file descriptors, per-client read buffers (because TCP delivers data in chunks — your biggest hidden challenge), and only 6 commands minimum to get HexChat working: `NICK`, `USER`, `PASS`, `JOIN`, `PRIVMSG`, `PING/PONG`.

**Client to use:** HexChat — it has a raw log window so you can see every IRC command flying back and forth, which is invaluable when debugging.

**Real servers to learn from:** Connect to `irc.libera.chat` first, open the raw log, and just watch the handshake. That half-day will save you two days of confusion when building.

**Two people:** Cuts the timeline from tight to comfortable. The key rule — spend day 1 together agreeing on your data structures before anyone writes a single line of code. Person A owns the core loop and buffers, Person B owns the IRC logic and command handlers. They work in parallel then integrate.

**Is 2 weeks realistic?** Yes — comfortable with two people, tight with one. The hardest parts are partial reads/writes and getting the exact welcome code sequence right for HexChat. Budget time for those.