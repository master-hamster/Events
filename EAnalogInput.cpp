#include "EAnalogInput.h"

//================================== class EAnalogInput ======================
oid_t EAnalogInput::init(port_t port, uint16_t refreshTime) 
{
   int result =  EDevice::init(port);
   currentData=0;
   refreshTimer.init(refreshTime,false);
   return result;
};
   
void EAnalogInput::idle()
{
   if (false==this->isDisabled) {
      if (refreshTimer.expired()) {
         refreshTimer.start();
         getData();
#ifdef DEBUG_EANALOGINPUT
         Serial.print("EAInput::idle: value=");
         Serial.println(this->currentData);
#endif   
         eventStack.pushEvent(evAIData,this->getID(),0,this->currentData);
      }
   }   
};
   
void EAnalogInput::getName(char* result)
{
   sprintf(result,"EAnalogInput: ID=%d port=%d ",getID(),this->port);
};   
	
int EAnalogInput::getData()
{
   currentData=analogRead(this->port); 
};

int EAnalogInput::handleEvent(Event& tmpEvent)
{
   EObject::handleEvent(tmpEvent);
};
   


