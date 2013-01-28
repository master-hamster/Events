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
   Serial.print("Event::print Typ=");
   Serial.print(this->eventType);
   Serial.print(" Src=");
   Serial.print(this->sourceID);
   Serial.print(" Dst=");
   Serial.print(this->destinationID);
   Serial.print(" Data=");
   Serial.print( this->eventData );
   Serial.print( " Node=" );
   Serial.println( this->sourceGroupID );
};

/*
const void Event::copy(Event& newEvent)
//копировать данные в новое событие
{
   newEvent.eventType=this->eventType;
   newEvent.eventData=this->eventData;
   newEvent.sourceID=this->sourceID;
   newEvent.destinationID=this->destinationID;
   newEvent.nodeID = this->nodeID;
};
*/

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
#ifdef DEBUG_ERROR      
      Serial.println("ERROR EventStack::push: Stack Overflow!");
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
#ifdef DEBUG_ERROR      
      Serial.println("EventStack::pushEvent: Stack Overflow!");
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
      Serial.print("EventStack::pop: EventType=");
      Serial.println(newEvent.eventType);
#endif
      return 1;
   }
};


void EventStack::print()
// print all events from stack to Serial
{
   Serial.print("EventStack:print size=");
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
	Serial.print("Timer::init interval=");
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
// возвращает true если интервал ненулевой и уже прошел
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
Процедура обработки событий
Принимает событие
Проверяет, является ли оно персональным
В базовом объекте обратабываются только следующие события:
  evEnable
  evDIsable
Возвращает: 
	0 если событие не было обработано
	ID объекта, если событие было обработано
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
				Serial.print("EObject::handleEvent Enabled ID=");
				Serial.println(this->getID());
#endif
				return this->ID;          
				break;
			case evDisable:
				this->isEnabled = false;
#ifdef DEBUG_EOBJECT            
				Serial.print("EObject::handleEvent Disabled ID=");
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

const bool EObject::eventForMe(const Event& tmpEvent)
{
   return ( ( tmpEvent.destinationID == this->ID ) || ( tmpEvent.destinationID == BROADCAST_OID ) );
};

void EObject::getName(char* result) const
{
   sprintf( result,"EObject ID=%d", this->ID );
};


//============================== EDevice ==================================================
EDevice::EDevice() : EObject()
{
};

oid_t EDevice::init( const port_t port )
{
   this->port=port;
   return getID();
};

void EDevice::getName( char* result ) const
{
   sprintf( result, "EDevice: ID=%d port=%d ", getID(), this->port );
};


//========================= EInputDevice =================
EInputDevice::EInputDevice() : 
EDevice(){
   debounceTimer.init(DEBOUNCEDELAY,false);
};

oid_t EInputDevice::initReverse(const port_t port, const InputMode im)
{
   debounceTimer.init(DEBOUNCEDELAY,false);
   return EInputDevice::init( port, im, true, true );  //????!!! - порты подтягиваем, это не всегда правильно
};

oid_t EInputDevice::init(const port_t port, const InputMode im, const bool reverseOn, bool pullUp)
{
   oid_t result=EDevice::init( port );       
#ifdef DEBUG_EINPUTDEVICE
   Serial.println("EInputdevice::init_full()");
#endif
   debounceTimer.init( DEBOUNCEDELAY, false );
   this->inputMode = im;               //зададим режим создания событий устройства
   this->reverseOn = reverseOn;        //зададим режим реверса
   pinMode( this->port, INPUT );       // после этого запрограммируем порт в режим чтения, подтяжку порта не делаем
#ifdef DEBUG_EINPUTDEVICE
   int tmp = this->port;
#endif
   if (pullUp) {
      digitalWrite( this->port, HIGH );
#ifdef DEBUG_EINPUTDEVICE
      Serial.print("EInputDevice::init PullUp port:");
      Serial.println( tmp );
#endif
   } 
   else {
      digitalWrite( this->port, LOW );
#ifdef DEBUG_EINPUTDEVICE
      Serial.print("EInputDevice::init PullDown port:");
      Serial.println( tmp );
#endif
   }
   getDataFromInput();                           // считаем данные с учетом флага реверса
   this->currentData = this->currentState;
   return result;
};


void EInputDevice::idle()
//основная процедура
//при каждом проходе проверяется состояние,
//сравнивается с предыдущим состоянием и на основании этого генерируется событие
//в целом событие может генерироваться и не сразу, а на следующем проходе этой процедуры
//2010-01-16 - отдадка  2010-01-19 - отладка
{
	//обработка делается только для случая когда не начата процедура обработки дребезга или
	//она начата и уже завершена
	uint16_t eventType = evNone;

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!КОПАТЬ ЗДЕСЬ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if ( debouncingStarted ) {
		if ( debounceTimer.expired() ) {
         //закончилась задержка дребезга - время считать показания устройства
			this->debouncingStarted = false;
			getDataFromInput();
			if ( this->currentState != this->currentData ) {
				//данные изменились, теперь следует понять, не послать ли событие?
#ifdef DEBUG_EINPUTDEVICE
				int tmpID = this->getID();
				Serial.print("EInputDevice::idle() ID=");
				Serial.print(tmpID);
				Serial.print(" Input changed!! currentState=");
				Serial.print(this->currentState);
				Serial.print("   lastState=");
				Serial.print(this->lastState);
				Serial.println("");
#endif
				if ( this->currentState==0) {
					//данные имеют низкий уровень -> нужно сформировать событие для некоторых условий
					switch ( this->inputMode) {
					case imUpDown:
					case imDownOnly:
						eventType = evInputDown;
						break;
					case imToggle:
						eventType = evInputToggle;
						break;
					default:
						break;
					}
				} else {
					//если мы здесь ->уровень вырос
					switch ( this->inputMode ) {
					case imUpDown:
					case imUpOnly:
						eventType = evInputUp;
						break;
					case imToggle:
						eventType = evInputToggle;
						break;
					default:
						break;
					}
				} //конец ветки по высокому/низкому считанному уровню
				//теперь если задан какой-то тип события - надо поднимать событие
				if ( eventType != evNone ) {
#ifdef DEBUG_EINPUTDEVICE
					Serial.print("EInputDevice::idle: eventType=");
					Serial.println( eventType );
#endif
					if ( this->isEnabled ) {
						eventStack.pushEvent( eventType, this->getID(), 0, this->currentState );
					}
				
				}
				//теперь сохраним время и значение последнего состояни
				this->currentState = this->currentData;
			}
		}
	} else {
      //если мы здесь - то обработка дребезга не идет, надо посмотреть состояние
      //ввода и дейстовать соответственно
		getDataFromInput();
		if ( this->currentState != this->currentData ) {
#ifdef DEBUG_EINPUTDEVICE
			Serial.print("EInputDevice::idle: start debouncing, newstate=");
			Serial.println( this->currentState );
#endif
			//считали данные и они не соответствуют тем, что были раньше
			//надо запустить антидребезг
			this->debouncingStarted = true;
			debounceTimer.start();
		}
	}
};


int16_t EInputDevice::getData() const
//считывание данных из currentState
{
   return this->currentData;
};

int16_t EInputDevice::getDataFromInput()
//считывание данных из порта в currentState с учетом флага реверса и нормализации (для других объектов)
{
   return this->currentData = ( this->reverseOn ^ digitalRead( this->port ) );
};

void EInputDevice::riseEvent(const event_t evType) const
{
	if ( this->isEnabled ) {
		eventStack.pushEvent( evType, this->getID(), 0, this->currentState );
	}
};


void EInputDevice::getName( char* result ) const
{
   long interval = this->debounceTimer.getInterval();
   sprintf( result,"InputDevice: ID=%d port=%d reverseOn=%d debTime,ms=%ld",
            getID(), this->port, this->reverseOn, interval );
};


//========================== EOutputDevice ================
EOutputDevice::EOutputDevice() : 
EDevice(){
};

oid_t EOutputDevice::init( const port_t port, const bool reverse )
{
   oid_t result;
   result = EDevice::init( port );
   pinMode( this->port, OUTPUT );
   //set OFF at start according to reverse flag
   if ( (reverseOn = reverse) ) {
      digitalWrite( this->port, HIGH );
   } else {
      digitalWrite( this->port, LOW );
   }
   return result;
};

oid_t EOutputDevice::initReverse( const uint16_t port )
{
   return EOutputDevice::init( port, true );
};

int EOutputDevice::handleEvent( Event& tmpEvent )
{
	if ( eventForMe( tmpEvent ) ) {
		if ( this->isEnabled ) {
			switch ( tmpEvent.eventType ) {
			case evTurnOn:
				on();
				return this->getID();
				break;
			case evTurnOff:
				off();
				return this->getID();
				break;
			default:
				break;
			}
		} 
		return EDevice::handleEvent( tmpEvent );
	}
	return 0;
};

void EOutputDevice::getName( char* result ) const
{
   sprintf( result, "EOutputDevice: ID=%d port=%d reverseOn=%d", getID(), this->port, this->reverseOn );
};

void EOutputDevice::on()
//включение устройства с сохранением соответствующих полей
{
   isOn = true;
   if ( this->reverseOn ) {
      digitalWrite( this->port, LOW );
#ifdef DEBUG_ELED	  
	  Serial.println("EOD:on() REVERSE, ON");
#endif
   } 
   else {
      digitalWrite( this->port, HIGH );
#ifdef DEBUG_ELED	  
	  Serial.println("EOD:on() NO REVERSE, ON");
#endif
   }
};

void EOutputDevice::off()
//выключение устройства с сохранением соответствующих полей
{
   isOn = false;
   if ( this->reverseOn ) {
      digitalWrite( this->port, HIGH );
#ifdef DEBUG_ELED	  
	  Serial.println("EOD:off() REVERSE, OFF");
#endif
   } 
   else {
      digitalWrite( this->port, LOW );
#ifdef DEBUG_ELED	  
 	  Serial.println("EOD:off() NO REVERSE, OFF");
#endif
  };
};

void EOutputDevice::toggle()
{
  isOn?off():on();
}


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
 */



