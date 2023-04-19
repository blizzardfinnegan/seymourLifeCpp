#include <iostream>
#include <pigpio.h>
#include <unistd.h>

class GpioFacade {
	private:
		int pi;
	public:
		GpioFacade() {
			pi = pigpio_start();
		};
};
