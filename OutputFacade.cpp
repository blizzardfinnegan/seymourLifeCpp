#include <fstream>
#include <iostream>
#include "OutputFacade.hpp"

void OutputFacade::write(DeviceLog log)
{
		std::ofstream fileOut;
		fileOut.open(log.getSerial()+".txt");
		fileOut << "Serial: " << log.getSerial() << std::endl;
		fileOut << "Reboot count: " << log.getReboots() << std::endl ;
		fileOut << "Successful BP tests: " << log.getSuccessfulBPs() << std::endl;
		fileOut << "Successful temperature tests: " << log.getSuccessfulTemps() << std::endl;
};
