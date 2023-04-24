#include "Device.hpp"
#include "DeviceLog.hpp"
#include "GpioFacade.hpp"
#include "OutputFacade.hpp"
#include "BlockingCollection.h"
#include <ctime>
#include <thread>
#include <bits/stdc++.h>
#include <experimental/filesystem>
#include <algorithm>
#include <fstream>
#include <list>
#include <utility>

const std::string VERSION = "2.0.0";
int iterationCount = -1;
const int BP_CYCLES_PER_ITERATION = 3;
const int TEMP_CYCLES_PER_ITERATION = 2;

code_machina::BlockingQueue<std::string> loggingQueue(INT_MAX);

std::list<Device> deviceList{};
std::set<int> remainingGpioPins {GpioFacade::getPinSet()};
std::map<Device,std::pair<DeviceLog,OutputWriter>> deviceMap {};

time_t currentUnixTimestamp = time(0);
tm *localTime = localtime(&currentUnixTimestamp);
char datetime[30] {};

void setIterationCount();
void logWriter(code_machina::BlockingQueue<std::string>);
void singleDeviceIterations(Device, std::pair<DeviceLog, OutputWriter>, int);

int main()
{
		sprintf(datetime, "%.4i-%.2i-%.2i_%.2i.%.2i.%.2i", 
						(1900 + localTime->tm_year),
						(1+localTime->tm_mon),
						(localTime->tm_mday),
						(1+localTime->tm_hour),
						(localTime->tm_min),
						(localTime->tm_sec));
		try
		{	
				std::thread loggingThread(logWriter,loggingQueue);
				loggingThread.detach();
				loggingQueue.add("=====================");
				loggingQueue.add("Seymour Life v. "+VERSION);
				loggingQueue.add("=====================");
				const std::experimental::filesystem::path devDirectory{"/dev"};
				for(const auto& file : std::experimental::filesystem::directory_iterator{devDirectory})
				{
						if(file.path().string().find("ttyUSB") == std::string::npos) continue;
						std::string serialPath = file.path().string();
						loggingQueue.add("Opening serial port " +serialPath);
						deviceList.push_front(Device (serialPath));
				}

				for(Device device : deviceList) 
				{
						device.darkenScreen();
						DeviceLog log{};
						OutputWriter writer{device.getSerial(),datetime};
						std::pair<DeviceLog,OutputWriter> logAndWriter{log,writer};
						std::pair<Device,std::pair<DeviceLog,OutputWriter>> completeDevice {device, logAndWriter};
						deviceMap.insert(completeDevice);
				}

				for(Device device : deviceList)
				{
						device.brightenScreen();
						std::cout << "Enter the serial of the device with the bright screen: ";
						std::string deviceSerial;
						std::cin >> deviceSerial;
						device.setSerial(deviceSerial);
						device.darkenScreen();
						GpioFacade gpio{};
						for(int pin : remainingGpioPins)
						{
								gpio.relayHigh(pin);
								sleep(5);
								if(device.isTempRunning())
								{
										std::string logMessage;
										logMessage.append("Serial ").append(deviceSerial)
												  .append(" attached to relay ").append(std::to_string(pin));
										device.passGPIO(gpio);
										device.setGPIO(pin);
								}
						}
						remainingGpioPins.erase(device.getGPIO());
				}
				setIterationCount();

				for(const auto &device : deviceList)
				{
						std::thread singleDeviceIterations(device, deviceMap.at(device), iterationCount);
						singleDeviceIterations.detach();
				}
		}
		catch(...)
		{
				loggingQueue.add("Caught error! Closing gracefully...");
				loggingQueue.complete_adding();
		}
		return 0;
}


void singleDeviceIterations(Device device, std::pair<DeviceLog,OutputWriter> logAndWriter, int iterationCount)
{
		DeviceLog &log = logAndWriter.first;
		OutputWriter &writer = logAndWriter.second;
		for(int i = 0; i < iterationCount; i++)
		{
				for(int j = 0; j < BP_CYCLES_PER_ITERATION; j++)
				{
						device.startBP();
						while(!device.isBPRunning()){}
						log.successfulBP();
				}
		}
}


void setIterationCount()
{
		iterationCount = -1;
		while(iterationCount < 0)
		{
				try
				{
						std::cout << "Input the number of iterations to complete: ";
						std::string userInput;
						std::cin >> userInput;
						int parsedInput = std::stoi(userInput);
						if(parsedInput < 0) throw std::runtime_error("");
						else iterationCount = parsedInput;
				}
				catch(...)
				{
						std::cout << "Input not a valid integer!" << std::endl;
				}
		}
		std::string logMessage;
		logMessage = "New iteration count: " + std::to_string(iterationCount);
		loggingQueue.add(logMessage);
}

void logWriter(code_machina::BlockingQueue<std::string> loggingQueue)
{
		char logName[30];
		sprintf(logName, "logs/%s.log", datetime);

		std::ofstream logFile;

		logFile.open(logName, std::ios::app);

		while(!loggingQueue.is_completed())
		{
				std::string input;
				if(loggingQueue.take(input) == code_machina::BlockingCollectionStatus::Ok)
						logFile << input << std::endl;
		}

		logFile.close();
};

