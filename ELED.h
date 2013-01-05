#ifndef ELED_h
#define ELED_h
#include "Events.h"
/*
Светодиод, надо запрограммиовать учет реверса и мигание
сделан на базе стандартного цифрового выхода

*/

class ELED : public EOutputDevice {
public:
   ELED();
   oid_t init(port_t ledPort);
   virtual void getName(char* result);
   bool getState(){return isOn;};
   void toggle();
//        void blink(unsigned int time, byte times=1);
//        void setValue(byte val);
   virtual int handleEvent(Event& tmpEvent);
//   virtual void idle();
protected:
   void on();
   void off();
   bool isOn; //включен или нет
};





#endif