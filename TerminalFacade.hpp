#include <map>
#include <termios.h>
#include "Device.hpp"
#ifndef TERMINALFACADE_H_
#define TERMINALFACADE_H_

enum State
{ LOGIN_PROMPT, DEBUG_MENU, BP_MENU, LIFECYCLE_MENU, BRIGHTNESS_MENU };

enum Command {QUIT, ENTER_BP_MENU, START_BP, CANCEL_BP, CHECK_BP_STATE, ENTER_LIFECYCLE_MENU, ENTER_BRIGHTNESS_MENU, BRIGHTNESS_HIGH, BRIGHTNESS_LOW, READ_TEMP, UP_MENU_LEVEL, REDRAW_MENU, LOGIN, NEWLINE};

const std::map<Command,std::string> CommandString = 
		{
				{ QUIT, "q\n" },
				{ENTER_BP_MENU,"n"},
				{START_BP,"s"},
				{CANCEL_BP,"c"},
				{CHECK_BP_STATE,"C"},
				{ENTER_LIFECYCLE_MENU, "L"},
				{ENTER_BRIGHTNESS_MENU, "B"},
				{BRIGHTNESS_LOW, "1"},
				{BRIGHTNESS_HIGH, "0"},
				{READ_TEMP, "h"},
				{UP_MENU_LEVEL,"\\"},
				{REDRAW_MENU,"?"},
				{LOGIN,"root\npython3 -m debugmenu; shutdown -r now"},
				{NEWLINE,"\n"}
		};

class TerminalFacade
{
		private:
				Device device;
				struct termios tty;
				State state;
				void goToLoginPrompt();
				void goToBPMenu();
				void goToDebugMenu();
				void goToLifecycleMenu();
				void goToBrightnessMenu();
		public:
				TerminalFacade(Device);
				void darkenScreen();
				void brightenScreen();
				bool isTempRunning();
				bool isBPRunning();
				void startBP();
				void reboot();
};

#endif
