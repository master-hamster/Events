#ifndef EBeeper_h
#define EBeeper_h
#include "Events.h"

/*
�������. ������������� ��� ����������� ������������������ �������

*/
class EBeeper : public EOutputDevice {
public:
   EBeeper();
   oid_t init(port_t port, uint16_t time=EBEEPER_DEFAULT_BEEP_TIME,
                bool reverse=false);//������������ � ������ � �������� �����
   oid_t initReverse(port_t port, uint16_t time=EBEEPER_DEFAULT_BEEP_TIME); //�� �� �����, �� � ����������� �������
   void setBeepTime(uint16_t time);                  //������ ����� �����
   virtual int handleEvent(Event& tmpEvent);         //���������� �������
   virtual void idle();                              //���������� ����
   void beep();                                      //�������
   virtual void getName(char* result);               //������ ��� �������
protected:
   void on();
   void off();
private:
   Timer beepTimer;
};

#endif