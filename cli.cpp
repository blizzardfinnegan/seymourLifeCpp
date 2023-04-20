#include "Device.hpp"
#include "DeviceLog.hpp"
#include "GpioFacade.hpp"
#include "OutputFacade.hpp"
#include "BlockingCollection.h"
#include <ctime>
#include <thread>
#include <bits/stdc++.h>

void logWriter(code_machina::BlockingQueue<std::string> loggingQueue)
{
		time_t currentUnixTimestamp = time(0);
		tm *localTime = localtime(&currentUnixTimestamp);
		char logName[30];
		sprintf(logName, "logs/%.4i-%.2i-%.2i_%.2i.%.2i.%.2i.log", 
						(1900 + localTime->tm_year),
						(1+localTime->tm_mon),
						(localTime->tm_mday),
						(1+localTime->tm_hour),
						(localTime->tm_min),
						(localTime->tm_sec));

		std::ofstream logFile;
		logFile.open(logName, std::ios::app);
		while(!loggingQueue.is_completed() || loggingQueue.size() > 0)
		{
				std::string input;
				if(loggingQueue.take(input) == code_machina::BlockingCollectionStatus::Ok)
						logFile << input << std::endl;
		}
		logFile.close();
};



int main()
{
		code_machina::BlockingQueue<std::string> loggingQueue(INT_MAX);
		std::thread loggingThread(logWriter,loggingQueue);
		loggingThread.detach();

		int iterationCount = -1;
		while(iterationCount < 0)
		{
				try
				{
						std::cout << "Input the number of iterations to complete: ";
						std::string userInput;
						std::cin >> userInput;
						int parsedInput = std::stoi(userInput);
				}
				catch(...)
				{
						std::cout << "Input not a valid integer!" << std::endl;
				}
		}
		return 0;
}
