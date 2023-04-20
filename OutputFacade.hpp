#include "DeviceLog.hpp"
#include <fstream>
#include "BlockingCollection.h"
#ifndef OUTPUTFACADE_H_
#define OUTPUTFACADE_H_

class OutputWriter
{
		private:
				std::ofstream fileOut;
		public:
				OutputWriter(std::string,std::string,code_machina::BlockingQueue<DeviceLog>);
				void write(DeviceLog,std::string);
				code_machina::BlockingQueue<std::string> getQueue();
				~OutputWriter();
};

#endif
