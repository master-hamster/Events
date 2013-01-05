#ifndef EAButton_h
#define EAButton_h
#include "Events.h"
/*
аналоговая кнопка. имеет несколько пороговых значений
уровень определяется аналоговым значением, если оно больше порога, соответствующего
предыдущему уровню, но меньше или равно порогу, соответствующего данному уровню.
пример: при значениях 0-100 1-200 2-300 значению 175 соответствует уровень 1, значению 255 - 2
при изменении уровня генерируется событие evLevelChanged со значением данных равным уровню 
Событие с изменением уровня может также генерироваться с защитой от дребезга
*/

class EAButton : public EDevice {
public:
	oid_t init(port_t port);
	uint8_t addLevel(int newLevel); //возвращает количество уровней
	virtual int handleEvent(Event& tmpEvent);
	virtual void idle();
	virtual void getName(char* result);
	virtual int getData();
protected:
	Timer debounceTimer;
	uint8_t levelCount; //количество уровней
	int currentData;
	uint8_t currentLevel; 
	int levels[ABUTTONMAXLEVELS];
};


#endif