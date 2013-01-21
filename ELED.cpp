#include "ELED.h"

//==================================== class ELED =========================
ELED::ELED() : EOutputDevice()
{
	this->isOn=false;
};

oid_t ELED::init(const port_t ledPort, const bool reverse)
{
   //this->port = ledPort;
   //инициируем порт в выключенном режиме
   //pinMode( this->port, OUTPUT );
   //digitalWrite( this->port, LOW );
   //return getID();
   return EOutputDevice::init(ledPort, reverse);
};

void ELED::getName( char* result )
{
	sprintf( result, "ELED: ID=%d port=%d ", getID(), this->port );
};

int ELED::handleEvent( Event& tmpEvent )
{
#ifdef DEBUG_ELED
	Serial.print("ELED::handleEvent() ID:");
	Serial.print(getID());
	Serial.print(" got eventType=");
	Serial.println(tmpEvent.eventType);
#endif

	if ( eventForMe( tmpEvent ) ) {
#ifdef DEBUG_ELED
		Serial.print("ELED::handleEvent() ID:");
		Serial.print(getID());
		Serial.print(" got for me eventType=");
		Serial.println(tmpEvent.eventType);
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
	Serial.print( "ELED::on():ON ID=" );
	int port = getID();
	Serial.println(port);
#endif   
	EOutputDevice::on();
};

void ELED::off( void ) {
#ifdef DEBUG_ELED
	Serial.print( "ELED::off():OFF ID=" );
	int port = getID();
	Serial.println( port );
#endif   
	EOutputDevice::off();
};

void ELED::toggle( void ){
	isOn ? off() : on();
};






