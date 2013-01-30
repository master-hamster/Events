/*
  
	Arduino Event Library
	Started 2010-01-07 by MH
	http://github.com/master-hamster/Events
	EButton object
	
	
*/	


#ifndef EButton_h
#define EButton_h
#include "Events.h"
#include "EDevice.h"

class EButton : public EInputDevice {
public:
	EButton();
	oid_t init(const port_t port, const bool reverseOn=false, const bool pullUp=true);
	virtual void getName(char* result) const;
	virtual void idle();
	void setEvents( const event_t eKeyPressed, 
				const event_t eKeyDoublePressed = evKeyDoublePressed,
				const event_t eKeyHold = evKeyHold );
protected:
	Timer doublePressTimer;
	Timer holdTimer;
private:
	event_t eventKeyPressed;
	event_t eventKeyDoublePressed;
	event_t eventKeyHold;
};


#endif