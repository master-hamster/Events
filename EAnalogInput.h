#ifndef EAnalogInput_h
#define EAnalogInput_h
#include "EDevice.h"
/*
Аналоговый ввод. Через заданное при инициализации время генерируется
событие AIData в поле DATA которого запихнуто текущее значение,
считанное из порта. Дребезг не обрабатывается
!!!!!!!!!!!   evTellMe добавить
*/
class EAnalogInput : public EInputDevice {
public:
	//		инициация с ID объекта и временем передачи в mS
	oid_t init( const port_t port, const uint16_t refreshTime = 5000 );
	virtual void idle();
//	virtual int handleEvent(Event& tmpEvent);
	virtual void getName( char* result );
	virtual int getDataFromInput();  // get current data after measuring
//	int getValue(); // do measuring and return value
//	virtual int doMeasure(); //do measuring, if object is enabled
protected:
//	int currentData; //последние считанные данные
	Timer refreshTimer;//таймер для отправления информации
//	InputMode inputMode; //в каком режиме работает устройство
//	bool reverseOn; //работает с инвертированием ввода
//	bool debouncingStarted; //запущена обработка дребезга
//	int16_t currentState; //текущее состояние
//	int16_t lastState; //последнее состояние
//	unsigned long lastTime; //время последнего изменения состояни
};


#endif