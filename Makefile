otp: src/*.cpp
	g++ src/*.cpp -o otp

otp-gui: src/otp.cpp src/chacha.cpp src/gui/*.cpp
	g++ src/gui/*.cpp src/otp.cpp src/chacha.cpp -o otp-gui $(shell pkg-config --cflags --libs gtkmm-4.0)

install:
	cp ./otp /bin/
	cp ./otp-gui /bin/otp-gui
	cp src/gui/otp-gui-global.desktop /usr/share/applications/otp-gui.desktop

local:
	cp ./otp ~/.local/bin/
	cp ./otp-gui ~/.local/bin/otp-gui

clean:
	rm otp
	rm otp-gui
