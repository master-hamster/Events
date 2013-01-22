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
	debounceTimer.init( DEBOUNCEDELAY, false );
	doublePressTimer.init( DOUBLEPRESSDELAY, false );
	holdTimer.init( KEYHOLDDELAY, false );
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
		if (debounceTimer.expired()) {
			//we passed debounce delay, lets get current state
			this->debouncingStarted = false;
			getDataFromInput();
			if ( this->currentState != this->currentData) {
				//State changed. let's think have we to rise event?
#ifdef DEBUG_EBUTTON
				int tmpID = this->getID();
				Serial.print( "EButton::idle() ID=" );
				Serial.print( tmpID );
				Serial.print(" Input changed!! currentState=");
				Serial.print( this->currentState );
				Serial.print("	currentData=");
				Serial.print( this->currentData );
				Serial.println("");
#endif
				if ( this->currentState == 0) {
					//данные имеют низкий уровень -> нужно сформировать событие для некоторых условий
					if ( doublePressTimer.expired() ) {
						//таймер двойного нажатия просрочен, запустим событие нажатия
						//и заодно перезапустим таймер двойного нажатия
						if ( holdTimer.expired() ) {
							eventType = eventKeyHold;
						  } else {
							eventType = eventKeyPressed;
						}	
						doublePressTimer.start();
					} else {
						eventType = eventKeyDoublePressed;
					}
				} 
				//if event type was set, and button is enabled, rise event
				if ( ( eventType != evNone) && ( this->isEnabled ) ) {
#ifdef DEBUG_EBUTTON
					Serial.print("EButton::idle: eventType=");
					Serial.println( eventType );
#endif
					eventStack.pushEvent( eventType, this->getID(), 0, this->currentState );
				}
				else {
					holdTimer.start();
				};
				//save current input state for future use
				this->currentData = this->currentState;
			}
		}
	}
	else {
		//если мы здесь - то обработка дребезга не идет, надо посмотреть состояние
		//ввода и дейстовать соответственно
		getDataFromInput();
		if ( this->currentState != this->currentData ) {
#ifdef DEBUG_EBUTTON
			Serial.print( "EButton::idle: start debouncing, newstate=" );
			Serial.println( this->currentData );
#endif
			//current data differ from last read
			//let's start debouncing timer
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
	Serial.println(eventKeyPressed);
	Serial.println(eventKeyDoublePressed);
	Serial.println(eventKeyHold);
}