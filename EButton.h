/*
  
	Arduino Event Library
	Started 2010-01-07 by MH
	http://github.com/master-hamster/Events
	EButton object
	
	
*/	


#ifndef EButton_h
#define EButton_h
#include "Events.h"


class EButton : public EInputDevice {
public:
	EButton();
	oid_t init(const port_t port, const bool reverseOn=false, const bool pullUp=true);
//	virtual int handleEvent(Event& tmpEvent);
	virtual void getName(char* result);
	virtual void idle();
protected:
	Timer doublePressTimer;
	Timer holdTimer;
};


#endif