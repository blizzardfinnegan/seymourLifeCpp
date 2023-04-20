#include "GpioFacade.hpp"

//The following is copied from https://elinux.org/RPi_GPIO_Code_Samples#Direct_register_access
//The correct Peripheral Base (BCM2708_PERI_BASE) is defined in https://forums.raspberrypi.com/viewtopic.php?p=1487289&sid=5f69639d1b361720b0a34dbe2b77333c#p1487289
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define BCM2708_PERI_BASE        0xfe000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

GpioFacade::GpioFacade() : RELAY_PINS({4, 5, 6, 12, 13, 17, 18, 19, 20, 26})
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
		throw std::runtime_error("Memory map failed to open!");
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;

		for(int address : GpioFacade::RELAY_PINS)
		{
				INP_GPIO(address);
				OUT_GPIO(address);
		}
};

bool GpioFacade::isValidAddress(int address)
{
		return this->RELAY_PINS.find(address) != this->RELAY_PINS.end();
}

bool GpioFacade::relayHigh(int address)
{
		if(this->isValidAddress(address))
		{
				GPIO_SET = 1<<address;
				return true;
		}
		else return false;
}

bool GpioFacade::relayLow(int address)
{
		if(this->isValidAddress(address))
		{
				GPIO_CLR = 1<<address;
				return true;
		}
		else return false;
}
