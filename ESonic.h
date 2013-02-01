/* 
  Library for HR-SC04 Ultrasonic Ranging Module.
*/

#ifndef ESonic_h
#define ESonic_h

//#include "Events.h"
#include "EDevice.h"


class ESonic : public EInputDevice {
public:
	oid_t init( const port_t transferPort, const port_t receivePort );
	virtual void idle();
	virtual void getName( char* result ) const;
	virtual int16_t getDataFromInput(); //Read data from input and return it
private:
	int Trig_pin;
	int Echo_pin;
};


// class ESonic ranger - check if something is in range and rise event
class ESonicRanger : public ESonic {
public:
	oid_t init( const port_t transferPort, const port_t receivePort,
			const uint16_t maxRange, const event_t event2Rise );
private:
	uint16_t range; //max distance for check
};

#endif