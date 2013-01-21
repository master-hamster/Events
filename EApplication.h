#ifndef EApplication_h
#define EApplication_h
#include "Events.h"

//================  прообраз главного приложения ====================================
class EApplication {
public:
	EApplication();
	const int printNames();                //Print all object names registered in App
	const void printEvent();               //Print current event waiting for handling
	void sendTestEvent(const event_t e1Type, const int e1Delay = 1000,
				const event_t e2Type = evNone, const int e2Delay = 1000);
	int getEvent();                  //просмотреть, нет ли событий, если есть - то получить
	int pushEvent( const event_t evntType,  //Push event for handling
		const oid_t destinationID = 0,      //Destination ID
		const oid_t sourceID = 0,           //Source ID
		const int16_t eventData = 0);       //Optional Data
	virtual void parseEvent(){};	//анализ события, необходимые действия
	int handleEvent();           //Ask all registered objects to handle event
	void idle();                 //Idle Cycle - call .idle() for all objects
	oid_t addObject( EObject* newObject ); //добавление нового объекта, возвращает OID или 0 при неудаче
protected:
	Event currentEvent;			  //Current Event that waits for processing
private:
	int objectsAdded;				 //Total objects registered in App
	EObject* objects[MAXAPPOBJECTS]; //Object ID's buffer
};

#endif