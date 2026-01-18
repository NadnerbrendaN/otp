# otp
### A one-time pad encryption and decryption system
This program allows for the provably secure encryption and decryption of arbitrary data (the message) using
an equal or greater quantity of data (the key). The [one-time pad](https://en.wikipedia.org/wiki/One-time_pad)
(OTP) encryption scheme used is very simple, and it has also been mathematically proven to be unbreakable
without access to the key used for encryption, given the following conditions:
1. The key is communicated secretly before any messages are encrypted and it is never shared with others
2. The key is as long as or longer than the message
3. No part of the key is ever reused
4. The data used for the key are truly random

This program cannot enforce #1, #3, and #4, but it does require #2. The security of the cyphertext is left to
the user, as the user is trusted above all else.

### Planned features:
- [x] Read, encrypt, and write byte-by-byte with precommunicated keys
- [ ] Seeded random keys for much-less-secure but much-more-convenient communication
- [ ] Support for other group sizes or processes?

### To build:
It's a single file of c++ code, so just run it through a compiler. I can't guarantee it will work on anything
other than Linux with some GNU utilities, as that's what I run, but I don't know enough about
Windows/Unix/etc to rule it out.

One possible Unix-like example using GCC (but of course you can do it however you want):
```shell
g++ main.cpp -o otp
```

### To run:
Call the executable form without arguments for built-in help that is updated every time a feature is.
Calling the executable with a mode as the first argument and nothing else will give help for that mode.
#### Simple overview:
`otp byte` enters byte mode, where bytes are taken from the message, encrypted with bytes from the key, and
written to the cyphertext. Specify the key with `-k (path/to/key_file)`, the input (message or cypher) with
`-m (path/to/message_file)`, and output with `-o (path/to/output_file)`.
