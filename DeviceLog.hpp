#include "Device.hpp"
#ifndef DEVICELOG_H_
#define DEVICELOG_H_

class DeviceLog
{
		private:
				std::string serial;
				int reboots;
				int bps;
				int temps;
		public:
				DeviceLog(std::string="",int=0,int=0,int=0);
				int getReboots();
				std::string getSerial();
				int getSuccessfulBPs();
				int getSuccessfulTemps();
};
#endif
