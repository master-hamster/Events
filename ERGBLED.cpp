#include "ERGBLED.h"

//==================================== class ERGBLED =========================
// ????Reverce????

ERGBLED::ERGBLED() : EOutputDevice()
{
};

oid_t ERGBLED::init( const port_t rPort, const port_t gPort, const port_t bPort  )
{
   //Save initial states
   this->isOn  = false;
   this->port  = rPort;
	this->portR = rPort;
	this->portG = gPort;
	this->portB = bPort;
	this->currentState = elsOff;
	
   //Set mode for all output ports
   pinMode( this->portR, OUTPUT );
   pinMode( this->portG, OUTPUT );
   pinMode( this->portB, OUTPUT );
   digitalWrite( this->portR, LOW );
   digitalWrite( this->portG, LOW );
   digitalWrite( this->portB, LOW );

   return getID();
};

void ERGBLED::getName(char* result)
{
   sprintf( result,"ERGBLED: ID=%d ports=%d %d %d ", getID(), portR,
			 portG, portB );
};

int ERGBLED::handleEvent(Event& tmpEvent)
{
   EOutputDevice::handleEvent(tmpEvent);
   switch ( tmpEvent.eventType) {
   case evTurnOn :
      if (eventForMe(tmpEvent)) {
         on();
         return true;
      };
      break;
   case evTurnOff :
      if (eventForMe(tmpEvent)) {
         off();
         return true;
      };
      break;
   }
};

//Turn the light On according to his mode
void ERGBLED::on(void){
#ifdef DEBUG_ERGBLED
   Serial.print("ELED::on():ON ID=");
   int port = getID();
   Serial.println(port);
#endif   
   this->isOn=true;
   digitalWrite(this->port,HIGH);
};

//Turn the LED off
void ERGBLED::off(void){
   this->isOn=false;
   digitalWrite(this->port,LOW);
};

//Change LED State
void ERGBLED::toggle(void){
   isOn ? off() : on();
};






