#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include "OutputFacade.hpp"
#include <thread>

OutputWriter::OutputWriter(std::string serial, std::string datetime)
: serial(serial), datetime(datetime), queue(INT_MAX)
{
		std::thread writeThread(&OutputWriter::threadFunction,&queue);
		writeThread.detach();
}

void OutputWriter::threadFunction(code_machina::BlockingQueue<DeviceLog> writeQueue)
{
		while( !writeQueue.is_completed() || writeQueue.size() > 0)
		{
				std::ofstream fileOut;
				fileOut.open("output-"+this->datetime+"/"+this->serial+".txt");
				DeviceLog log;
				if(writeQueue.take(log) == code_machina::BlockingCollectionStatus::Ok)
				{
						fileOut << "Reboot count: " << log.getReboots() << std::endl;
						fileOut << "Successful BP tests: " << log.getSuccessfulBPs() << std::endl;
						fileOut << "Successful temperature tests: " << log.getSuccessfulTemps() << std::endl;
				}
				fileOut.close();
		}
}

void OutputWriter::closeThread() { this->queue.complete_adding(); }
void OutputWriter::writeLog(DeviceLog log) { this->queue.add(log); }

OutputWriter::~OutputWriter()
{
		this->closeThread();
		delete this;
}
