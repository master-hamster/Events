#include "EAEventButton.h"


//================================== class EAEventButton ======================

oid_t EAEventButton::init(port_t port)
{
   return EAButton::init(port);
};   

uint8_t EAEventButton::addLevel(int newLevel,uint16_t eventType, uint16_t eventDest)
{
   if (this->levelCount<ABUTTONMAXLEVELS) {
      this->levels[++this->levelCount]=newLevel;
      this->events[this->levelCount]=eventType;
      this->destinations[this->levelCount]=eventDest;
   }      
};     


void EAEventButton::idle()
{
};

void EAEventButton::getName(char* result)
{
   sprintf(result,"EAEventButton: ID=%d port=%d ",getID(),this->port);
};

