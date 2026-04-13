- [x] **1. Client and Channel data structures**

Everything else depends on these. You can't write a single command handler without knowing what a `Client` and `Channel` look like. Do this first, agree on it with your partner, don't change it later.

---

- [x] **2. Message parser**

Second because every command handler calls it. Build it, test it with raw strings, make sure it correctly handles prefix, command, params, and trailing before moving on.

---

- [x] **3. Registration flow — PASS, NICK, USER, welcome sequence**

Third because every other command requires a registered client. Get irssi to successfully connect to your server and see the welcome message before writing any other command. This is your first major milestone — **irssi connects and shows you as online**.

---

- [ ] **4. QUIT and cleanup**

Do this right after registration, not at the end. You'll be connecting and disconnecting constantly while testing everything else. If QUIT is broken, your server leaks state and crashes during testing of other commands.

---

- [/] **5. Command dispatcher**

Now that you have a parser and at least PASS/NICK/USER/QUIT working, build the full dispatcher. Add all the command strings, route to stub functions that just print "command received" for now. This gives you the skeleton everything else plugs into.

---

- [x] **6. Message delivery — PRIVMSG, NOTICE, broadcast**

Before channels, do direct user-to-user PRIVMSG. It's simpler — no channel lookup, just find the target nick and send. Then build broadcast which channels will rely on heavily.

---

- [/] **7. Channel management — JOIN, PART, NAMES, TOPIC, KICK, INVITE**

Now channels. Do them in this sub-order:

- JOIN first — you need it to get into a channel at all
- PART — clean exit
- TOPIC — simple get/set
- KICK — requires operator check
- INVITE — requires operator check

---

- [/] **8. All numeric reply codes**

You'll have been adding these as you go. At this point do a full pass — check every command handler sends the right error codes for every edge case. Missing a `461` here or a `442` there is what causes irssi to behave strangely.

---

- [ ] **9. MODE handling**

Last because it's the most complex and the least critical for basic functionality.
· i: Set/remove Invite-only channel
· o: Give/take channel operator privilege
· t: Set/remove the restrictions of the TOPIC command to channel operators 
· k: Set/remove the channel key (password)
· l: Set/remove the user limit to channel

---
- [ ] **10. Handle multi-args /join and /privmsg 

like join a,b,c or msg a,b,c

--------------------------------------
- [ ] **11. put a limit and error check on the port 

check program argument

----------
**In short:**

```
Structs → Parser → Registration → QUIT → Dispatcher 
→ PRIVMSG → Channels → Error codes → MODE
```

Each step you should be able to test with irssi before moving to the next.