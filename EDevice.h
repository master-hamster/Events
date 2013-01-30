/*
   General objects for input and output
   EDevice(EObject)
     EInputDevice(EDevice)
	 EOutputDevice(EDevice)
	 
*/
#ifndef EDevice_h
#define EDevice_h

#include <Events.h>

//Generic Input/output device
class EDevice : public EObject {
public:
	EDevice();
	oid_t init( const port_t port );
	virtual void getName( char* result ) const;
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
	virtual void getName( char* result ) const;
	virtual int16_t getData() const; // get data direct 
	virtual int16_t getDataFromInput(); //Read data from input and return it
protected:
	void riseEvent(const event_t evType) const;
	InputMode inputMode;	 //в каком режиме работает устройство
	bool reverseOn;			//работает с инвертированием ввода
	bool debouncingStarted; //запущена обработка дребезга
	int16_t currentState;	//Current state (after bounce check etc)
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
	virtual void getName( char* result ) const;
	virtual void on();       //Turn Output ON
	virtual void off();      //Turn Output OFF
	void toggle();
protected:
	bool reverseOn;  //работает с инвертированием вывода
	bool isOn;       //включен или нет
};



#endif