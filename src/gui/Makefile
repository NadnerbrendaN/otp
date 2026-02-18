gui:
	g++ gui-main.cpp window.cpp ../otp.cpp ../chacha.cpp -o otp-gui $(shell pkg-config --cflags --libs gtkmm-4.0)

install:
	sudo cp ./otp-gui /bin/otp-gui
	sudo cp ./otp-gui-global.desktop /usr/share/applications/otp-gui.desktop

local:
	cp ./otp-gui ~/.local/bin/otp-gui
	cp ./otp-gui-local.desktop ~/.local/share/applications/otp-gui.desktop

clean:
	rm ./otp-gui
