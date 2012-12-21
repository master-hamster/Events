#ifndef EAEventButton_h
#define EAEventButton_h
//#include "Event.h"
#include "EAButton.h"


//объект аналогичен EAButton, за исключением того, что генерируются 
//заранее заданные события
class EAEventButton : public EAButton { //!!!???не работает пока
   public:
      oid_t init(port_t port);
      uint8_t addLevel(int newLevel,uint16_t eventType, uint16_t eventDest=0);
      virtual void idle();
      virtual void getName(char* result);
//		virtual int getData();
   private:
      int events[ABUTTONMAXLEVELS];
      int destinations[ABUTTONMAXLEVELS];
};   


#endif