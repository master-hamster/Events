//==================================== class ELED =========================
#include "ELED.h"
#ifdef DEBUG_ELED
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif

ELED::ELED() : EOutputDevice()
{
	this->isOn=false;
};

oid_t ELED::init( const port_t ledPort, const bool reverse )
{
   return EOutputDevice::init( ledPort, reverse );
};

void ELED::getName( char* result ) const
{
	sprintf( result, "ELED: ID=%d port=%d ", getID(), this->port );
};

int ELED::handleEvent( Event& tmpEvent )
{
#ifdef DEBUG_ELED
	DBG_PRINT( "ELED::handleEvent() ID:" );
	DBG_PRINT( getID() );
	DBG_PRINT( " got eventType=" );
	DBG_PRINTLN( tmpEvent.eventType );
#endif

	if ( eventForMe( tmpEvent ) ) {
#ifdef DEBUG_ELED
		DBG_PRINT( "ELED::handleEvent() ID:" );
		DBG_PRINT( getID() );
		DBG_PRINT( " got for me eventType=" );
		DBG_PRINTLN( tmpEvent.eventType );
#endif
		if ( this->isEnabled ) {
			switch ( tmpEvent.eventType) {
			case evTurnOn :
				this->on();
				return getID();
				break;
			case evTurnOff :
				this->off();
				return getID();
				break;
			default:
				break;
			}
		}
	    return EOutputDevice::handleEvent( tmpEvent );
	} 
	return 0;
};

void ELED::on( void ) {
#ifdef DEBUG_ELED
	DBG_PRINT( "ELED::on():ON ID=" );
	int port = getID();
	DBG_PRINTLN(port);
#endif   
	EOutputDevice::on();
};

void ELED::off( void ) {
#ifdef DEBUG_ELED
	DBG_PRINT( "ELED::off():OFF ID=" );
	int port = getID();
	DBG_PRINTLN( port );
#endif   
	EOutputDevice::off();
};

void ELED::toggle( void ){
	isOn ? off() : on();
};

#ifdef DEBUG_ELED
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif
