#include "ETimer.h"

//============================== class ETimer ============================================
ETimer::ETimer() : 
EObject(), timer(1000){
};

ETimer::ETimer(const unsigned long interval) : 
EObject(), timer(interval){
};

oid_t ETimer::init(const unsigned long interval, const event_t evType, const bool autorestart)
{
   timer.init(interval,autorestart);
   event.eventType = evType;
   event.sourceID = getID();
   stoped = false;
   expired = false;
   this->autorestart = autorestart;
   return event.sourceID;
};

int ETimer::handleEvent(Event& tmpEvent)
{  
	if ( eventForMe( tmpEvent ) ) {
		switch (tmpEvent.eventType) {
			case evTimerStart :
				start();
				break;
			case evTimerStop :
				stop();
				break;   
			default:
				EObject::handleEvent(tmpEvent);
		};
   };   
};

void ETimer::idle()
{
   if (!this->stoped) {
      if (!this->expired) {
         if (timer.expired()) {
#ifdef DEBUG_ETIMER
            int tmpID = this->getID();
            Serial.print("ETimer::idle() ID=");
            Serial.print(tmpID);
            Serial.print(" Timer expired!! currentEvent=");
            Serial.print(this->event.eventType);
            Serial.println("");
#endif
            eventStack.push(event);
            if (this->autorestart) {
               start();
            } else {
               this->expired = true;
            }   
         }
      }   
   } 
};

void ETimer::start()
{
   stoped=false;
   expired=false;
   timer.start();
};    

void ETimer::stop()
{
   stoped=true;
};        

void ETimer::getName(char* result)
{
   sprintf(result,"ETimer ID=%d Started=%ld ", getID(), timer.getStartTime());
};
