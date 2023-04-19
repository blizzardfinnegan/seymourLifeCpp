#include "Device.hpp"
#ifndef DEVICELOG_H_
#define DEVICELOG_H_

class DeviceLog
{
		private:
				const Device device;
				const int reboots;
				const int bps;
				const int temps;
		public:
				DeviceLog(Device,int=0,int=0,int=0);
				int getReboots();
				std::string getSerial();
				int getSuccessfulBPs();
				int getSuccessfulTemps();
};
#endif
