#include <set>
#ifndef GPIOFACADE_H_
#define GPIOFACADE_H_

class GpioFacade
{
	private:
		const std::set<int> RELAY_PINS;

    public:
		GpioFacade();
		bool isValidAddress(int);
		bool relayHigh(int);
		bool relayLow(int);
};

#endif
