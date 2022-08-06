#pragma once

// ref: https://www.delftstack.com/howto/cpp/cpp-serial-communication/

#include <string>
#include <Windows.h>

static constexpr short buffer_size = 255;

namespace light
{
	enum class message
	{
		successful_begin,
		failed_begin,
		invalid_handle,
		failed_get_settings,
		failed_set_settings,
		failed_timeout,
		successful_write,
		failed_write,
	};

	class serial
	{
	public:
		serial(const std::string& port_name);

		serial() = delete;
		serial(const serial&) = delete;

		~serial();

		light::message begin(int baud_rate);
		light::message write(const std::string& text);
		std::string read();
		bool connected();

	private:
		HANDLE handle_;
		bool is_connected;
	};
}

light::serial::serial(const std::string& port_name) {
	is_connected = true;

	handle_ = CreateFileA(port_name.c_str(), GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (handle_ == INVALID_HANDLE_VALUE)
		is_connected = false;
}

light::serial::~serial() {
	CloseHandle(handle_);
}

bool light::serial::connected() {
	return is_connected;
}

light::message light::serial::begin(const int baud_rate) {
	if (!connected())
		return light::message::invalid_handle;

	light::message msg = message::successful_begin;

	DCB dcbSerialParam = { 0 };

	if (!GetCommState(handle_, &dcbSerialParam))
		msg = light::message::failed_get_settings;
	else {
		dcbSerialParam.BaudRate = baud_rate;
		dcbSerialParam.ByteSize = 8;
		dcbSerialParam.StopBits = ONESTOPBIT;
		dcbSerialParam.Parity = NOPARITY;
		dcbSerialParam.fDtrControl = DTR_CONTROL_ENABLE;

		if (!SetCommState(handle_, &dcbSerialParam))
			msg = light::message::failed_set_settings;
		else {
			COMMTIMEOUTS timeout = { 0 };
			timeout.ReadIntervalTimeout = 60;
			timeout.ReadTotalTimeoutConstant = 60;
			timeout.ReadTotalTimeoutMultiplier = 15;
			timeout.WriteTotalTimeoutConstant = 60;
			timeout.WriteTotalTimeoutMultiplier = 8;

			if (!SetCommTimeouts(handle_, &timeout))
				msg = light::message::failed_timeout;
		}
	}

	is_connected = true;
	return msg;
}

light::message light::serial::write(const std::string& text) {
	if (!connected())
		return light::message::failed_write;

	DWORD bytes_send;

	if (!WriteFile(handle_, (void*)text.c_str(), text.length(), &bytes_send, 0))
	{
		ClearCommError(handle_, NULL, NULL);
		return light::message::failed_write;
	}

	return light::message::successful_write;
}

std::string light::serial::read() {
	if (!connected())
		return "";

	DWORD read = 0;
	char buffer[buffer_size + 1] = { 0 };

	if (!ReadFile(handle_, (void*)buffer, buffer_size, &read, NULL))
		return "";

	return static_cast<std::string>(buffer);
}