#include <fstream>
#include <iostream>
#include "OutputFacade.hpp"

OutputWriter::OutputWriter(std::string serial, std::string datetime, 
						code_machina::BlockingQueue<DeviceLog> writeQueue)
{
		this->fileOut.open("output-"+datetime+"/"+serial+".txt");
}

void threadFunction(code_machina::BlockingQueue<DeviceLog> writeQueue)
{
		while( !writeQueue.is_completed() || writeQueue.size() > 0)
		{
				DeviceLog input;
				if(writeQueue.take(input) == code_machina::BlockingCollectionStatus::Ok)
				{

				}
		}
}

//void OutputFacade::write(DeviceLog log, std::string datetime)
//{
//		std::ofstream fileOut;
//		fileOut.open("output-"+datetime+"/"+log.getSerial()+".txt");
//		fileOut << "Serial: " << log.getSerial() << std::endl;
//		fileOut << "Reboot count: " << log.getReboots() << std::endl ;
//		fileOut << "Successful BP tests: " << log.getSuccessfulBPs() << std::endl;
//		fileOut << "Successful temperature tests: " << log.getSuccessfulTemps() << std::endl;
//		fileOut.close();
//};
