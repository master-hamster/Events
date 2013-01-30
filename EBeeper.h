#ifndef EBeeper_h
#define EBeeper_h
#include "EDevice.h"

/*
пищалка. предназначена для подключения пьезоэлектрической пищалки

*/
class EBeeper : public EOutputDevice {
public:
	EBeeper();
	oid_t init( const port_t port, const uint16_t time = EBEEPER_DEFAULT_BEEP_TIME,
					 const bool reverse = false );//инициировать с портом и временем писка
	oid_t initReverse( const port_t port, 
					const uint16_t time=EBEEPER_DEFAULT_BEEP_TIME); //то же самое, но с реверсивным выходом
	void setBeepTime( const uint16_t time );     //задать время писка
	virtual int handleEvent( Event& tmpEvent );	 //обработчик событий
	virtual void idle();                         //стандатный цикл
	void beep();                                 //BEEEEP!
	virtual void getName( char* result ) const;       
protected:
	void on();
	void off();
private:
	Timer beepTimer;
};

#endif