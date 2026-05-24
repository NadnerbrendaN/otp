otp: src/*.cpp
	g++ src/*.cpp -o otp

otp-gui: src/otp.cpp src/chacha.cpp src/gui/*.cpp
	g++ src/gui/*.cpp src/otp.cpp src/chacha.cpp -o otp-gui $(shell pkg-config --cflags --libs gtkmm-4.0)

install:
	if [ -r ./otp ]; then install ./otp /usr/local/bin/; fi
	if [ -r ./otp-gui ]; then install ./otp-gui /usr/local/bin/otp-gui; install src/gui/otp-gui-global.desktop /usr/share/applications/otp-gui.desktop; fi

local:
	if [ -r ./otp ]; then install ./otp ~/.local/bin/; fi
	if [ -r ./otp-gui ]; then install ./otp-gui ~/.local/bin/otp-gui; fi

clean:
	if [ -r ./otp ]; then rm otp; fi
	if [ -r ./otp-gui ]; then rm otp-gui; fi
