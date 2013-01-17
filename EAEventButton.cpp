#include "EAEventButton.h"

//================================== class EAEventButton ======================

oid_t EAEventButton::init(const port_t port)
{
	return EAButton::init(port);
};

uint8_t EAEventButton::addLevel(const int newLevel, const uint16_t eventType, 
											const uint16_t eventDest)
{
	if (this->levelCount<ABUTTONMAXLEVELS) {
		this->levels[++this->levelCount]=newLevel;
		this->events[this->levelCount]=eventType;
		this->destinations[this->levelCount]=eventDest;
		return 0;
	} else {
	  return 1;
	}
}; 


void EAEventButton::idle()
{
};

void EAEventButton::getName(char* result)
{
	sprintf(result,"EAEventButton: ID=%d port=%d ",getID(),this->port);
};

