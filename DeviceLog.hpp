#include "Device.hpp"
#ifndef DEVICELOG_H_
#define DEVICELOG_H_

class DeviceLog
{
		private:
				int reboots;
				int bps;
				int temps;
		public:
				DeviceLog(int=0,int=0,int=0);
				int getReboots();
				int getSuccessfulBPs();
				int getSuccessfulTemps();
};
#endif
