//2010-02-25 all checked MH
//2013-01-16 clear code, add 'const'
//2013-01-20 add programmable events 

#include "Events.h"
#include "EButton.h"
#ifdef DEBUG_EBUTTON
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif

//================================== class EButton ===========================
EButton::EButton(): EInputDevice()
{
};

oid_t EButton::init( const port_t port, const bool reverseOn, const bool pullUp )
{
	oid_t result = EInputDevice::init( port, imUpOnly );
	eventKeyPressed        = evKeyPressed;
	eventKeyDoublePressed  = evKeyDoublePressed;
	eventKeyHold           = evKeyHold;

	DBG_PRINTLN( "EButton::init_full()" );
	debounceTimer.init( DEBOUNCEDELAY, TIMER_NO_AUTORESTART, TIMER_NO_AUTOSTART );
	doublePressTimer.init( DOUBLEPRESSDELAY, TIMER_NO_AUTORESTART, TIMER_AUTOSTART );
	holdTimer.init( KEYHOLDDELAY, TIMER_NO_AUTORESTART, TIMER_NO_AUTOSTART );
	this->reverseOn = reverseOn;  
	pinMode( this->port, INPUT );       // set this port to INPUT mode
	if ( pullUp ) {
		digitalWrite( this->port, HIGH );
		DBG_PRINT( "EButton::init PullUp port:" );
		DBG_PRINTLN( this->port );
	}; 
	getData();
	this->currentData = this->currentState;
	return result;
};

void EButton::getName( char* result ) const
{
	sprintf( result, "EButton: ID=%d port=%d ", getID(), this->port );
};

void EButton::idle()
 //check current input,
 //rise event if input changed
 {
	uint16_t eventType = evNone;

	if ( debouncingStarted ) {
		if ( debounceTimer.expired() ) {
			//we passed debounce delay, lets get current state
			this->debouncingStarted = false;
			getDataFromInput();
			if ( this->currentState != this->currentData ) {
				//State changed. let's think have we to rise event?
#ifdef DEBUG_EBUTTON
				int tmpID = this->getID();
				DBG_PRINT( "EButton::idle() ID=" );
				DBG_PRINT( tmpID );
				DBG_PRINT( " Input changed!! currentState=" );
				DBG_PRINT( this->currentState );
				DBG_PRINT( " currentData=" );
				DBG_PRINTLN( this->currentData );
#endif
				if ( this->currentState == 0 ) {
					//Last state 0, new state 1 - Let's determine event type
					DBG_PRINTLN( "EButton::idle point 1" );
					if ( doublePressTimer.expired() ) {
						DBG_PRINTLN( "EButton::idle point 2" );
						//It is not double click, only first press
						//let's generate evKeyPressed and start doublepress timer
						DBG_PRINTLN( "EButton::idle point 4" );
						eventType = eventKeyPressed;
						doublePressTimer.start();
						holdTimer.start();
					} else {
						DBG_PRINTLN("EButton::idle point 5");
						eventType = eventKeyDoublePressed;
					}
				} else {  // State changed from 1 to 0, evKeyRelease?
					// !!!!!!!!!!To be coded!!!!!!!!!
				}
				//if event type was set, and button is enabled, rise event
				if ( eventType != evNone) {
					DBG_PRINT( "EButton::idle(): rise eventType=" );
					DBG_PRINT( eventType );
					DBG_PRINT( " Src:");
					DBG_PRINTLN( getID() );
					riseEvent( eventType );
					//eventStack.pushEvent( eventType, this->getID(), 0, this->currentState );
				} 	else {
				//	holdTimer.start();
				};
				//save current input state for future use
				this->currentState = this->currentData;
			} else { // state was't changed
				// check, if state is 1 and hold too long, so it evKeyHold
				if ( ( holdTimer.expired() ) && ( this->currentState == 1 ) ) {
					DBG_PRINTLN( "EButton::idle point 3" );
					eventType = eventKeyHold;
					riseEvent( eventType );
					//eventStack.pushEvent( eventType, this->getID(), 0, this->currentState );
				} // holdTimer.Expired
			}
		}
	} else {   // Debouncing procedure wasn't started, so lets start it!
		getDataFromInput();
		if ( this->currentState != this->currentData ) {
			//current data differ from last read, but debouncing wasn't started
			//let's start debouncing timer
			DBG_PRINT( "EButton::idle(): start debouncing, newstate=" );
			DBG_PRINTLN( this->currentData );
			this->debouncingStarted = true;
			debounceTimer.start();
		}
	}
};

void EButton::setEvents( const event_t eKeyPressed, 
				const event_t eKeyDoublePressed, const event_t eKeyHold )
{
	eventKeyPressed        = eKeyPressed;
	eventKeyDoublePressed  = eKeyDoublePressed;
	eventKeyHold           = eKeyHold;
	DBG_PRINTLN( eventKeyPressed );
	DBG_PRINTLN( eventKeyDoublePressed );
	DBG_PRINTLN( eventKeyHold );
};

#ifdef DEBUG_EBUTTON
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif
