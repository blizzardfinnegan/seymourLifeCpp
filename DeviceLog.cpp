#include "DeviceLog.hpp"

DeviceLog::DeviceLog(std::string serial, int reboots, int bps, int temps)
{
		this->serial = serial;
		this->reboots = reboots;
		this->bps = bps;
		this-> temps = temps;
};

int DeviceLog::getReboots() { return this->reboots; };
int DeviceLog::getSuccessfulBPs() { return this->bps; };
int DeviceLog::getSuccessfulTemps() { return this->temps; };
std::string DeviceLog::getSerial() { return this->serial; };
