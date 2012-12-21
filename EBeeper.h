#ifndef EBeeper_h
#define EBeeper_h
#include "Events.h"

/*
пищалка. предназначена для подключения пьезоэлектрической пищалки

*/
class EBeeper : public EOutputDevice {
public:
   EBeeper();
   oid_t init(port_t port, uint16_t time=EBEEPER_DEFAULT_BEEP_TIME,
                bool reverse=false);//инициировать с портом и временем писка
   oid_t initReverse(port_t port, uint16_t time=EBEEPER_DEFAULT_BEEP_TIME); //то же самое, но с реверсивным выходом
   void setBeepTime(uint16_t time);                  //задать время писка
   virtual int handleEvent(Event& tmpEvent);         //обработчик событий
   virtual void idle();                              //стандатный цикл
   void beep();                                      //бипнуть
   virtual void getName(char* result);               //выдать имя объекта
protected:
   void on();
   void off();
private:
   Timer beepTimer;
};

#endif