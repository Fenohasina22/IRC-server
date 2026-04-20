# 🧭 HOW TO USE

Each item follows:

- **Action** → what you do

- **Expected** → what should happen

- **Verify** → what to check concretely


Mark ✔ only if all 3 are satisfied.

---

# ⚙️ PREPARATION

- [x] **Build server**
    - [x] → Run `make`
    - [x] → Expected: no errors
    - [x] → Verify: binary exists

- [x] **Check port availability**
    - [x] → Use port `6667`
    - [x] → Expected: free
    - [x] → Verify: server binds successfully
    - [x] ->verify min and max ports 


---

# 🚀 LIFECYCLE

- [x] **Start server**
    - [x] → Launch binary
    - [x] → Expected: listening
    - [x] → Verify: can connect via `nc`

- [x] **Graceful shutdown**
    - [x] → Send `SIGINT`
    - [x] → Expected: clean exit
    - [x] → Verify: port freed

- [x] **Restart immediately**
    - [x] → Restart server
    - [x] → Expected: no bind error
    - [x] → Verify: connects again


---

# 🔌 CONNECTION & REGISTRATION

- [x] **Valid registration**
    - [x] → Send `PASS`, `NICK`, `USER`
    - [x] → Expected: `001–004`
    - [x] → Verify: correct order + format

- [x] **Missing USER/NICK**
    - [x] → Omit one
    - [x] → Expected: no welcome
    - [x] → Verify: no `001`

- [x] **Duplicate nick**
    - [x] → Two clients same nick
    - [x] → Expected: `433`
    - [x] → Verify: second rejected


---

# 🚪 JOIN

- [x] **Create channel**
    - [x] → `/join #chan`
    - [x] → Expected: JOIN - [ ] → 331/332 - [ ] → 353 - [ ] → 366
    - [x] → Verify: correct order

- [x] **Join existing channel**
    - [x] → 2 clients join
    - [x] → Expected: broadcast JOIN
    - [x] → Verify: both see each other

- [x] **Invalid channel**
    - [x] → `JOIN chan`
    - [x] → Expected: `403`
    - [x] → Verify: no channel created

- [x] **Invite-only (+i)**
    - [x] → Enable +i
    - [x] → Expected: join blocked
    - [x] → Verify: `473`

- [x] **Key (+k)**
    - [x] → Wrong key
    - [x] → Expected: reject
    - [x] → Verify: `475`

- [x] **Limit (+l)**
    - [x] → Exceed limit
    - [x] → Expected: reject
    - [x] → Verify: `471`


---

# 💬 PRIVMSG

- [x] **User message**
    - [x] → `PRIVMSG nick :msg`
    - [x] → Expected: received
    - [x] → Verify: correct prefix

- [x] **Channel message**
    - [x] → `PRIVMSG #chan :msg` (someone not member of a channel can send message in it but can't receive is it normal)
    - [x] → Expected: broadcast
    - [x] → Verify: all members receive

- [x] **Missing target**
    - [x] → `PRIVMSG`
    - [x] → Expected: `411` (there is 461 and no 411 in the code)

- [x] **Missing message**
    - [x] → `PRIVMSG nick`
    - [x] → Expected: `412` (there is 461 and no 412 in the code)



---

# 🏷️ TOPIC

- [x] **Set topic**
    - [x] → `TOPIC #chan :text`
    - [x] → Expected: broadcast
    - [x] → Verify: all updated

- [x] **Get topic**
    - [x] → `TOPIC #chan`
    - [x] → Expected: `332` or `331`

- [x] **Restricted (+t)**
    - [x] → non-op sets topic
    - [x] → Expected: `482`


---

# 🛠️ MODE

## Query

- [x] **Get modes**
    - [x] → `MODE #chan`
    - [x] → Expected: `324 #chan +modes`
    - [x] → Verify: order = `itkl`, empty = `+`


---

## Change

- [x] **Enable mode**
    - [x] → `MODE #chan +i`
    - [x] → Expected: broadcast
    - [x] → Verify: only `+i` sent

- [x] **Disable mode**
    - [x] → `MODE #chan -i`
    - [x] → Expected: broadcast

- [x] **Invalid mode**
    - [x] → `MODE #chan +z` 
    - [x] → Expected: `472`

- [x] **Missing param**
    - [x] → `MODE #chan +k`
    - [x] → Expected: `461`


---

## Mode enforcement

- [x] **+i blocks JOIN - [ ] → `473`**

- [x] **+k blocks JOIN - [ ] → `475`**

- [x] **+l blocks JOIN - [ ] → `471`**

- [x] **+t blocks TOPIC - [ ] → `482`**


---

# 👑 OPERATORS (+o)

- [x] **Grant op**
    - [x] → `MODE #chan +o nick`
    - [x] → Expected: `@nick` in NAMES

- [x] **Remove op**
    - [x] → `MODE #chan -o nick`
    - [x] → Expected: removed

- [x] **Non-op action**
    - [x] → try KICK/MODE
    - [x] → Expected: `482`


---

# 🥾 KICK

- [x] **Kick user**
    - [x] → `KICK #chan nick`
    - [x] → Expected: removed + broadcast

- [x] **Invalid target**
    - [x] → Expected: `441`


---

# ✉️ INVITE

- [x] **Invite user**
    - [x] → `INVITE nick #chan`
    - [x] → Expected: target notified + `341`

- [x] **Join after invite**
    - [x] → Expected: succeeds

# 💣 NC BREAK TESTS

## Setup

`nc localhost 6667`

---

- [x] **Random input**
    - [x] → `HELLO`
    - [x] → Expected: no crash

- [/] **Missing CRLF**
    - [/] → partial send (maybe u need to test it more throughthully ?)
    - [x] → Expected: no crash

- [?] **Invalid JOIN**
    - [?] → `JOIN` (nothing happens)
    - [?] → Expected: `461` (nothing)

- [x] **Broken PRIVMSG**
    - [x] → `PRIVMSG nick`
    - [x] → Expected: `412` (461 caught)

- [x] **Broken MODE**
    - [x] → `MODE #chan i`
    - [x] → Expected: reject

- [?] **Invalid limit**
    - [?] → `MODE #chan +l -5` (461 not enough params)
    - [?] → Expected: reject

- [ ] **Flood test**
    - [ ] → spam commands
    - [ ] → Expected: stable

- [ ] **Long message (>512)**
    - [ ] → Expected: safe handling

- [ ] **Unknown command**
    - [ ] → `FOOBAR`
    - [ ] → Expected: `421`


---

# 🧱 EDGE CASES

- [ ] No ghost users

- [ ] No duplicate users

- [ ] Channel deleted when empty

- [ ] Rejoin works cleanly


---

# 🧪 FINAL VALIDATION

- [ ] No crashes

- [ ] All numerics correct

- [ ] All ordering correct

- [ ] irssi behaves correctly

- [ ] No desync


---

# 🏁 DONE

- [ ] All tests pass

- [ ] Stable under stress

- [ ] Protocol compliant


---

END
