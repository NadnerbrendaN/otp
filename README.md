# otp
This program allows for the encryption and decryption of arbitrary data (the message) using an equal or
greater quantity of data (the key). It also has features that use a short seed instead of a key, which
trade some amount of security for major gains in ease of use.

The [one-time pad](https://en.wikipedia.org/wiki/One-time_pad) (OTP) encryption scheme used is very simple,
and it has also been mathematically proven to be unbreakable without access to the key used for encryption,
as long as it meets certain conditions (enumerated in the linked Wikipedia article).

Whether or not *otp* truly supplies the theoretical security of the OTP encryption scheme is untested, but
realistically it should do so as long as an attacker only has access to the cyphertext. In the end,
**the security of the system is left in the capable hands of the user.**

### Structure
All source files are in the `src/` directory.

The file `main.cpp` makes a simple command line interface for the actual program. The primary files are
`otp.cpp`, containing the logic and code for encryption, and `chacha.cpp`, which contains an algorithm
needed to implement seeded random use.

Everything inside the `src/gui/` subdirectory is related to the GTK GUI, which requires gtkmm-4.0 and supporting
libraries to build. These are only required for the GUI, while the CLI has no external dependencies.

When used with the seed flag, the key is read as a seed for a
[ChaCha](https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant)-style
[CSPRNG](https://en.wikipedia.org/wiki/CSPRNG). This reduces the amount of communicated data necessary to encrypt
the message, as the only data necessary is a 320-bit (40 byte) seed
from which the true key data is generated. As mentioned earlier, this does reduce security, but *otp* is much
easier to use when the key can be a short passphrase or data sequence instead of a massive pile of data
larger than the total length of the messages one wants to send.

### Compatibility
I am unsure if *otp* will work on anything other than Linux with some GNU utilities, as that's what I run,
***but*** I do not know enough about Windows/Unix/etc to rule out compatibility. Some windows binaries are
included for releases, but that does not imply they are 100% identical in functionality to Linux releases.
No matter the OS or any other context, no warranty is provided for this program. Read more in the `LICENSE`
file.

### Planned features:
- [x] Read, encrypt, and write byte-by-byte with precommunicated keys
- [x] Seeded encryption using the ChaCha cipher for much-less-secure but much-more-convenient communication
- [x] GUI
- [ ] TUI
- [ ] GEN mode to make random keys and seeds
- [ ] Support for other block sizes or processes?

### To install:
The CLI is currently the primary easily-buildable form of *otp*, as it does not have external dependencies.
The GUI (built with GTK) requires `gtkmm-4.0` and there is Makefile support for it, but it should be treated
as experimental.

There are `Makefile`s, so you can run `make` in the `src/` directory if you have `make` and `gcc`/`g++`
installed. This should build the executable form from the source code form. `make install` will copy the
binary to `/bin/` and `make local` will copy the binary to `~/.local/bin/`. If these do not work for you or
you do not have `make`, follow the instructions below.

*otp* is primarily a few files of C++ code which only rely on each other and on the standard library, so it
should be straightforward to compile once you acquire the code. This applies to the CLI only. As mentioned
above, the GUI is more complex.

Here is how I compile it using GCC (but of course you can do it however you want):
```shell
g++ src/main.cpp src/otp.cpp src/chacha.cpp -o otp
```
After that, it's just a matter of running the executable (or moving it to a directory for program binaries and
then running it).

### To run:
The GUI should be self-explanatory, and the CLI tries to be as well. For CLI:

Call the executable form without arguments for built-in help that is updated every time a feature is.
Calling the executable with a mode as the first argument and nothing else will give help for that mode.

Running an improper command will, in general, not cause any harm such as deleting data, so it is safe
to try things out and run incomplete commands to learn one's way around the system by using it.
