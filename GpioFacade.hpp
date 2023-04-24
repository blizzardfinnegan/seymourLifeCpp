#include <set>
#ifndef GPIOFACADE_H_
#define GPIOFACADE_H_

class GpioFacade
{
	private:
		const static std::set<int> RELAY_PINS;

    public:
		GpioFacade();
		bool isValidAddress(int);
		static std::set<int> getPinSet();
		bool relayHigh(int);
		bool relayLow(int);
};

#endif
