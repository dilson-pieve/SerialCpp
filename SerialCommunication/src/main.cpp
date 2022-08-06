#include <iostream>
#include <thread>

#include "../include/Serial.hpp"

using namespace std::chrono_literals;

void writing(bool write, light::serial& serial)
{
	const std::string text = "Hello, World";

	if (write && serial.connected())
	{
		auto msg = serial.write(text);
		if (msg != light::message::successful_write)
			std::cout << "text=" << text << " not sent" << '\n';

		std::this_thread::sleep_for(1000ms);
	}
}

void reading(bool read, light::serial& serial)
{
	if (read && serial.connected())
	{
		auto msg = serial.read();
		std::cout << (msg.empty() ? "empty" : msg) << '\n';
	}
}

int main()
{
	light::serial serial("COM5");
	if (serial.begin(9600) != light::message::successful_begin)
		std::cout << "serial didn't begin" << '\n';
	else {
		bool running = true;

		while (running)
		{
			bool read = true; // false to write

			reading(read, serial);
			writing(!read, serial);

			std::this_thread::sleep_for(1000ms);
		}
	}

	return 0;
}