//==================================== class EBeeper ======================
#include "EBeeper.h"

#ifdef DEBUG_EBEEPER
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif

EBeeper::EBeeper() : EOutputDevice()
{
	beepTimer.init( EBEEPER_DEFAULT_BEEP_TIME, false );
};

oid_t EBeeper::init( const port_t port, const uint16_t time, const bool reverse )
{
	oid_t result;
	result = EOutputDevice::init( port );
	beepTimer.init( time, false );
	this->reverseOn = reverse;
	return result;
};

oid_t EBeeper::initReverse( const port_t port, const uint16_t time )
{
	return EBeeper::init( port, time, true );
};


int EBeeper::handleEvent( Event& tmpEvent )
{
#ifdef DEBUG_EBEEPER
	DBG_PRINT( F("MyBeeper::handleEvent eventType=") );
	DBG_PRINTLN( tmpEvent.eventType );
#endif
	return EOutputDevice::handleEvent( tmpEvent );
};


void EBeeper::idle()
//по умолчанию отслеживаются только таймаут на выключение
{
	if ( beepTimer.expired() ) {
			off();
		}
};

void EBeeper::getName( char* result ) const
{
	sprintf( result, "EBeeper: ID=%d port=%d beepTime:%ld", getID(),
				this->port, this->beepTimer.getInterval() );
};

void EBeeper::setBeepTime( uint16_t time )
{
	this->beepTimer.setInterval( time );
};

void EBeeper::beep()
{
	DBG_PRINTLN( F("MyBeeper::beep BEEP!") );
	on();
}

void EBeeper::on()
//BEEP!
{
	beepTimer.start();
	EOutputDevice::on();
};

void EBeeper::off()
//Silence, please!
{
	EOutputDevice::off();
};

#ifdef DEBUG_EBEEPER
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif

