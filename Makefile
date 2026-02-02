otp: main.cpp chacha.cpp otp.cpp
	g++ main.cpp -o otp

install: otp
	sudo cp ./otp /bin/

local: otp
	sudo cp ./otp ~/.local/bin/

clean:
	rm otp
