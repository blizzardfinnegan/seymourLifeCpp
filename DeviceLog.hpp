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
				DeviceLog(int reboots=0,int bps=0,int temps=0) : 
						reboots(reboots), bps(bps), temps(temps) {};
				void successfulReboot() { this->reboots++; };
				void successfulBP() { this->bps++; };
				void successfulTemp() { this->temps++; };
				int getReboots() { return this->reboots; };
				int getSuccessfulBPs() { return this->bps; };
				int getSuccessfulTemps() { return this->temps; };
};
#endif
