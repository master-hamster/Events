#include "EApplication.h"

//================================== class EApplication ======================
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
//печатает на консоль имена всех подобъектов, только для отладки!!
{
   char sTmp[64];
	int printCount;
   for ( printCount = 0; printCount < objectsAdded; printCount++) {
      objects[printCount]->getName( sTmp );
      Serial.println( sTmp );
   }
	return printCount;
};

int EApplication::pushEvent( const uint16_t evntType, // Event Type
		const oid_t destinationID,          //who have to handle this Event
		const oid_t sourceID,               //who send this Event
		const int16_t eventData )           //Optional data
{
   return eventStack.pushEvent(evntType,sourceID,destinationID,eventData);   
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

int EApplication::handleEvent()
/*
функция обработки события, пускает событие по всем своим объектам, 
возвращает ненулевое значение в том случае, если какой-то объект 
при обработке вернул ненулевое значение
если один объект обработал событие, то вернется его OID, если несколько - то хрень
*/
{
	int j=0;
	//цикл идет пока не кончатся объекты или пока один из них не сбросит тип событи

	for ( int i=0; ( ( i < this->objectsAdded ) && ( currentEvent.eventType != evNone ) ); i++) {
		j += objects[i]->handleEvent(currentEvent);
	}
	return j;
};

