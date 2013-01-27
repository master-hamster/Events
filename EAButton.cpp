#include "EAButton.h"
#ifdef DEBUG_EABUTTON
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif

//================================== class EAButton ======================
oid_t EAButton::init(const port_t port)
{
	oid_t result =  EDevice::init(port);
//	currentLevel=0;
	debounceTimer.init( DEBOUNCEDELAY );
	// Serial.println("EAInput");
	return result;
};

uint8_t EAButton::addLevel(const int newLevel)
{
	if (this->levelCount<ABUTTONMAXLEVELS) {
		this->levels[++this->levelCount] = newLevel;
	} 
	return levelCount;
};	

int EAButton::handleEvent(Event& tmpEvent)
{
	return EObject::handleEvent(tmpEvent);
};


void EAButton::idle()
{
	uint8_t lastLevel = this->currentLevel;
	if ( this->isEnabled) {
		getData();
		if ( lastLevel != this->currentLevel ) { 
			//Level changed, let's generate event!
			eventStack.pushEvent( evLevelChanged, this->getID(), 0, this->currentData );
			DBG_PRINT( "EAButton::idle: data=" );
			DBG_PRINT( this->currentData );
			DBG_PRINT( " level=" );
			DBG_PRINTLN( this->currentLevel );
		}
	}	
};

void EAButton::getName( char* result )
{
	sprintf( result, "EAButton: ID=%d port=%d ", getID(), this->port );
};

int EAButton::getData()
{
	currentData=analogRead(this->port);
	//выбрать значение уровня, при котором текущее значение не превышает 
	//порогового для данного уровня
	for ( currentLevel = 0; currentLevel < this->levelCount; currentLevel++ ) {
		if ( this->currentData < this->levels[currentLevel] ) break;
	} 
	return currentLevel;
};

#ifdef DEBUG_EABUTTON
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif