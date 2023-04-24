#include "GpioFacade.hpp"
#include <map>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <termios.h>
#include <stdexcept>
#ifndef DEVICE_H_
#define DEVICE_H_

const int errorBufferSize = 256;
const int msgBufferSize = 4096;
enum State
{ 
		LOGIN_PROMPT, DEBUG_MENU, LIFECYCLE_MENU, BRIGHTNESS_MENU 
};

enum Command 
{
		QUIT, START_BP, CHECK_BP_STATE, ENTER_LIFECYCLE_MENU, ENTER_BRIGHTNESS_MENU, 
		BRIGHTNESS_HIGH, BRIGHTNESS_LOW, READ_TEMP, UP_MENU_LEVEL, REDRAW_MENU, 
		LOGIN, NEWLINE
};

enum Response
{
		PASSWORD_PROMPT, SHELL_PROMPT, BP_ON, BP_OFF, TEMPERATURE, USERNAME_PROMPT, DEBUG_MENU_PROMPT, DEBUG_MENU_EXCEPTION, DEBUG_MENU_ERROR_THREE, OTHER
};

const std::map<Command,std::string> CommandString = 
{
		{ QUIT, "q\n" },
		{START_BP,"c"},
		{CHECK_BP_STATE,"C"},
		{ENTER_LIFECYCLE_MENU, "L"},
		{ENTER_BRIGHTNESS_MENU, "B"},
		{BRIGHTNESS_LOW, "1"},
		{BRIGHTNESS_HIGH, "0"},
		{READ_TEMP, "h"},
		{UP_MENU_LEVEL,"\\"},
		{REDRAW_MENU,"?"},
		{LOGIN,"root\npython3 -m debugmenu; shutdown -r now\n"},
		{NEWLINE,"\n"}
};

const std::map<Response,std::string> TemplateResponseStrings = 
{
		{PASSWORD_PROMPT, "Password:"},
		{SHELL_PROMPT, "root@"},
		{BP_ON, "MANUAL_BP"},
		{BP_OFF, "IDLE"},
		{TEMPERATURE, "Temp:"},
		{USERNAME_PROMPT,"login:"},
		{DEBUG_MENU_PROMPT,">"},
		{DEBUG_MENU_EXCEPTION,"Traceback (most recent call last)"},
		{DEBUG_MENU_ERROR_THREE, "Error number -3"},
		{OTHER, ""}
};


class Device
{
		private:
				std::string serial;
				int gpioPort;
				GpioFacade gpio;
				const int terminalPort;
				struct termios tty;
				State state;
				char errorBuffer[errorBufferSize];
				char msgBuffer[msgBufferSize];

				void outputThread();
				std::string readFromDevice();
				void writeToDevice(Command);
				void goToLoginPrompt();
				void goToDebugMenu();
				void goToLifecycleMenu();
				void goToBrightnessMenu();
		public:
				Device(std::string);
				void setSerial(std::string);
				void passGPIO(GpioFacade);
				bool setGPIO(int);
				bool startTemp();
				bool stopTemp();
				int getGPIO();
				std::string getSerial();
				void darkenScreen();
				void brightenScreen();
				bool isTempRunning();
				bool isBPRunning();
				void startBP();
				void cancelBP();
				void reboot();
				~Device();
};
#endif
