//==================================== class ELED =========================
#include "ECHAIN.h"
#ifdef DEBUG_CHAIN
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
};

void EChain::getName(char* result)
{
   sprintf(result,"EChain: ID=%d port=%d ",getID(),this->port);
};


void EChain::on()
{
};

void EChain::off()
{
	
};

void EChain::run()
{
};

void EChain::stop()
{
};









#ifdef DEBUG_ECHAIN
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif