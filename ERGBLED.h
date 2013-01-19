#ifndef ERGBLED_h
#define ERGBLED_h
#include "Events.h"
/*
RGB LED Output

2013-01-20 MH - lets start

*/

//#define DEBUG_ERGBLED 1

enum elState {
	elsOff,
	elsRed,
	elsGreen,
	elsBlue,
	elsWhite
};	


class ERGBLED : public EOutputDevice {
public:
   ERGBLED();
   oid_t init( const port_t rPort, const port_t gPort, const port_t bPort  );
   virtual void getName( char* result );
   bool getState(){return isOn;};
	
   void toggle();
   virtual int handleEvent( Event& tmpEvent );
protected:
   void on();
   void off();
   bool isOn; //включен или нет
	elState currentState;
	port_t portR;
	port_t portG;
	port_t portB;
};

#endif