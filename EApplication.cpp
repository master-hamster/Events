//class EApplication
#include "EApplication.h"

EApplication::EApplication()
{
   objectsAdded = 0;
   currentEvent.eventType=evNone;
};

oid_t EApplication::addObject(EObject* newObject)
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

const int EApplication::printNames()
//Print on Serial All Objects' Names, for debug only!
{
   char sTmp[64];
	int printCount;
   for ( printCount = 0; printCount < objectsAdded; printCount++) {
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
#ifdef DEBUG_EAPPLICATION
	Serial.println( "App::sendTestEvent() started!" );
	Serial.print( " This->objectsAdded=" );
	Serial.println( this->objectsAdded);
	Serial.println ( e1Type );
	Serial.println ( e2Type );
	Serial.println ( e1Delay );
	Serial.println ( e2Delay );
#endif

	oid_t result = 0;
	currentEvent.sourceID = EAPPLICATION_SOURCE_ID;
	for ( int i = 0; i < this->objectsAdded; i++ ) {
		currentEvent.eventType = e1Type;
		currentEvent.destinationID = objects[i]->getID();
#ifdef DEBUG_EAPPLICATION
		Serial.print( " App::sendTestEvent() ObjectID=" );
		Serial.println( i );
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
   return eventStack.pushEvent( evntType, sourceID, destinationID, eventData);   
};


int EApplication::getEvent()
//получить событие из стека событий
{
   int   i=0;
   i=eventStack.pop(currentEvent);
   if ( i) {
#ifdef DEBUG_APPLICATION
      Serial.println("EApplication::getEvent GotEvent!");
#endif
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
если один объект обработал событие, то вернется его OID, если несколько - то хрень
*/
{
	oid_t j=0;
	if ( directSend ) {
		if ( currentEvent.destinationID < objectsAdded ) {
			j = objects[currentEvent.destinationID]->handleEvent( currentEvent );
		}
	} else {
	//Run cycle for each of registered objects
		for ( int i = 0; 
			( ( i < this->objectsAdded ) && ( currentEvent.eventType != evNone ) );
			i++ ) {
#ifdef DEBUG_EAPPLICATION
			Serial.print( " App.handleEvent() ObjectID=" );
			Serial.println( i );
			currentEvent.print();
#endif
			j += objects[i]->handleEvent( currentEvent );
		}
	}
	return j;
};

