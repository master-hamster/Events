//class EApplication
#include "EApplication.h"
#ifdef DEBUG_EAPPLICATION
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif
EApplication::EApplication()
{
	objectsAdded = 0;
	currentEvent.eventType = evNone;
};

oid_t EApplication::addObject( EObject* newObject )
{
	if ( objectsAdded < MAXAPPOBJECTS) {
		objects[objectsAdded] = newObject;
		objectsAdded++;
		return newObject->getID();
	} 
	else {
		return 0;
	}
};

const int EApplication::printNames() const
//Print on Serial All Objects' Names, for debug only!
{
	char sTmp[64];
	int printCount;
	for ( printCount = 0; printCount < objectsAdded; printCount++ ) {
		objects[printCount]->getName( sTmp );
		Serial.println( sTmp );
	}
	return printCount;
};

void EApplication::sendTestEvent( const event_t e1Type, const event_t e2Type,
				const int e1Delay, const int e2Delay )
/*  
  Test all registered objects
  Send them Event1, delay delay1, send Event2, delay Delay2
  Second event sended when eventType <> evNone
*/  
{
	DBG_PRINTLN ( F("App::sendTestEvent() started!") );
	DBG_PRINT ( F(" This->objectsAdded=") );
	DBG_PRINTLN ( this->objectsAdded);
	DBG_PRINTLN ( e1Type );
	DBG_PRINTLN ( e2Type );
	DBG_PRINTLN ( e1Delay );
	DBG_PRINTLN ( e2Delay );

	oid_t result = 0;
	currentEvent.sourceID = EAPPLICATION_SOURCE_OID;
	for ( int i = 0; i < this->objectsAdded; i++ ) {
		currentEvent.eventType = e1Type;
		currentEvent.destinationID = objects[i]->getID();
#ifdef DEBUG_EAPPLICATION
		DBG_PRINT( F(" App::sendTestEvent() ObjectID=") );
		DBG_PRINTLN( i );
		currentEvent.print();
#endif
		result += objects[i]->handleEvent( currentEvent );
		delay( e1Delay );
		if ( e2Type != evNone ) {
			currentEvent.eventType = e2Type;
			objects[i]->handleEvent( currentEvent );
			delay( e2Delay );
		}
	}
};


int EApplication::pushEvent( const uint16_t evntType, // Event Type
		const oid_t destinationID,          //who have to handle this Event
		const oid_t sourceID,               //who send this Event
		const int16_t eventData )           //Optional data
{
   return eventStack.pushEvent( evntType, sourceID, destinationID, eventData );   
};


int EApplication::getEvent()
// get event from eventstack to current event, 
// return 1 if any, 0 if no events ready
{
   int i = 0;
   i = eventStack.pop( currentEvent );
   if ( i ) {
      DBG_PRINTLN( F("EApplication::getEvent GotEvent!") );
      return i;
   } 
   else {
      return 0;
   };
};

const void EApplication::printEvent()
{
	this->currentEvent.print();
};

void EApplication::idle()
{
	for ( int i=0; i<this->objectsAdded; i++ ) {
		objects[i]->idle();
	}
};

int EApplication::handleEvent( const bool directSend )
/*
функция обработки события, пускает событие по всем своим объектам, 
возвращает ненулевое значение в том случае, если какой-то объект 
при обработке вернул ненулевое значение
если один объект обработал событие, то вернется его OID,
если несколько - то хрень
*/
{
	oid_t j=0;
	for ( int i = 0; 
		( ( i < this->objectsAdded ) && ( currentEvent.eventType != evNone ) );
		i++ ) {
#ifdef DEBUG_EAPPLICATION
		DBG_PRINT( F(" App.handleEvent() ObjectID=") );
		DBG_PRINTLN( i );
		currentEvent.print();
#endif
		if ( !directSend || ( objects[i]->getID() == currentEvent.destinationID ) ) {
			j += objects[i]->handleEvent( currentEvent );
		}
	}
	return j;
};

#ifdef DEBUG_EAPPLICATION
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif
