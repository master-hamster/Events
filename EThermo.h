#ifndef EThermo_h
#define EThermo_h
#include "EDevice.h"
#include "DallasTemperature.h"

//default themperature measure delay
#define ETHERMO_THERMOMEASUREDELAY 1000
#define evTemperature 132

class EThermo : public EInputDevice {
public:	
	oid_t init( const uint16_t timeout = 0 );
	virtual int handleEvent( Event& tmpEvent );
	virtual void idle();
	virtual void getName( char* result );	 
	float  getTemperature(); // return current temperature
protected:
	void tellThemperature(); // rise event with temperature
	float temperature;
	uint8_t active; //0 - waiting for info request, 1 - create events after timeout
	Timer timer; 
} ;

class EThermoDs1820 : public EThermo{
public:
	oid_t init(DallasTemperature* dt, const uint16_t timeout = 0, const uint8_t precision = 9);
	virtual void getName(char* result);	 
	virtual int16_t getDataFromInput();
	DeviceAddress insideThermometer;
protected:
	DallasTemperature* ds1820;
};

class EThermoLM35 : public EThermo{
public:
	oid_t init( const port_t port, const uint16_t timeout = 0 );
	DeviceAddress insideThermometer;
	virtual void getName(char* result);	 
	virtual int16_t getDataFromInput();
};


#endif