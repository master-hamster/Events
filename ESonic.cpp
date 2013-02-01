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
   pinMode( transferPort, OUTPUT );
   pinMode( receivePort, INPUT );
   Trig_pin = transferPort;
   Echo_pin = receivePort;
   return EDevice:init( transferPort );
}

void ESonic::idle()
{
}

void EBeeper::getName( char* result ) const
{
	sprintf( result, "ESonic: ID=%d port=%d ", getID(), this->port );
};


int16_t ESonic::getDataFromInput()
{
	uint_t16 duration;
	digitalWrite( Trig_pin, LOW );
	delayMicroseconds( 2 );
	digitalWrite( Trig_pin, HIGH );
	delayMicroseconds( 10 );
	digitalWrite( Trig_pin, LOW );
	//Wait for sound reflection
	duration = pulseIn( Echo_pin, HIGH );
	// mks -> cm
	// !!!!!!!!!!!!!!!!! to check
	currentData = duration / 33 / 2
	DBG_PRINT( F(" ESonic:GetData() duration,ms:") )
	DBG_PRINT( duration );
	DBG_PRINT( F(" distance,cm:") );
	DBG_PRINTLN( currentData );
	return currentData;
}


#ifdef DEBUG_ESONIC
	#undef DBG_PRINTLN 
	#undef DBG_PRINT 
#endif