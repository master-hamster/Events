/* This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
*/

/*
  
   модуль основных объектов для архитектуры управляемой событиями
   начат 2010-01-07
  
   Задачи:
   1. Реализовать цифровое устройство ввода с учетом режимов - ОК
   2. Реализовать цифровое устройство ввода с дребезгом - ОК
   3. Реализовать аналоговое устройство ввода - OK 070210
   4. Реализовать аналоговое устройство ввода с выдачей событий по уровням. 
   5. реализовать в таймере паузу и обработку переполнения millis()
   6. Сделать возможность вешать на устройство ввода определенное событие взамен штатного
   7. Реализовать клавишу с событиями: kbPressed, kbDoublePressed, kbLongPressed  OK 070210
   8. Перевести EInputDevice на использование таймера вместо обращения к millis() ОК
   9. Перевести EBeeper на millis() ОК
   10. Сделать объект типа AnalogLed  
   11. Сделать объект типа Candle  ОК
   12. Сделать протоколирование событий включения
   13. Сделать отправление событий при переключении исполнительных устройств
   14. Сделать запрос-ответ состояния различных устройств
	15. Изменить по модулям обработку TurnOn TurnOff
	16. Изменить по модулям обработку TellMe
 */


#ifndef EVENT_H
#define EVENT_H

#if ARDUINO < 100
#include "WProgram.h"
#else
#include "Arduino.h"
#endif
//#include "WProgram.h"
//включение отладки событий
//#define DEBUG_EVENT 

#ifdef DEBUG_EVENT
//#define PRINTNAMES OBJECT NAME PRINTING
//#define DEBUG_EOBJECT
//#define DEBUG_EVENTNAMES
//#define DEBUG_EVENTSTACK 
//#define DEBUG_EINPUTDEVICE  
//#define DEBUG_APPLICATION 
//#define DEBUG_EBEEPER 
//#define DEBUG_ETIMER 
//#define DEBUG_EANALOGINPUT
//#define DEBUG_EBUTTON 
//#define DEBUG_ELED
//#define DEBUG_ECANDLE
//#define DEBUG_ECANDLE1
//#define DEBUG_ETHERMO


//включение выдачи ошибочных сообщений на консоль, любое значение - идет выдача
#define DEBUG_ERROR 
#endif


//никакого события нет
#define evNone                0
//команды на включение и выключение устройства
#define evEnable              1  //разрешить объекту обработку событий
#define evDisable             2  //запретить объекту обработку событий
#define evTurnOn					31 //Включить объект (если есть такое состояние)
#define evTurnOff					32	//Выключить объект (если есть такое состояние)
#define evTellMe              33 //Взбодрить объект (пусть ответит, есди обучен)

//ETimer create evTimerExpired when it has expired 
#define evTimerExpired        3
//ETimer stop working whet it got evTimerStop
#define evTimerStop           4
//ETimer resume counting whet it has got evTimerStart
#define evTimerStart          5

//сенсор получил данные, в данных - условный ID кнопки
//#define evInputEvent          7

//основные события абстрактного датчика
#define evInputUp             6
#define evInputDown           7
#define evInputToggle         8
#define evInputHold           9

#define evKeyPressed          10
#define evKeyDoublePressed    11
#define evKeyHold             12


//событие от устройства аналогового ввода
#define evAIData              15
//событие изменения уровня аналогового ввода/кнопки
#define evLevelChanged        16

//обнаружено движение, в данных - условный ID датчика движения
#define evMotionDetected      17
// командна на гашение света, в данных - условынй ID источника света
//#define evLEDOff              21
// команда на включение света, в данных - условный ID источника света
//#define evLEDOn               22
//#define evCandleOn            23
//#define evCandleOff           24
#define evFlicker             25

//команды на включение и выключение устройства - заменены на TurnOn TurnOff
//#define evOutputOn            26
//#define evOutputOff           27



//перечисление возможных режимов работы устройства ввода
enum InputMode {
   imUpOnly,  //выдается событие только при повышении уровня до 1 evInputUp
   imDownOnly,//выдается событие только при падении уровня до 0 evInputDown
   imUpDown,  //выдается событие и при падении и при повышении
   imToggle   //аналогично, но событие evToggle
};

enum CandleState {
   csOff, //полностью выключено
   csFadeIn, //включается
   csFadeOut, //выключается
   csOn,      //полностью включено
   csFlickering //мелькает
};   

//задержка по времени в мс для устранения дребезка
#define DEBOUNCEDELAY      50
#define DOUBLEPRESSDELAY   500
//задержка по времени для определения события Hold
#define KEYHOLDDELAY       1000
//длительность писка бипера по умолчанию
#define EBEEPER_DEFAULT_BEEP_TIME   25

//#define HOLDREPEATDELAY 3000
//максимальное количество событий в стеке событий
#define EVENTSTACKSIZE              10
//максимальное количество событий в мультиуровневой аналоговой кнопке
#define ABUTTONMAXLEVELS            5


#define CANDLEMINIMALLIGHTLEVEL     5
#define CANDLEMAXIMALLIGHTLEVEL     255
#define CANDLELIGHTDELTA            2 

//максимальное количество объектов в приложении
#define MAXAPPOBJECTS               20

//!!!!!!перевести в статические члены класса EObject
//???счетчик для выдачи номеров новым объектам EObject, пока не задействован
typedef uint16_t oid_t;
typedef uint16_t event_t;
typedef uint16_t port_t;


//основной класс - событие
class Event {
public:
    event_t eventType;  //тип события
    oid_t sourceID;  //идентификатор создателя
    oid_t destinationID; //идентификатор получателя, если есть
    int16_t eventData;   //дополнительные данные события
    void print();
    void copy(Event& newEvent); //копировать данные в новое событие
	 Event& operator=(const Event& from);
};


//ниже идет определение стека событий - места, куда все объекты кидают событи
//и откуда их берет обработчик событий
//при передаче все данные дублируются, работы с памятью не производится, размер стека ограничен

class EventStack {
public:
    int push(Event& newEvent);
    int pushEvent(event_t evntType, //тип события
         oid_t sourceID=0,           //идентификатор создателя
         oid_t destinationID=0,      //идентификатор получателя, если есть
         int16_t eventData=0);          //дополнительные данные события
    int pop(Event& newEvent);        //вытащить событие из стека, 0 - нет, 1 - вытащено
    void clear(){size=0;};           //очистить содержимое стека событий
    void print();
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
   Timer(unsigned long interval);
   void init(unsigned long interval, bool autorestart=false);
   void setInterval(unsigned long interval);
   void setStartTime(unsigned long newTime){this->startTime=newTime;};
   unsigned long int elapsedTime();
   bool expired();
   void start();
   unsigned long getStartTime(){return this->startTime;}; //когда таймер был запущен
   unsigned long getInterval(){return this->interval;};   //какой интервал
private:
   bool autorestart;          //флажок авторестарта
   unsigned long  interval;   //интервал таймера
   unsigned long  startTime;  //дата начала интервала
};

//==================================  Объекты, работающие с событиями ==============
//другой основной класс - объект, работающий с событиями
class  EObject {
public:
 //   static uint8_t nextID; //счетчик идентификаторов для класса
   EObject();
   oid_t init(); //возвращает идентификатор объекта
   virtual int handleEvent(Event& tmpEvent);
   virtual void idle(){};
//		virtual void setEvent(uint8_t evtype, uint16_t dest, int16_t data);   //заложить событие
   int eventForMe(Event& tmpEvent);
   virtual void getName(char* result);
   oid_t getID() {return this->ID;};
private:
   oid_t ID;       //идентификатор объекта
protected:
   Event event;      //событие - буфер
   bool isDisabled;  //разрешена ли работа устройству?   
};


//устройство ввода/вывода
class EDevice : public EObject {
public:
   EDevice();
   oid_t init(port_t port);
   virtual void getName(char* result);
protected:
   port_t port;
};

class EInputDevice : public EDevice {
   public:
      EInputDevice(); //инициация по умолчанию
      oid_t initReverse(port_t port, InputMode im=imUpDown );
      oid_t init(port_t port, InputMode im=imUpDown, 
                 bool reverseOn=false, bool pullUp=false);
      virtual void idle();
      virtual void getName(char* result);
		virtual int16_t getData();
   protected:
      InputMode inputMode;    //в каком режиме работает устройство
      bool reverseOn;         //работает с инвертированием ввода
      bool debouncingStarted; //запущена обработка дребезга
      int16_t currentState;   //текущее состояние
      int16_t lastState;      //последнее состояние
      unsigned long lastTime; //время последнего изменения состояни
      Timer debounceTimer;    //таймер для обработки дребезга
};

class EOutputDevice : public EDevice {
public:
   EOutputDevice();
   oid_t init(port_t port, bool reverse=false);
   oid_t initReverse(port_t port);
   virtual void getName(char* result);
protected:
	bool reverseOn; //работает с инвертированием вывода
   bool isOn; 		 //включен или нет
   void on();      //включить выход
   void off();     //выключить выход
};


//================  прообраз главного приложения ====================================
class EApplication {
public:
   EApplication();
   int printNames();             // вывести на консоль список данных по объектам
   void printEvent();            //напечатать на консоли текущее событие
   int getEvent();               //просмотреть, нет ли событий, если есть - то получить
   int pushEvent(event_t evntType,  //тип события
         oid_t destinationID=0,   //идентификатор получателя, если есть
         oid_t sourceID=0,        //идентификатор создателя
         int16_t eventData=0);       //дополнительные данные события
   virtual int parseEvent(){return 1;};   //анализ события, необходимые действия
   int handleEvent();            //передать подчиненным на обработку
   void idle();                  //стандартный цикл
   int addObject(EObject* newObject); //добавление нового объекта, возвращает успех добавления TRUE/FALSE
//protected:
   Event currentEvent;           //текущее событие
private:
   int objectsAdded;             //количество уже добавленных объектов
   EObject* objects[MAXAPPOBJECTS]; //массив объектов, с которыми работает приложение
};





class DebounceButton {
  public:
 //   DebounceButton();
 //  ~DebounceButton();
      void ftest();
    static void updateAll(){}; 
private: 
    static DebounceButton *buttons[];
    static byte buttonId;
    byte id;
};




#endif

//    Button(byte type);
//    void assign(byte pin);
//    byte check();
//    byte check(byte mode_v);
	// Setters
//    void setMode(byte type_v);
//    void setTimer(unsigned int t);
//    void setRefresh(unsigned int r);
//      virtual int getEvent(Event tmpEvent);
//      virtual void idle();
//   private:
//    byte mode;  //режим работы кнопки
//    unsigned long hold_timer;
//    unsigned long refresh_timer;
//    unsigned int hold_level;
//    unsigned int hold_refresh;
//    bool previous;


//======================= EButton
//#define OneShot 0
//#define Memory 1
//#define Timer 2
//#define OneShotTimer 3
//#define MemoryTimer 4

//#define ON 1
//#define OFF 0
//#define Pressed 2
//#define Released 3
//#define Hold 4



/* Button modes:
 * Режимы работы кнопок:
 *  - OneShot: OneShot only, returns 2 values (ON/OFF)
 *  - Memory: Returns (Pressed/ON/Released/OFF)
 *  - Timer: Hold System (OFF/ON/Hold)
 *  - OneShotTimer: Combi OneShot & Timer (ON/Hold/OFF)
 *  - MemoryTimer: Combi Memory & Timer
 *  (Pressed/ON/Hold/Released/OFF)

 */
