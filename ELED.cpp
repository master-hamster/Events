#include "ELED.h"


//==================================== class ELED =========================
//конструктор
ELED::ELED() : EOutputDevice()
{
};

oid_t ELED::init(const port_t ledPort)
{
   //сохраняем правильные состояни
   this->isOn=false;
   this->port=ledPort;
   //инициируем порт в выключенном режиме
   pinMode(this->port,OUTPUT);
   digitalWrite(this->port,LOW);
   return getID();
};

void ELED::getName(char* result)
{
   sprintf(result,"ELED: ID=%d port=%d ",getID(),this->port);
};

int ELED::handleEvent(Event& tmpEvent)
{
//   EOutputDevice::handleEvent(tmpEvent);
   switch ( tmpEvent.eventType) {
   case evTurnOn :
      if (eventForMe(tmpEvent)) {
         on();
         return true;
      };
      break;
   case evTurnOff :
      if (eventForMe(tmpEvent)) {
         off();
         return true;
      };
      break;
   }
};

//включаем свет
void ELED::on(void){
#ifdef DEBUG_ELED
   Serial.print("ELED::on():ON ID=");
   int port = getID();
   Serial.println(port);
#endif   
   this->isOn=true;
   digitalWrite(this->port,HIGH);
};

//выключаем свет
void ELED::off(void){
   this->isOn=false;
   digitalWrite(this->port,LOW);
};

//переключаем свет в зависимости от состояни
void ELED::toggle(void){
   isOn ? off() : on();
};






