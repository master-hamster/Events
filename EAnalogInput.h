#ifndef EAnalogInput_h
#define EAnalogInput_h
#include "Events.h"
/*
���������� ����. ����� �������� ��� ������������� ����� ������������
������� AIData � ���� DATA �������� ��������� ������� ��������,
��������� �� �����. ������� �� ��������������
*/
class EAnalogInput : public EDevice {
   public:
      //      ��������� � ID ������� � �������� �������� � mS
      oid_t init(port_t port, uint16_t refreshTime=5000);
      virtual void idle();
      virtual int handleEvent(Event& tmpEvent);
      virtual void getName(char* result);
		virtual int getData();
   protected:
      int currentData; //��������� ��������� ������
      Timer refreshTimer;//������ ��� ����������� ����������
//      InputMode inputMode; //� ����� ������ �������� ����������
//      bool reverseOn; //�������� � ��������������� �����
//      bool debouncingStarted; //�������� ��������� ��������
//      int16_t currentState; //������� ���������
//      int16_t lastState; //��������� ���������
//      unsigned long lastTime; //����� ���������� ��������� ��������
};   


#endif