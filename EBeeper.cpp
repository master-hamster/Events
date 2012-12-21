#include "EBeeper.h"

//==================================== class EBeeper ======================

EBeeper::EBeeper() : EOutputDevice()
{
   beepTimer.init(EBEEPER_DEFAULT_BEEP_TIME,false);
};

oid_t EBeeper::init(port_t port, uint16_t time, bool reverse)
{
   oid_t result;
   result = EOutputDevice::init(port);
   beepTimer.init(time,false);
   this->reverseOn = reverse;
   return result;
};

oid_t EBeeper::initReverse(port_t port, uint16_t time)
{
   return EBeeper::init(port, time,true);
};


int EBeeper::handleEvent(Event& tmpEvent)
//������ ��������� ������ ������� ������� �� ���������
//� ���������� �� ��������� � ����� ON.
//������ ������ ������ ��������� ������� ���������&????????????????????????????????
{
   EOutputDevice::handleEvent(tmpEvent);
   //-----????????????????????????????????????????????????????????????????????????
#ifdef DEBUG_EBEEPER
   Serial.print("MyBeeper::handleEvent eventType=");
   Serial.println(tmpEvent.eventType);
#endif
};

void EBeeper::idle()
//�� ��������� ������������� ������ ������� �� ����������
{
   if ( beepTimer.expired()) {
         off();
      }
};

void EBeeper::getName(char* result)
{
   sprintf(result,"EBeeper: ID=%d port=%d beepTime:%d",getID(),this->port,this->beepTimer.getInterval());
};

void EBeeper::setBeepTime(uint16_t time)
{
   this->beepTimer.setInterval(time);
};

void EBeeper::beep()
{
#ifdef DEBUG_EBEEPER
   Serial.println("MyBeeper::beep BEEP!");
#endif
   on();
}

void EBeeper::on()
//��������� ���������� � ����������� ��������������� �����
{
   beepTimer.start();
   EOutputDevice::on();
};

void EBeeper::off()
//���������� ���������� � ����������� ��������������� �����
{
   EOutputDevice::off();
};


