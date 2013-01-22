#include "EAnalogInput.h"

//================================== class EAnalogInput ======================
oid_t EAnalogInput::init(port_t port, uint16_t refreshTime) 
{
   int result  =  EInputDevice::init( port );
   currentData = 0;
   refreshTimer.init( refreshTime, false );
   return result;
};
   
void EAnalogInput::idle()
{
   if ( this->isEnabled ) {
      if ( refreshTimer.expired() ) {
         refreshTimer.start();
         getDataFromInput();
#ifdef DEBUG_EANALOGINPUT
         Serial.print("EAInput::idle: value=");
         Serial.println( this->currentData );
#endif   
         eventStack.pushEvent( evAIData, this->getID(), 0, this->currentData );
      }
   }   
};
   
void EAnalogInput::getName( char* result )
{
   sprintf(result, "EAnalogInput: ID=%d port=%d ", getID(), this->port);
};   
	
int EAnalogInput::getDataFromInput()
{
	return ( currentData = analogRead( this->port ) );
};

/*
int EAnalogInput::getValue()
{
	if ( this->isEnabled ) {
		doMeasure();
	}
	return currentData;	
};
*/
/*
int EAnalogInput::getData()
{
	return currentData;
};
*/
/*
int EAnalogInput::handleEvent( Event& tmpEvent )
{
   return EObject::handleEvent( tmpEvent );
};
*/   


