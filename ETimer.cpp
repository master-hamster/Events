//============================== class ETimer ============================================
#include "ETimer.h"
#ifdef DEBUG_ETIMER
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif


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
            DBG_PRINT( "ETimer::idle() ID=" );
            DBG_PRINT( tmpID );
            DBG_PRINT( " Timer expired!! currentEvent=" );
            DBG_PRINT( this->event.eventType );
			if ( this->isEnabled ) { 
				DBG_PRINTLN(" but Event is disabled!");
			} else {
				DBG_PRINTLN("");
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

#ifdef DEBUG_ETIMER
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif
