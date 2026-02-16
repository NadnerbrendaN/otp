cli: src/main.cpp src/chacha.cpp src/otp.cpp
	g++ src/main.cpp src/otp.cpp src/chacha.cpp -o otp

install: otp
	sudo cp ./otp /bin/

local: otp
	cp ./otp ~/.local/bin/

clean: otp
	rm otp
