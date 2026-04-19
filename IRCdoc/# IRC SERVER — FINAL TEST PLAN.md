# 🧭 HOW TO USE

Each item follows:

- **Action** → what you do

- **Expected** → what should happen

- **Verify** → what to check concretely


Mark ✔ only if all 3 are satisfied.

---

# ⚙️ PREPARATION

- [ ] **Build server**
    - [x] → Run `make`
    - [x] → Expected: no errors
    - [x] → Verify: binary exists

- [ ] **Check port availability**
    - [ ] → Use port `6667`
    - [ ] → Expected: free
    - [ ] → Verify: server binds successfully
    - [ ] ->verify min and max ports 


---

# 🚀 LIFECYCLE

- [ ] **Start server**
    - [ ] → Launch binary
    - [ ] → Expected: listening
    - [ ] → Verify: can connect via `nc`

- [ ] **Graceful shutdown**
    - [ ] → Send `SIGINT`
    - [ ] → Expected: clean exit
    - [ ] → Verify: port freed

- [ ] **Restart immediately**
    - [ ] → Restart server
    - [ ] → Expected: no bind error
    - [ ] → Verify: connects again


---

# 🔌 CONNECTION & REGISTRATION

- [ ] **Valid registration**
    - [ ] → Send `PASS`, `NICK`, `USER`
    - [ ] → Expected: `001–004`
    - [ ] → Verify: correct order + format

- [ ] **Missing USER/NICK**
    - [ ] → Omit one
    - [ ] → Expected: no welcome
    - [ ] → Verify: no `001`

- [ ] **Duplicate nick**
    - [ ] → Two clients same nick
    - [ ] → Expected: `433`
    - [ ] → Verify: second rejected


---

# 🚪 JOIN

- [ ] **Create channel**
    - [ ] → `/join #chan`
    - [ ] → Expected: JOIN - [ ] → 331/332 - [ ] → 353 - [ ] → 366
    - [ ] → Verify: correct order

- [ ] **Join existing channel**
    - [ ] → 2 clients join
    - [ ] → Expected: broadcast JOIN
    - [ ] → Verify: both see each other

- [ ] **Invalid channel**
    - [ ] → `JOIN chan`
    - [ ] → Expected: `403`
    - [ ] → Verify: no channel created

- [ ] **Invite-only (+i)**
    - [ ] → Enable +i
    - [ ] → Expected: join blocked
    - [ ] → Verify: `473`

- [ ] **Key (+k)**
    - [ ] → Wrong key
    - [ ] → Expected: reject
    - [ ] → Verify: `475`

- [ ] **Limit (+l)**
    - [ ] → Exceed limit
    - [ ] → Expected: reject
    - [ ] → Verify: `471`


---

# 💬 PRIVMSG

- [ ] **User message**
    - [ ] → `PRIVMSG nick :msg`
    - [ ] → Expected: received
    - [ ] → Verify: correct prefix

- [ ] **Channel message**
    - [ ] → `PRIVMSG #chan :msg`
    - [ ] → Expected: broadcast
    - [ ] → Verify: all members receive

- [ ] **Missing target**
    - [ ] → `PRIVMSG`
    - [ ] → Expected: `411`

- [ ] **Missing message**
    - [ ] → `PRIVMSG nick`
    - [ ] → Expected: `412`


---

# 🏷️ TOPIC

- [ ] **Set topic**
    - [ ] → `TOPIC #chan :text`
    - [ ] → Expected: broadcast
    - [ ] → Verify: all updated

- [ ] **Get topic**
    - [ ] → `TOPIC #chan`
    - [ ] → Expected: `332` or `331`

- [ ] **Restricted (+t)**
    - [ ] → non-op sets topic
    - [ ] → Expected: `482`


---

# 🛠️ MODE

## Query

- [ ] **Get modes**
    - [ ] → `MODE #chan`
    - [ ] → Expected: `324 #chan +modes`
    - [ ] → Verify: order = `itkl`, empty = `+`


---

## Change

- [ ] **Enable mode**
    - [ ] → `MODE #chan +i`
    - [ ] → Expected: broadcast
    - [ ] → Verify: only `+i` sent

- [ ] **Disable mode**
    - [ ] → `MODE #chan -i`
    - [ ] → Expected: broadcast

- [ ] **Invalid mode**
    - [ ] → `MODE #chan +z`
    - [ ] → Expected: `472`

- [ ] **Missing param**
    - [ ] → `MODE #chan +k`
    - [ ] → Expected: `461`


---

## Mode enforcement

- [ ] **+i blocks JOIN - [ ] → `473`**

- [ ] **+k blocks JOIN - [ ] → `475`**

- [ ] **+l blocks JOIN - [ ] → `471`**

- [ ] **+t blocks TOPIC - [ ] → `482`**


---

# 👑 OPERATORS (+o)

- [ ] **Grant op**
    - [ ] → `MODE #chan +o nick`
    - [ ] → Expected: `@nick` in NAMES

- [ ] **Remove op**
    - [ ] → `MODE #chan -o nick`
    - [ ] → Expected: removed

- [ ] **Non-op action**
    - [ ] → try KICK/MODE
    - [ ] → Expected: `482`


---

# 🥾 KICK

- [ ] **Kick user**
    - [ ] → `KICK #chan nick`
    - [ ] → Expected: removed + broadcast

- [ ] **Invalid target**
    - [ ] → Expected: `441`


---

# ✉️ INVITE

- [ ] **Invite user**
    - [ ] → `INVITE nick #chan`
    - [ ] → Expected: target notified + `341`

- [ ] **Join after invite**
    - [ ] → Expected: succeeds


---

# 📡 NAMES

- [ ] **Names list**
    - [ ] → Expected: `353` + `366`

- [ ] **Operator prefix**
    - [ ] → Expected: `@nick`


---

# 💣 NC BREAK TESTS

## Setup

`nc localhost 6667`

---

- [ ] **Random input**
    - [ ] → `HELLO`
    - [ ] → Expected: no crash

- [ ] **Missing CRLF**
    - [ ] → partial send
    - [ ] → Expected: no crash

- [ ] **Invalid JOIN**
    - [ ] → `JOIN`
    - [ ] → Expected: `461`

- [ ] **Broken PRIVMSG**
    - [ ] → `PRIVMSG nick`
    - [ ] → Expected: `412`

- [ ] **Broken MODE**
    - [ ] → `MODE #chan i`
    - [ ] → Expected: reject

- [ ] **Invalid limit**
    - [ ] → `MODE #chan +l -5`
    - [ ] → Expected: reject

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
