otp: main.cpp chacha.hpp
	g++ main.cpp -o otp

install: otp
	sudo cp ./otp /bin/

local: otp
	sudo cp ./otp ~/.local/bin/

clean:
	rm otp
