#include "Device.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <fstream>
#include <iostream>

Device::Device(std::string serial, int gpioPort, std::string ttyFile) :
		serial(serial) , gpioPort(gpioPort)
{
		int port = open(ttyFile.c_str(), O_RDWR);
		if(tcgetattr(port, &tty) != 0)
				std::cerr << "Cannot open serial connection to " << ttyFile << std::endl;
		else
		{
				tty.c_cflag &= ~PARENB; //Disable parity (more common).
				tty.c_cflag &= ~CSTOPB; //Use one stop bit in communication (more common).
				tty.c_cflag &= ~CSIZE; tty.c_cflag |= CS8; //set 8 bits per byte in serial communication
				tty.c_cflag &= ~CRTSCTS; //disable hardware flow control.
				tty.c_cflag |= CREAD | CLOCAL; //Allow us to read, ignore modem-specific signal lines
				tty.c_lflag &= ~ICANON; //Force inputs to be processed character-by-character
				//The next line stops the device on the other end from sending back exactly what we sent it.
				tty.c_lflag &= ~(ECHO | ECHOE | ECHONL);

				tty.c_lflag &= ~ISIG; //Don't interpret the INTR, QUIT, and SUSP characters

				tty.c_iflag &= ~(IXON | IXOFF | IXANY); //Turn off software flow control
				tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

		};
};


