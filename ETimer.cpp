#include "ETimer.h"

//============================== class ETimer ============================================
ETimer::ETimer() : 
EObject(), timer(ETIMER_DEFAULT_DELAY){
};

ETimer::ETimer(const unsigned long interval) : 
EObject(), timer(interval){
};

oid_t ETimer::init( const unsigned long interval, const event_t evType, 
					const bool autorestart, const bool autostart )
{
   timer.init( interval, autorestart, autostart );
   event.eventType   = evType;
   event.sourceID    = getID();
   stoped            = false;
   expired           = false;
   this->autorestart = autorestart;
   return event.sourceID;
};

int ETimer::handleEvent(Event& tmpEvent)
{  
	if ( eventForMe( tmpEvent ) ) {
		if ( this->isEnabled ) {
			if ( tmpEvent.eventType == evTimerStart ) {
				start();
				return getID();
			} else if ( tmpEvent.eventType == evTimerStop ) {
				stop();
				return getID();
			}
		} 
		return EObject::handleEvent( tmpEvent );
	}
	return 0;
};

void ETimer::idle()
{
   if ( !this->stoped ) {
      if ( !this->expired ) {
         if ( timer.expired() ) {
#ifdef DEBUG_ETIMER
            int tmpID = this->getID();
            Serial.print( "ETimer::idle() ID=" );
            Serial.print( tmpID );
            Serial.print( " Timer expired!! currentEvent=" );
            Serial.print( this->event.eventType );
			if ( this->isEnabled ) { 
				Serial.println(" but Event is disabled!");
			} else {
				Serial.println("");
			}
#endif
            if ( this->isEnabled ) {
				eventStack.push( event );
			};
            if ( this->autorestart ) {
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
   stoped = true;
};        

void ETimer::getName(char* result)
{
   sprintf( result,"ETimer ID=%d Started=%ld ", getID(), timer.getStartTime() );
};
