/*
  
	Arduino Event Library
	Started 2010-01-07 by MH
	http://github.com/master-hamster/Events
	Main library
	General Objects
*/	

/*	
	Задачи:
	4. Реализовать аналоговое устройство ввода с выдачей событий по уровням. 
	5. реализовать в таймере паузу и обработку переполнения millis()
	6. Сделать возможность вешать на устройство ввода определенное событие взамен штатного
	12. Сделать протоколирование событий включения
	13. Сделать отправление событий при переключении исполнительных устройств
	14. Сделать запрос-ответ состояния различных устройств
 */


#ifndef EVENT_H
#define EVENT_H

#if ARDUINO < 100
#include "WProgram.h"
#else
#include "Arduino.h"
#endif

//Uncomment next line for turn on debugging
//#define DEBUG_EVENT 

#ifdef DEBUG_EVENT
//#define PRINTNAMES
//#define DEBUG_EOBJECT
//#define DEBUG_EVENTNAMES
//#define DEBUG_EVENTSTACK
//#define DEBUG_EINPUTDEVICE
#define DEBUG_EAPPLICATION
//#define DEBUG_EBEEPER
//#define DEBUG_ETIMER
//#define DEBUG_EANALOGINPUT
#define DEBUG_EBUTTON
//#define DEBUG_ELED
//#define DEBUG_ERGBLED
//#define DEBUG_ECANDLE
//#define DEBUG_ECANDLE1
//#define DEBUG_ETHERMO
//
#define DEBUG_ERROR 
#endif


//EVent NoEvent
// 0 - 49 - system events, 50-99 - reserved for future, 101+ - application user-defined
#define evNone						0
//Object manipulation events
#define evEnable                    1   //enable event handling and generating
#define evDisable                   2   //disable event handling and generating
#define evTurnOn                    3   //turn on  EOutputDevice and successors
#define evTurnOff                   4   //turn off EOutputDevice and successors
#define evTellMe                    5   //Ask EObject to send answer Event

//ETimer create evTimerExpired when it has expired 
#define evTimerExpired               6
//ETimer stop working whet it got evTimerStop
#define evTimerStop                  7
//ETimer resume counting whet it has got evTimerStart
#define evTimerStart                 8

//Abstract Input Events
#define evInputUp                    9
#define evInputDown                 10
#define evInputToggle               11
#define evInputHold                 12
// button's events - created by EButton
#define evKeyPressed                13
#define evKeyDoublePressed          14
#define evKeyHold                   15

//событие от устройства аналогового ввода
#define evAIData                    16
//событие изменения уровня аналогового ввода/кнопки
#define evLevelChanged              17

//обнаружено движение, в данных - условный ID датчика движения
#define evMotionDetected            18
// командна на гашение света, в данных - условынй ID источника света
//#define evLEDOff                    21
// команда на включение света, в данных - условный ID источника света
//#define evLEDOn                     22
//#define evCandleOn                  23
//#define evCandleOff                 24
#define evFlicker                   19

//команды на включение и выключение устройства - заменены на TurnOn TurnOff
//#define evOutputOn				26
//#define evOutputOff			27


//============= SYSTEM PARAMETERS ===============
//максимальное количество событий в стеке событий
#define EVENTSTACKSIZE        10
//Maximum objects that can be registered, change it according to free memory
#define MAXAPPOBJECTS					20

//============= OBJECT SETTINGS =================
//задержка по времени в мс для устранения дребезка
#define DEBOUNCEDELAY       50
#define DOUBLEPRESSDELAY   500
//задержка по времени для определения события Hold
#define KEYHOLDDELAY      1000
//#define HOLDREPEATDELAY 3000

//длительность писка бипера по умолчанию
#define EBEEPER_DEFAULT_BEEP_TIME   25

//максимальное количество событий в мультиуровневой аналоговой кнопке
#define ABUTTONMAXLEVELS       5

// Candle parameters
#define CANDLEMINIMALLIGHTLEVEL		5
#define CANDLEMAXIMALLIGHTLEVEL		255
#define CANDLELIGHTDELTA			2 

//=============== only substitutions, no need to change  =======
#define PORT_REVERSE_MODE true
#define PORT_NO_REVERSE_MODE false
#define BROADCAST_OID 32767
#define EAPPLICATION_SOURCE_OID 65535
#define TIMER_AUTOSTART true
#define TIMER_NO_AUTOSTART false
#define TIMER_AUTORESTART true
#define TIMER_NO_AUTORESTART false

//============== TYPES DEFINITION =============
typedef uint16_t oid_t;
typedef uint16_t event_t;
typedef uint16_t port_t;

//====== EInputDevice modes ==================
enum InputMode {
	imUpOnly,	//выдается событие только при повышении уровня до 1 evInputUp
	imDownOnly,	//выдается событие только при падении уровня до 0 evInputDown
	imUpDown,	//выдается событие и при падении и при повышении
	imToggle		//аналогично, но событие evToggle
};
//====== ECandle modes =================
enum CandleState {
	csOff,			//полностью выключено
	csFadeIn,		//включается
	csFadeOut,		//выключается
	csOn,				//полностью включено
	csFlickering	//мелькает
};


//Main class definition 
class Event {
public:
	event_t eventType;  //тип события
	oid_t sourceID;  //идентификатор создателя
	oid_t destinationID; //идентификатор получателя, если есть
	int16_t eventData;	//дополнительные данные события
	int16_t sourceGroupID;     // optional data: source sensor group/node
	const void print();
//	const void copy(Event& newEvent); //копировать данные в новое событие
	Event& operator =( const Event& from );
};


//ниже идет определение стека событий - места, куда все объекты кидают событи
//и откуда их берет обработчик событий
//при передаче все данные дублируются, работы с памятью не производится, размер стека ограничен
class EventStack {
public:
	int push( Event& newEvent );
	int pushEvent( event_t evntType, //Push event to stack, first - Event Type
		oid_t sourceID = 0,        //Source Object ID
		oid_t destinationID = 0,   //Destination ID, 0 if no desination
		int16_t eventData = 0 );   //16-bit optional data
	int pop( Event& newEvent );    //вытащить событие из стека, 0 - нет, 1 - вытащено
	void clear(){size = 0;};       //Clear stack content
	void print();                  //print all events from stack to Serial

private:
	int size; //количество объектов в стеке
	Event items[EVENTSTACKSIZE];  //буффер с событиями
};

extern EventStack eventStack;

//простой таймер без генерации событий и т.п., все времена в миллисекундах
//если установлен autorestart, то после истечения таймера первая положительная 
// выдача expired()==TRUE приведет к перезапуску таймера, если нет - по истечении
// таймера все время будет возвращаться TRUE
class Timer {
public:
	Timer();
	Timer( const unsigned long interval );
	void init( const unsigned long interval, 
				const bool autorestart = false, 
				const bool autostart = true );
	void setInterval( const unsigned long interval );
	void setStartTime( const unsigned long newTime ){ this->startTime = newTime; };
	unsigned long int elapsedTime();
	bool expired();
	void start();
	unsigned long getStartTime(){return this->startTime;}; //когда таймер был запущен
	unsigned long getInterval(){return this->interval;};	//какой интервал
private:
	bool autorestart;          //флажок авторестарта
	bool stoped;               // if stoped then expired() always return false
	unsigned long  interval;   //интервал таймера
	unsigned long  startTime;  //дата начала интервала
};

//==================================  Objects =====================
//другой основной класс - объект, работающий с событиями
class  EObject {
public:
//	static uint8_t nextID; //счетчик идентификаторов для класса
	EObject();
	oid_t init(); // get own ID, return it
	virtual int handleEvent( Event& tmpEvent );
	virtual void idle(){};
	const bool eventForMe( const Event& tmpEvent );
	virtual void getName( char* result );
	oid_t getID() { return this->ID; };
private:
	oid_t ID;          //This Object ID
protected:
	Event event;       //Event buffer
	bool isEnabled;   //Can handle events
};


//Generic Input/output device
class EDevice : public EObject {
public:
	EDevice();
	oid_t init( const port_t port );
	virtual void getName( char* result );
protected:
	port_t port;
};

class EInputDevice : public EDevice {
public:
	EInputDevice(); //инициация по умолчанию
	oid_t initReverse( const port_t port, const InputMode im=imUpDown );
	oid_t init( const port_t port, const InputMode im=imUpDown, 
				bool reverseOn=false, bool pullUp=false );
	virtual void idle();
	virtual void getName( char* result );
	virtual int16_t getData(); // get data direct 
	virtual int16_t getDataFromInput(); //Read data from input and return it
protected:
	InputMode inputMode;	 //в каком режиме работает устройство
	bool reverseOn;			//работает с инвертированием ввода
	bool debouncingStarted; //запущена обработка дребезга
	int16_t currentState;	//текущее состояние
	int16_t currentData;    //Actual data after last measurement
	unsigned long lastTime; //время последнего изменения состояни
	Timer debounceTimer;	 //таймер для обработки дребезга
};

class EOutputDevice : public EDevice {
public:
	EOutputDevice();
	oid_t init( const port_t port, const bool reverse=false );
	oid_t initReverse( const port_t port );
	virtual int handleEvent( Event& tmpEvent );
	virtual void getName( char* result );
	virtual void on();       //Turn Output ON
	virtual void off();      //Turn Output OFF
protected:
	bool reverseOn;  //работает с инвертированием вывода
	bool isOn;       //включен или нет
};



#endif






/* Button modes:

getData - получить данные, сохраненные в буффере.


 * Режимы работы кнопок:
 *  - OneShot: OneShot only, returns 2 values (ON/OFF)
 *  - Memory: Returns (Pressed/ON/Released/OFF)
 *  - Timer: Hold System (OFF/ON/Hold)
 *  - OneShotTimer: Combi OneShot & Timer (ON/Hold/OFF)
 *  - MemoryTimer: Combi Memory & Timer
 *  (Pressed/ON/Hold/Released/OFF)

 */
