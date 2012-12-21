//#include "Event.h"
#include "EButton.h"
//2010-02-25 ��� ��������� ��


//================================== class EButton ===========================
EButton::EButton(): EInputDevice()
{
};

oid_t EButton::init(port_t port, bool reverseOn, bool pullUp)
//������������� c ������ �����
{
   oid_t result = EInputDevice::init(port, imUpOnly);

#ifdef DEBUG_EBUTTON
   Serial.println("EButton::init_full()");
#endif
   debounceTimer.init(DEBOUNCEDELAY,false);
   doublePressTimer.init(DOUBLEPRESSDELAY,false);
   holdTimer.init(KEYHOLDDELAY,false);
   this->reverseOn=reverseOn;        //������� ����� �������
   pinMode(this->port,INPUT);       // ����� ����� ��������������� ���� � ����� ������
   int tmp = this->port;
   if (pullUp) {
      digitalWrite(this->port, HIGH);
#ifdef DEBUG_EBUTTON
      Serial.print("EButton::init PullUp port:");
      Serial.println(tmp);
#endif
   }; 
   getData();                           // ������� ������ � ������ ����� �������
   this->lastState=this->currentState;
   return result;
};

/*
int EButton::handleEvent(Event& tmpEvent)
{
   EObject::handleEvent(tmpEvent);
};
*/

void EButton::getName(char* result)
{
   sprintf(result,"EButton: ID=%d port=%d ",getID(),this->port);
};

void EButton::idle()
 //��������� �������� �������� ���������.
 {
   uint16_t eventType = evNone;

   if ( debouncingStarted ) {
      if (debounceTimer.expired()) {
         //����������� �������� �������� - ����� ������� ��������� ����������
         this->debouncingStarted=false;
         getData();
         if ( this->currentState!=this->lastState) {
            //������ ����������, ������ ������� ������, �� ������� �� �������?
#ifdef DEBUG_EBUTTON
            int tmpID = this->getID();
            Serial.print("EButton::idle() ID=");
            Serial.print(tmpID);
            Serial.print(" Input changed!! currentState=");
            Serial.print(this->currentState);
            Serial.print("   lastState=");
            Serial.print(this->lastState);
            Serial.println("");
#endif
            if ( this->currentState==0) {
               //������ ����� ������ ������� -> ����� ������������ ������� ��� ��������� �������
               if (doublePressTimer.expired()) {
                  //������ �������� ������� ���������, �������� ������� �������
                  //� ������ ������������ ������ �������� �������
                  if (holdTimer.expired()) {
                     eventType=evKeyHold;
               	  } else {
                     eventType=evKeyPressed;
                  }   
                  doublePressTimer.start();
               } else {
                  eventType=evKeyDoublePressed;
               }
            } 
            //������ ���� ����� �����-�� ��� ������� - ���� ��������� �������
            if ( eventType!=evNone) {
#ifdef DEBUG_EBUTTON
               Serial.print("EButton::idle: eventType=");
               Serial.println(eventType);
#endif
               eventStack.pushEvent(eventType,this->getID(),0,this->currentState);
            }
            else {
               holdTimer.start();
            };
            //������ �������� ����� � �������� ���������� ��������
            this->lastState=this->currentState;
         }
      }
   }
   else {
      //���� �� ����� - �� ��������� �������� �� ����, ���� ���������� ���������
      //����� � ���������� ��������������
      getData();
      if ( this->currentState!=this->lastState) {
#ifdef DEBUG_EBUTTON
         Serial.print("EButton::idle: start debouncing, newstate=");
         Serial.println(this->currentState);
#endif
         //������� ������ � ��� �� ������������� ���, ��� ���� ������
         //���� ��������� �����������
         this->debouncingStarted=true;
         debounceTimer.start();
      }
   }
};
