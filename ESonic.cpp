//================ ESonic class ====================
#include "ESonic.h"
#ifdef DEBUG_ESONIC
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif

oid_t ESonic::init( const port_t transferPort, const port_t receivePort )
{
	oid_t tmpOid;
	EDevice::init( transferPort );
	pinMode( transferPort, OUTPUT );
	pinMode( receivePort, INPUT );
	Trig_pin = transferPort;
	Echo_pin = receivePort;
	return tmpOid;
};

void ESonic::idle()
{
	EInputDevice::idle();
};

void ESonic::getName( char* result ) const
{
	sprintf( result, "ESonic: ID=%d port=%d ", getID(), this->port );
};

int16_t ESonic::getDataFromInput()
{
	long duration;
	digitalWrite( Trig_pin, LOW );
	delayMicroseconds( 2 );
	digitalWrite( Trig_pin, HIGH );
	delayMicroseconds( 10 );
	digitalWrite( Trig_pin, LOW );
	//Wait for sound reflection, timiout 10 ms == max range (1.5m)
	duration = pulseIn( Echo_pin, HIGH, 10000 );
	// us -> cm, 30us == 1m 0.3mm = 1mks
	// !!!!!!!!!!!!!!!!! to check
	currentData = duration / 30 / 2
	DBG_PRINT( F(" ESonic:GetData() duration,ms:") )
	DBG_PRINT( duration );
	DBG_PRINT( F(" distance,cm:") );
	DBG_PRINTLN( currentData );
	return currentData;
};

oid_t ESonicRanger::init( const port_t transferPort, const port_t receivePort, 
				const uint16_t maxRange, const event_t event2Rise )
{
	oid_t tmpOid;
	tmpOid = ESonic::init( transferPort, receivePort );
	range = maxRange;
	event.eventType = event2Rise;
	return tmpOid;
};


#ifdef DEBUG_ESONIC
	#undef DBG_PRINTLN 
	#undef DBG_PRINT 
#endif