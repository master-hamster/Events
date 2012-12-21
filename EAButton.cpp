#include "EAButton.h"

//================================== class EAButton ======================
oid_t EAButton::init(port_t port)
{
   oid_t result =  EDevice::init(port);
//   currentLevel=0;
   debounceTimer.init(50);
   // Serial.println("EAInput");
   return result;
};

uint8_t EAButton::addLevel(int newLevel)
{
   if (this->levelCount<ABUTTONMAXLEVELS) {
      this->levels[++this->levelCount]=newLevel;
   } 
	return levelCount;
};   

int EAButton::handleEvent(Event& tmpEvent)
{
   return EObject::handleEvent(tmpEvent);
};


void EAButton::idle()
{
   uint8_t lastLevel = this->currentLevel;
   if (false==this->isDisabled) {
      getData();
      if (lastLevel!=this->currentLevel ) { //уровень изменился, сгенерируем событие
         eventStack.pushEvent(evLevelChanged,this->getID(),0,this->currentData);
#ifdef DEBUG_EABUTTON
         Serial.print("EAButton::idle: data=");
         Serial.print(this->currentData);
         Serial.print(" level=");
         Serial.println(this->currentLevel);
#endif   
      }
   }   
};

void EAButton::getName(char* result)
{
   sprintf(result,"EAButton: ID=%d port=%d ",getID(),this->port);
};


int EAButton::getData()
{
   currentData=analogRead(this->port);
   //выбрать значение уровня, при котором текущее значение не превышает 
   //порогового для данного уровня
   for (currentLevel=0; currentLevel<this->levelCount; currentLevel++) {
      if (this->currentData < this->levels[currentLevel]) break;
   } 
	return currentLevel;
};

