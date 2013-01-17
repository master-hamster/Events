#ifndef ETimer_h
#define ETimer_h
#include "Events.h"

//таймер, работающий по событиям
//основные действия:  
//по evTimerStart сброситься и начать работать
//по evTimerStop перестать работать
//через заданное время, если работа разрешена, бросить вверх событие 
//если таймер не перезапускается, то 
class ETimer : public EObject {
public:
	ETimer();														//инициализация
		//аналогично с заданным интервалом и ID
	ETimer(const unsigned long interval);		  
		//задать интервал, автосброс, событие
	oid_t init(const unsigned long interval, const event_t evType = evTimerExpired, 
								const bool autorestart = false); 
	virtual int handleEvent(Event& tmpEvent);
	virtual void getName(char* result);
	virtual void idle();
	void start(); //запуск таймера
	void stop(); //остановка таймера
private:
	Timer timer;
	bool stoped;  //таймер остановлен
	bool expired;  //таймер уже сработал
	bool autorestart;
};



#endif