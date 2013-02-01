/* 
  Library for HR-SC04 Ultrasonic Ranging Module.
*/

#ifndef ESonic_h
#define ESonic_h

#include "Events.h"

class ESonic : public EInputDevice
{
public:
	void init( const int transferPort, const int receivePort );
	virtual void idle();
	virtual void getName( char* result ) const;
	virtual int16_t getDataFromInput(); //Read data from input and return it
private:
	int Trig_pin;
	int Echo_pin;
};

#endif