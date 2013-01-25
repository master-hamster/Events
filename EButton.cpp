//2010-02-25 all checked MH
//2013-01-16 clear code, add 'const'
//2013-01-20 add programmable events 

#include "Events.h"
#include "EButton.h"

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

#ifdef DEBUG_EBUTTON
	Serial.println( "EButton::init_full()" );
#endif
	debounceTimer.init( DEBOUNCEDELAY, TIMER_NO_AUTORESTART, TIMER_NO_AUTOSTART );
	doublePressTimer.init( DOUBLEPRESSDELAY, TIMER_NO_AUTORESTART, TIMER_AUTOSTART );
	holdTimer.init( KEYHOLDDELAY, TIMER_NO_AUTORESTART, TIMER_NO_AUTOSTART );
	this->reverseOn = reverseOn;  
	pinMode( this->port, INPUT );       // set this port to INPUT mode
	if ( pullUp ) {
		digitalWrite( this->port, HIGH );
#ifdef DEBUG_EBUTTON
		int tmp = this->port;
		Serial.print( "EButton::init PullUp port:" );
		Serial.println( tmp );
#endif
	}; 
	getData();
	this->currentData = this->currentState;
	return result;
};

void EButton::getName( char* result )
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
				Serial.print( "EButton::idle() ID=" );
				Serial.print( tmpID );
				Serial.print( " Input changed!! currentState=" );
				Serial.print( this->currentState );
				Serial.print( "	currentData=" );
				Serial.println( this->currentData );
#endif
				if ( this->currentState == 0 ) {
					//Last state 0, new state 1 - Let's determine event type
#ifdef DEBUG_EBUTTON
					Serial.println( "EButton::idle point 1" );
#endif
					if ( doublePressTimer.expired() ) {
#ifdef DEBUG_EBUTTON
						Serial.println( "EButton::idle point 2" );
#endif
						//It is not double click, only first press
						//let's generate evKeyPressed and start doublepress timer
#ifdef DEBUG_EBUTTON
						Serial.println( "EButton::idle point 4" );
#endif
						eventType = eventKeyPressed;
						doublePressTimer.start();
						holdTimer.start();
					} else {
#ifdef DEBUG_EBUTTON
						Serial.println("EButton::idle point 5");
#endif
						eventType = eventKeyDoublePressed;
					}
				} else {  // State changed from 1 to 0, evKeyRelease?
					// !!!!!!!!!!To be coded!!!!!!!!!
				}
				//if event type was set, and button is enabled, rise event
				if ( eventType != evNone) {
#ifdef DEBUG_EBUTTON
					Serial.print( "EButton::idle(): rise eventType=" );
					Serial.print( eventType );
					Serial.print( " Src:");
					Serial.println( getID() );
#endif
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
#ifdef DEBUG_EBUTTON
					Serial.println( "EButton::idle point 3" );
#endif
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
#ifdef DEBUG_EBUTTON
			Serial.print( "EButton::idle(): start debouncing, newstate=" );
			Serial.println( this->currentData );
#endif
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
#ifdef DEBUG_EBUTTON
	Serial.println( eventKeyPressed );
	Serial.println( eventKeyDoublePressed );
	Serial.println( eventKeyHold );
#endif
};