#include "DeviceLog.hpp"

DeviceLog::DeviceLog(int reboots, int bps, int temps)
{
		this->reboots = reboots;
		this->bps = bps;
		this-> temps = temps;
};

int DeviceLog::getReboots() { return this->reboots; };
int DeviceLog::getSuccessfulBPs() { return this->bps; };
int DeviceLog::getSuccessfulTemps() { return this->temps; };
