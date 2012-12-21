#ifndef EButton_h
#define EButton_h
#include "Events.h"


class EButton : public EInputDevice {
   public:
      EButton();
		oid_t init(port_t port, bool reverseOn=false, bool pullUp=true);
 //     virtual int handleEvent(Event& tmpEvent);
      virtual void getName(char* result);
      virtual void idle();
   protected:
	   Timer doublePressTimer;
		Timer holdTimer;
};


#endif