#include "DeviceLog.hpp"
#include <fstream>
#include "BlockingCollection.h"
#ifndef OUTPUTFACADE_H_
#define OUTPUTFACADE_H_

class OutputWriter
{
		private:
				std::string serial;
				std::string datetime;
				code_machina::BlockingQueue<DeviceLog> queue;
				void threadFunction(code_machina::BlockingQueue<DeviceLog> writeQueue);
		public:
				OutputWriter(std::string,std::string);
				void closeThread();
				void writeLog(DeviceLog);
				~OutputWriter();
};

#endif
