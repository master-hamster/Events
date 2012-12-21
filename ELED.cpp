#include "ELED.h"


//==================================== class ELED =========================
//�����������
ELED::ELED() : EOutputDevice()
{
};

oid_t ELED::init(port_t ledPort)
{
   //��������� ���������� ��������
   this->isOn=false;
   this->port=ledPort;
   //���������� ���� � ����������� ������
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

//�������� ����
void ELED::on(void){
#ifdef DEBUG_ELED
   Serial.print("ELED::on():ON ID=");
   int port = getID();
   Serial.println(port);
#endif   
   this->isOn=true;
   digitalWrite(this->port,HIGH);
};

//��������� ����
void ELED::off(void){
   this->isOn=false;
   digitalWrite(this->port,LOW);
};

//����������� ���� � ����������� �� ��������
void ELED::toggle(void){
   isOn ? off() : on();
};






