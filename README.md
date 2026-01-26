# otp
### A one-time pad encryption and decryption system
This program allows for the provably secure encryption and decryption of arbitrary data (the message) using
an equal or greater quantity of data (the key). The file `main.cpp` is the main code file and `chacha.hpp`
contains an algorithm needed to implement seeded random use. Whether or not it truly supplies the theoretical
security of the OTP encryption scheme is untested, but realistically it should do so as long as an attacker
only has access to the cyphertext. An attacker with access to your computer could likely read the message by
reading the program's memory, but realistically they could do that anyway by reading the file it's coming from.

The [one-time pad](https://en.wikipedia.org/wiki/One-time_pad) (OTP) encryption scheme used is very simple,
and it has also been mathematically proven to be unbreakable without access to the key used for encryption,
given the following conditions:

1. The key is communicated secretly before any messages are encrypted and it is never shared with others
2. The key is as long as or longer than the message
3. No part of the key is ever reused
4. The data used for the key are truly random

If the above rules are followed, the cyphertext should be impossible to convert back to the original readable
plaintext with any certainty. Follow those rules when using this program.

The program cannot enforce #1, #3, and #4. It does require #2 in order to function, and the delete flag may
assist with #3. In the end, the security of the system is left in the capable hands of the user. The program
also offers a method to violate these rules in exchange for increased convenience by using the output of a 
non-standard [ChaCha](https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant)-style
[CSPRNG](https://en.wikipedia.org/wiki/CSPRNG) instead of user-supplied random data. This reduces the amount of
user-supplied data necessary to encrypt the message, as the only data necessary is a 48-character seed from
which to generate the true key data.

### Compatibility
I am unsure if the program will work on anything other than Linux with some GNU utilities, as that's what I
run, ***but*** I don't know enough about Windows/Unix/etc to rule out compatibility. No matter the OS or any
other context, no warranty or guarantees are provided for this program, as detailed in the LICENSE file.

### Planned features:
- [x] Read, encrypt, and write byte-by-byte with precommunicated keys
- [x] Seeded encryption using the ChaCha cipher for much-less-secure but much-more-convenient communication
- [ ] Support for other block sizes or processes?
- [ ] UI?

### To install:
It is a few files of C++ code which only rely on each other and on the standard library, so it should be
straightforward to compile once you acquire the code.
Here is how I compile it using GCC (but of course you can do it however you want):
```shell
g++ main.cpp -o otp
```
After that, it's just a matter of running the executable (or moving it to a directory for program binaries, and
then running it).

### To run:
Call the executable form without arguments for built-in help that is updated every time a feature is.
Calling the executable with a mode as the first argument and nothing else will give help for that mode.
#### Simple overview:
`otp byte` enters byte mode, where bytes are taken from the message, encrypted with bytes from the key, and
written to the cyphertext. Specify the key with `-k (path/to/key_file)`, the input (message or cypher) with
`-m (path/to/message_file)`, and output with `-o (path/to/output_file)`.
Using the -e option specifies encryption as opposed to decryption. The -d option deletes used key data, and -s
specifies that the key is to be used as a seed. Using the -s option will override -d, turning it off, due to
the fact that deletion of data would be ineffective and simply lose the user's seed.
