#ifndef EChain_h
#define EChain_h
#include "ELED.h"
#include "FastSPI_LED2.h"


//============== EChain =======
/*
   Led Chain Object
   Based on FastSPILed2 Library

*/


enum LedChainMode {
  ecmRun,
  ecmStop
} ; 

struct ecsRGB { byte g; byte r; byte b; };


class EChain : public ELED {
public:
	oid_t init(const port_t ledPort, const int16_t nLeds, void * buff);
	virtual void idle();
	virtual void getName(char* result);
	virtual void on();
	virtual void off();
	virtual void run();
	virtual void stop();
protected:
	LedChainMode  chainMode;
	int16_t numLeds; //Number of LEDs in chain
    void * ledBuffer; 
};


#endif