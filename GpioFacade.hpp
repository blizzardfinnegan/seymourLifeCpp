#include <set>
#include "Device.hpp"
#ifndef GPIOFACADE_H_
#define GPIOFACADE_H_

class GpioFacade
{
	private:
		int piContext;
		const std::set<int> RELAY_PINS = {4,5,6,12,13,17,18,19,20,26};

    public:
		GpioFacade();
		void relayHigh(int);
		void relayLow(int);
		void relayHighByDevice(Device);
		void relayLowByDevice(Device);
};

#endif
