#ifndef EChain_h
#define EChain_h
#include "ELED.h"
#include "FastSPI_LED2.h"


//============== EChain =======
/*
   Led Chain Object
   Based on FastSPILed2 Library

*/

#define MAX_ECHAIN_SIZE 32

#define MAX_LED_LEVEL 50

#define DEFAULT_TRANSITION_TIME 1000

struct CRGB { byte g; byte r; byte b; };
struct CHSV {byte h; byte s; byte v;};

enum EChainMode {
  ecmRun,
  ecmStop
} ; 

enum EChainTransition {
	ectBlink,
	ectSmooth
};

struct ecsRGB { byte g; byte r; byte b; };


class EChain : public ELED {
public:
	oid_t init(const port_t ledPort, const int16_t nLeds, CRGB * buff, const long transTime = DEFAULT_TRANSITION_TIME);
//	oid_t init(const port_t ledPort, const int16_t nLeds, CRGB[] * buff, 
//				const long transTime = 50000);
	virtual void idle();
	virtual void getName(char* result);
	virtual void on();
	virtual void off();
	virtual void run();
	virtual void stop();
protected:
	uint16_t 		numLeds; //Number of LEDs in chain
    CRGB * 			ledBuffer;
    struct CHSV hsvBuffer[MAX_ECHAIN_SIZE]; //HSV Buffer
    long 			startTime;
	long 			transitionTime;
	EChainTransition chainTransition;
	EChainMode 		chainMode;
	word tmpLedNumber; //временная переменная
	
	
	void setRGBArray();
	void blinkChain(); //OK
	void smoothChain(); //no
	void caterpillerChain();//
	
};



//Light Power Map, source: http://arduino.cc/forum/index.php/topic,147810.0.html
static byte lightPowerMap[256] = {
   0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,
   2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,
   4,4,4,4,4,4,5,5,5,5,5,6,6,6,6,6,
   7,7,7,7,8,8,8,8,9,9,9,10,10,10,10,11,
   11,11,12,12,12,13,13,13,14,14,15,15,15,16,16,17,
   17,17,18,18,19,19,20,20,21,21,22,22,23,23,24,24,
   25,25,26,26,27,28,28,29,29,30,31,31,32,32,33,34,
   34,35,36,37,37,38,39,39,40,41,42,43,43,44,45,46,
   47,47,48,49,50,51,52,53,54,54,55,56,57,58,59,60,
   61,62,63,64,65,66,67,68,70,71,72,73,74,75,76,77,
   79,80,81,82,83,85,86,87,88,90,91,92,94,95,96,98,
   99,100,102,103,105,106,108,109,110,112,113,115,116,118,120,121,
   123,124,126,128,129,131,132,134,136,138,139,141,143,145,146,148,
   150,152,154,155,157,159,161,163,165,167,169,171,173,175,177,179,
   181,183,185,187,189,191,193,196,198,200,202,204,207,209,211,214,
   216,218,220,223,225,228,230,232,235,237,240,242,245,247,250,252
};

void HSV2RGB_Adv (unsigned char &r, unsigned char &g, unsigned char &b,
unsigned int hue, unsigned char sat, unsigned char val);
#endif