#include <string>
#include <termios.h>
#ifndef DEVICE_H_
#define DEVICE_H_

class Device
{
		private:
				const std::string serial;
				const int gpioPort;
				struct termios tty;
		public:
				Device(std::string , int, std::string);
				struct termios getTTY();
				int getGPIO();
				std::string getSerial() const;
};
#endif
