#include "DeviceLog.hpp"
#include <fstream>
#include <queue>
#ifndef OUTPUTFACADE_H_
#define OUTPUTFACADE_H_

class OutputWriter
{
		private:
				std::string serial;
				std::string datetime;
				std::queue<DeviceLog> queue;
				void threadFunction(std::queue<DeviceLog> writeQueue);
		public:
				OutputWriter(std::string,std::string);
				void closeThread();
				void writeLog(DeviceLog);
				~OutputWriter();
};

#endif
