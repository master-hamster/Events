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
   oid_t init( const port_t ledPort, const bool reverse = false);
   virtual void getName( char* result ) const;
   bool getState(){return isOn;};
   void toggle();
   virtual void on();
   virtual void off();
//        void blink(unsigned int time, byte times=1);
   virtual int handleEvent( Event& tmpEvent );
//   virtual void idle();
protected:
   bool isOn; //включен или нет
};



#endif