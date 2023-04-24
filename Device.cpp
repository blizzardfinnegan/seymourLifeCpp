#include "Device.hpp"

Device::Device(std::string ttyFile) : serial(""), gpioPort(-1), 
		state(State::LOGIN_PROMPT), terminalPort(open(ttyFile.c_str(),O_RDWR)) 
{
		//create variable containing serial settings
		if(tcgetattr(Device::terminalPort, &tty) != 0)
				std::cerr << "Cannot open serial connection to " << ttyFile << std::endl;
		else
		{
				//Disable parity, use one stop bit, clear number of bits per byte, disable hardware flow control
				tty.c_cflag &= ~(PARENB | CSTOPB | CSIZE | CRTSCTS); 
				//set 8 bits per byte, allow read, ignore modem-specific signals
				tty.c_cflag |= CS8 | CREAD | CLOCAL; 

				// Force inputs to be processed character by character, mute echoes, 
				tty.c_lflag &= ~ICANON; //Force inputs to be processed character-by-character
				//The next line stops the device on the other end from sending back exactly what we sent it.
				tty.c_lflag &= ~(ECHO | ECHOE | ECHONL);

				tty.c_lflag &= ~ISIG; //Don't interpret the INTR, QUIT, and SUSP characters

				tty.c_iflag &= ~(IXON | IXOFF | IXANY); //Turn off software flow control
				tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
				// Prevent special interpretation of output bytes; prevent conversion of newline to CR, LF, or CRLF characters
				tty.c_oflag &= ~(OPOST | ONLCR); 
				//Wait for up to 25.5 seconds [the max allowed] for data to come in.
				tty.c_cc[VTIME]=255;
				tty.c_cc[VMIN] = 0;

				//set baud rate to 115200
				cfsetispeed(&tty, B115200); cfsetospeed(&tty, B115200);

				//send settings to TTY, check for error 
				if(tcsetattr(Device::terminalPort,TCSANOW, &tty) != 0)
				{
						std::cerr << "Error " << errno << " from tcsetattr: "; //<< strerror_r(errno,&errorBuffer,sizeof(errorBuffer));
				}
		};

		
}

std::string Device::readFromDevice()
{
		int n = read(this->terminalPort,&this->msgBuffer, sizeof(this->msgBuffer));
		if(n > 0)
				return this->msgBuffer;
		else
				throw std::underflow_error("Serial connection lost!");
}

void Device::writeToDevice(Command command)
{
		write(this->terminalPort,
				CommandString.at(command).c_str(),
				sizeof(CommandString.at(command).c_str()));
}

void Device::goToLoginPrompt()
{
		while(this->state != State::LOGIN_PROMPT)
		{
				switch (this->state)
				{
						case State::LOGIN_PROMPT:
								return;
						case State::DEBUG_MENU:
						case State::BRIGHTNESS_MENU:
						case State::LIFECYCLE_MENU:
								this->writeToDevice(Command::QUIT);
								this->state = State::LOGIN_PROMPT;
								break;
				}
		};
}

void Device::goToDebugMenu()
{
		while(this->state != State::DEBUG_MENU)
		{
				switch (this->state)
				{
						case State::DEBUG_MENU:
								return;
						case State::LIFECYCLE_MENU:
								this->writeToDevice(Command::UP_MENU_LEVEL);
								this->state = State::DEBUG_MENU;
								return;
						case State::BRIGHTNESS_MENU:
								this->writeToDevice(Command::UP_MENU_LEVEL);
								this->state = State::LIFECYCLE_MENU;
								break;
						case State::LOGIN_PROMPT:
								this->writeToDevice(Command::LOGIN);
								this->state = State::DEBUG_MENU;
								return;
				}
		};
}

void Device::goToBrightnessMenu()
{
		while(this->state != State::BRIGHTNESS_MENU)
		{
				switch (this->state)
				{
						case State::BRIGHTNESS_MENU:
								return;
						case State::DEBUG_MENU:
								this->writeToDevice(Command::ENTER_LIFECYCLE_MENU);
								this->state = State::LIFECYCLE_MENU;
								break;
						case State::LIFECYCLE_MENU:
								this->writeToDevice(Command::ENTER_BRIGHTNESS_MENU);
								this->state = State::BRIGHTNESS_MENU;
								return;
						case State::LOGIN_PROMPT:
								this->writeToDevice(Command::LOGIN);
								this->state = State::DEBUG_MENU;
								break;
				}
		};
}

void Device::setSerial(std::string serial) { this->serial = serial; }
void Device::passGPIO(GpioFacade gpio) { this->gpio = gpio; }
bool Device::setGPIO(int pinAddress) 
{ 
		if(this->gpio.isValidAddress(pinAddress)) 
		{
				this->gpioPort = pinAddress; 
				return true;
		}
		else return false;
}

int Device::getGPIO() { return this->gpioPort; }
std::string Device::getSerial() { return this->serial; }

bool Device::startTemp()
{
		if (this->gpio.isValidAddress(this->gpioPort))
		{
				this->gpio.relayHigh(this->gpioPort);
				return true;
		}
		else return false;

}

bool Device::stopTemp()
{
		if (this->gpio.isValidAddress(this->gpioPort))
		{
				this->gpio.relayLow(this->gpioPort);
				return true;
		}
		else return false;

}

void Device::darkenScreen()
{
		this->goToBrightnessMenu();
		this->writeToDevice(Command::BRIGHTNESS_LOW);
}

void Device::brightenScreen()
{
		this->goToBrightnessMenu();
		this->writeToDevice(Command::BRIGHTNESS_LOW);
}

bool Device::isTempRunning()
{
		this->goToLifecycleMenu();
		this->writeToDevice(Command::READ_TEMP);
		std::string response = this->readFromDevice();
		size_t tempIndex = response.find(TemplateResponseStrings.at(Response::TEMPERATURE));
		if( tempIndex == std::string::npos)
		{
				throw std::runtime_error("Response string does not contain temperature measurement!");
		}
		else
		{ 		std::string substring = response.substr((tempIndex + 
											  (TemplateResponseStrings.at(Response::TEMPERATURE).size())));
				return std::stoi(substring) != 0;
		}
}

void Device::startBP()
{
		this->goToLifecycleMenu();
		this->writeToDevice(Command::START_BP);
}

bool Device::isBPRunning()
{
		this->goToLifecycleMenu();
		this->writeToDevice(Command::CHECK_BP_STATE);
		std::string response = this->readFromDevice();
		size_t bpOnIndex = response.find(TemplateResponseStrings.at(Response::BP_ON));
		size_t bpOffIndex = response.find(TemplateResponseStrings.at(Response::BP_OFF));
		if( bpOnIndex == std::string::npos && bpOffIndex == std::string::npos)
		{
				throw std::runtime_error("Response string does not contain BP information!");
		}
		else
		{
				return bpOnIndex != std::string::npos;
		};
}

void Device::reboot() 
{ 
		if (this->state != State::LOGIN_PROMPT)
		{
				this->goToLoginPrompt();
		}
		else
		{
				this->goToDebugMenu();
				this->goToLoginPrompt();
		}
}

Device::~Device()
{
		close(this->terminalPort);
		delete this;
};
