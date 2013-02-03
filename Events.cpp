/*
  
	Arduino Event Library
	Started 2010-01-07 by MH
	http://github.com/master-hamster/Events
	General Objects:
	Event
	EventStack
	Timer
	EObject
	EDevice
	EInputDevice
	EOutputDevice
	
*/
	
#include "Events.h"


//стек событий, в который объекты запихивают свои события по мере их появлени
EventStack eventStack;
static oid_t __next_EObject_ID__; //Идентификатор следующего номера объекта. Служит для присвоения объектам уникальных идентификаторов.

#ifdef DEBUG_EVENTNAMES
#define MAXEVENTNAMES 27 
//DOESNT WORK!!!
const char* eventTypeNames[]={
	"evNone",
	"evEnable",
	"evDisable",
	"evTurnOn",
	"evTurnOff",
	"evTellMe",
	"evTimerExpired",
	"evTimerStop",
	"evTimerStart",
	"evInputUp",
	"evInputDown",
	"evInputToggle",
	"evInputHold",
	"evKeyPressed",
	"evKeyDoublePressed",
	"evKeyHold",
	"evAIData",
	"evLevelChanged",
	"evMotionDetected",
	"evFlicker",
	"evKeyReleased",
	"",
	"",
	"",	
	"",
	"",
	"",
	"",
	"UNKNOWN"
};

const char *eventName(event_t evType)
{
   return (evType>MAXEVENTNAMES) ? eventTypeNames[MAXEVENTNAMES] : eventTypeNames[evType];
}   

#endif


//=================================== class Event ==============
//отладка, при вроведении отладки на консоль выдается содержимое события в серийный порт
const void Event::print()
{
   Serial.print( F("Event::print Typ=") );
   Serial.print(this->eventType);
   Serial.print( F(" Src=") );
   Serial.print(this->sourceID);
   Serial.print( F(" Dst=") );
   Serial.print(this->destinationID);
   Serial.print( F(" Data=") );
   Serial.print( this->eventData );
   Serial.print( F(" Node=") );
   Serial.println( this->sourceGroupID );
};

Event& Event::operator =(const Event& from)
{
	eventType     = from.eventType;
	eventData     = from.eventData;
	sourceID      = from.sourceID;
	destinationID = from.destinationID;
	sourceGroupID = from.sourceGroupID;
	return *this;
};

//====================== EventStack =================================
int EventStack::push(Event& newEvent)
//запихнуть событие в стек и увеличить счетчик запихнутых событий
//если нет места - возвращается 0, если все ОК - 1
{
   if ( size==EVENTSTACKSIZE) {
      //!!!======== ошибка, стек переполнен
#ifdef DEBUG_EVENTSTACK      
      Serial.println( F("ERROR EventStack::push: Stack Overflow!") );
#endif      
      return 0;
   } 
   else {
#ifdef DEBUG_EVENTSTACK
      Serial.print("EventStack::push: eventType=");
      Serial.print(newEvent.eventType);
      Serial.println(", size++");
#endif
      //      size++;
      //newEvent.copy(items[size++]);
      items[size++] = newEvent;
      //      items[size]=newEvent;
      return size;
   };
};

int EventStack::pushEvent(event_t evntType,  //тип события
oid_t sourceID,  //идентификатор создателя
oid_t destinationID, //идентификатор получателя, если есть
int16_t eventData)   //дополнительные данные события
//Процедура создает в стеке событие с указанными параметрами
{
#ifdef DEBUG_EVENTSTACK
   Serial.print("EventStack::pushEvent evntType:");
   Serial.print(evntType);
   Serial.print(" sourceID:");
   Serial.print(sourceID);
   Serial.print(" destID:");
   Serial.print(destinationID);
   Serial.print(" eventData:");
   Serial.println(eventData);
#endif
   if ( size==EVENTSTACKSIZE) {
      //!!!======== ошибка, стек переполнен
#ifdef DEBUG_EVENTSTACK      
      Serial.println( F("EventStack::pushEvent: Stack Overflow!") );
#endif
      return 0;
   } 
   else {
      items[size].eventType = evntType;
      items[size].sourceID = sourceID;
      items[size].destinationID = destinationID;
      items[size].eventData = eventData;
#ifdef DEBUG_EVENTSTACK
      items[size].print();
#endif
      size++;
      return size;
   }
};

int EventStack::pop(Event& newEvent)
//Получить событие из стека в переменную newEvent
//если стек пуст - возвращается 0, если все ОК - 1
{
   if ( size==0) {
      return 0;
   } 
   else {
      size--;
      newEvent = items[size];
#ifdef DEBUG_EVENTSTACK
      Serial.print( F("EventStack::pop: EventType=") );
      Serial.println(newEvent.eventType);
#endif
      return 1;
   }
};


void EventStack::print()
// print all events from stack to Serial
{
   Serial.print( F("EventStack:print size=") );
   Serial.println(size);
   for ( int i = 0; i < size; i++ ) {
      items[i].print();
   }
};

//==================================== class Timer =========================================
Timer::Timer()
{
   setInterval(0);
   start();
};

Timer::Timer(const unsigned long interval)
{
   setInterval(interval);
   start();
};

void Timer::init(const unsigned long interval, const bool autorestart, const bool autostart )
{
	this->autorestart = autorestart;
	this->stoped = !autostart; 
	setInterval(interval);
	if ( autostart ) {
		start();
	}
#ifdef DEBUG_ETIMER
	Serial.print( F("Timer::init interval=") );
	Serial.println(this->interval);
#endif   
};

void Timer::setInterval(const unsigned long interval)
{
	this->interval = interval;
};

//вернем сколько времени прошло с начала старта таймера
unsigned long Timer::elapsedTime() const
{
	return millis() - startTime;
};

bool Timer::expired()
// returns true if not stoped and time expired
{
	if ( this->stoped ) {
		return false;
	}
	if (( interval !=0 ) && ( millis() - startTime >= interval) ) {
	//??????????код ниже нужно осмыслить или выкинуть или переделать
      // As suggested by benjamin.soelberg@gmail.com, the following line
      // this->previous_millis = millis();
      // was changed to
      // this->previous_millis += this->interval_millis;
      // But then a bug would sometimes occur when the interval was set with random,
      // so I added the following check to reinstate the previous behavior, but I am
      // not sure this fixes it

      // if (this->interval_millis == 0 || this->autoreset ) {
      // 	this->previous_millis = millis();
      //} else {
      //	this->previous_millis += this->interval_millis;
      //}
		if ( autorestart ) { //если есть флаг автосброса - сбросим время и продолжим работу
//         this->startTime=millis();
			startTime += interval;
		}
		return true;
	}
	return false;
};

void Timer::start()
{
	this->startTime = millis();
	stoped = false;
};


//==================================== class EObject =======================================
EObject::EObject()
{
//	ID = __next_EObject_ID__++;
	ID = ++__next_EObject_ID__;
 //  this->ID = nextOId++;
	this->event.eventType = evNone;           //обнуляем значение готовящегося события.
	this->event.sourceID = this->ID;    //заранее записываем свой ID в данные
	this->isEnabled = true;
};

oid_t EObject::init()
{
	this->ID = ++__next_EObject_ID__;   //берем следующий идентификатор объекта
	this->event.eventType = evNone;     //обнуляем значение готовящегося события.
	this->event.sourceID = this->ID;    //заранее записываем свой ID в данные
	return this->ID;                  //возвращаем его для внешних любознательных
}; 

int EObject::handleEvent(Event& tmpEvent)
/*
Get event
1.Check event's destination by eventForMe() call
2.Process it
Basic object handle only next events:
  evEnable  - enable event handling and rising
  evDIsable - disable event handling and rising
Returns: 
	0 if event was not handled
	Object ID if event was handled
*/
{
	if ( eventForMe( tmpEvent ) ) {
		//основные команды, которые обрабоатывает любой объект
		// - это включение и выключение объекта
		if ( this->isEnabled ) {
			switch (tmpEvent.eventType) {
			case evEnable:
				this->isEnabled = true;
#ifdef DEBUG_EOBJECT            
				Serial.print( F("EObject::handleEvent Enabled ID=") );
				Serial.println(this->getID());
#endif
				return this->ID;          
				break;
			case evDisable:
				this->isEnabled = false;
#ifdef DEBUG_EOBJECT            
				Serial.print( F("EObject::handleEvent Disabled ID=") );
				Serial.println(this->getID());
#endif
				return this->ID;          
				break;
			}
		} else {
			if ( tmpEvent.eventType == evEnable ) {
				this->isEnabled = true;
			}
			return this->ID;          
		}
	} 
    return 0;
};   

bool EObject::eventForMe(const Event& tmpEvent) const 
{
   return ( ( tmpEvent.destinationID == this->ID ) || ( tmpEvent.destinationID == BROADCAST_OID ) );
};

void EObject::getName(char* result) const
{
   sprintf( result,"EObject ID=%d", this->ID );
};




/*
 * 2010-01-06 vs: started
 * 2010-01-15 6h vs: added EInput
 * 2010-01-18 4h vs: added Timer and ETimer, not debugged
 * 2010-01-19 3h debugging EInputDevice, EBeeper
 * 2010-01-20 1.5h Event::operator=, little debugging
 * 2010-01-21 2h - отладка запихивания события в стек из MyApp по получении другого событи
 * 2010-01-31 3h - отладка, перевод основного приложения на имена от LSM2, 
 *                 выяснилось, что не работает у цифровых портов PullDown
 *         ВАЖНО: сделать тип кнопки с реакцией на обычное нажатие и двойное нажатие
 * 2010-02-01 0.5h - удален enum описывающий режим работы порта на вход, сделано bool'ом.
 * 2010-02-02 2h - доудален enum, собран и отлажен костяк приложения 
   с изменением состояний.
   состояния фиксируются пока с помощью простой целой переменной, без FSM
   признаки жизни есть, режимы переключаются. При подключенных 5 выходах, 3 входах, 
   бипере и таймере размер кода составляет 10034 байт при отключенной отладке. 
   скорость 15000 итераций/с  (цикл 67uS)
 * 2010-02-02 - 2.5h - ETimer отлажен, исправлен баг в  EventStack::push, 
   добавлен таймер с событием, меняющим текущий режим на +1 
   заменено все с millis на Timer, 
   размер кода 10608 байт, скорость 12000, цикл 82uS     
 * 2010-02-07 - 6h 
   - добавлен EButton с реакцией на двойное нажатие и 
   на длительное удержание.
   - в приложении на кнопку на эти события повешена функция включения 
   максимального и выключения всего света
   - добавлена возможность включения/выключения объекта
   - реализована функция аналогового ввода с передачей данных событием по таймеру
   - проверена возможность включения/выключения датчика света
   размер кода 11722 байта, скорость 10800, цикл 92uS
  *2010-02-10 - 1h программирование аналогового дискретизатора(многоуровневой кнопки)
      до рабочего состояния не доведена 
      код 11842 байта
  *2010-02-14 - 8h программирование ECandle  все перенесено, написана логика на простых
      операторах switch case, реализованы fadeIn и FadeOut, но без перехода 
      из одного режима в другой/
      8 объектов, код 14850 байт, freemem=660 скорость=10959, цикл 91uS
  *2010-02-15 - 2h отлкадка ECandle, переход от затухания к разгоранию и наоборот реализован,
      правда стыковка не совсем ровная, начата отладка аналоговой кнопки 
      с генерацией событий по уровням
      размер 15670 байт, freemem=685, скорость не честная, около 6400 итераций, цикл 154uS 
  *2010-02-16 - 1h спрятана переменная eventStack, сделана оболочка в Application
  *2010-02-17 - 3h переделан вызов app::pushEvent для упрощения вызова, собрана конфигурация
      совпадающая с холлом без учета выходных каскадов, но ничего не заработало  
  *2010-02-19 - 3h переделан ETimer, изменена логика работы, добавлено управление событиями      
  *2010-02-22 - 1h мелкая отладка
  *2010-02-24 - 6h чистка кода, попытка подключить static-члены, изменение типов порта и
      id объекта, удаление инициации с заданным номером объекта 
      размер кода 12238 байт, скорость 10665, цикл 93uS
      !!!Добавление символа в выдачу имени объекта увеличивает занимаемую память на 1 байт!!!
  *2010-02-25 3h
      Некоторые сложные объекты вынесены в отдельные модули.
      Event.print не работает, если какие-либо из данных 8-битные, пришлось вернуть 
      типы данных oid_t и event_t обратно на uint16_t 
      Возможно, следует делать это только в случае отладки, но сейчас лениво
  *2012-01-19 6h Куча правок по вычистке текста, исправлена пара мелких ошибок,
		изменен тип application.addObject()
  *2013-01-19 4h Правка текста, добавление обработки блокировки объектов
  *2013-01-30 1h строки обработаны F()
 */



