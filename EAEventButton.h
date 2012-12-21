#ifndef EAEventButton_h
#define EAEventButton_h
//#include "Event.h"
#include "EAButton.h"


//������ ���������� EAButton, �� ����������� ����, ��� ������������ 
//������� �������� �������
class EAEventButton : public EAButton { //!!!???�� �������� ����
   public:
      oid_t init(port_t port);
      uint8_t addLevel(int newLevel,uint16_t eventType, uint16_t eventDest=0);
      virtual void idle();
      virtual void getName(char* result);
//		virtual int getData();
   private:
      int events[ABUTTONMAXLEVELS];
      int destinations[ABUTTONMAXLEVELS];
};   


#endif