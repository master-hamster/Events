//==================================== class ELED =========================
#include "ECHAIN.h"
#define DEBUG_ECHAIN

#ifdef DEBUG_ECHAIN
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif



oid_t EChain::init(const port_t ledPort, const int16_t nLeds, void * buff)
{
    //сохраняем правильные состояния
    this->isOn=false;
	this->port=ledPort;
	this->chainMode = ecmStop;
    //инициируем порт в выключенном режиме
	pinMode(this->port,OUTPUT);
	//digitalWrite(this->port,LOW);
	ledBuffer = buff;
	numLeds = nLeds;
    return getID();
	
};

void EChain::idle()
{
	// if we have to run - run
    if (( ledChainMode == ecmRun ) && ( isOn ))	{
		
		
    }
	
};

void EChain::getName(char* result)
{
   sprintf(result,"EChain: ID=%d port=%d ",getID(),this->port);
};


void EChain::on()
{
    for(int iLed = 0; iLed < numLeds; iLed++) {
        memset(ledBuffer, 255,  numLeds * sizeof(struct CRGB));
	}
#ifdef DEBUG_ECHAIN
	DBG_PRINT( "ELED::on():ON ID=" );
	int port = getID();
	DBG_PRINTLN(port);
#endif   
//	EOutputDevice::on();
	isOn = true;
};

void EChain::off()
{
    for(int iLed = 0; iLed < numLeds; iLed++) {
        memset(ledBuffer, 0,  numLeds * sizeof(struct CRGB));
	}
#ifdef DEBUG_ECHAIN
	DBG_PRINT( "ELED::on():ON ID=" );
	int port = getID();
	DBG_PRINTLN(port);
#endif   
	//EOutputDevice::on();
	isOn = false;
	
};

void EChain::run()
{
#ifdef DEBUG_ECHAIN
	DBG_PRINT( "ELED::run():ON ID=" );
	int port = getID();
	DBG_PRINTLN(port);
#endif   
	chainMode = ecmRun;
	
	
};

void EChain::stop()
{
#ifdef DEBUG_ECHAIN
	DBG_PRINT( "ELED::stop():ON ID=" );
	int port = getID();
	DBG_PRINTLN(port);
#endif   
	chainMode = ecmStop;

	
};













void HSV2RGB_Adv (unsigned char &r, unsigned char &g, unsigned char &b,
unsigned int hue, unsigned char sat, unsigned char val)
{
  if (!sat) { // Acromatic color (gray). Hue doesn't mind.
    r=g=b=val;
  } else  { 

    
    //hue = hue%384;
    uint8_t base = ((255 - sat) * val)>>8;

    switch(hue>>8) {
    case 0:
        r = val;
        g = (((val-base)*hue)/256)+base;
        b = base;
    break;

    case 1:
        r = (((val-base)*(256-(hue%256)))/256)+base;
        g = val;
        b = base;
    break;

    case 2:
        r = base;
        g = val;
        b = (((val-base)*(hue%256))/256)+base;
    break;

    case 3:
        r = base;
        g = (((val-base)*(256-(hue%256)))/256)+base;
        b = val;
    break;

    case 4:
        r = (((val-base)*(hue%256))/256)+base;
        g = base;
        b = val;
    break;

    case 5:
        r = val;
        g = base;
        b = (((val-base)*(256-(hue%256)))/256)+base;
    break;
    }
  }   
 } // HSV 2 RGB Binary





#ifdef DEBUG_ECHAIN
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif