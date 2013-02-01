/* 
  EVent/ESonic Library for HR-SC04 Ultrasonic sensor
*/
#ifndef ESonic_h
#define ESonic_h
#include "EDevice.h"

// Class ESnic - measure distance, return distance in cm
// max distance = 1.5m = 150cm, measure delay = 10ms
class ESonic : public EInputDevice {
public:
	oid_t init( const port_t transferPort, const port_t receivePort );
	virtual void idle();
	virtual void getName( char* result ) const;
	virtual int16_t getDataFromInput(); //send sound impulse, get distance in cm
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