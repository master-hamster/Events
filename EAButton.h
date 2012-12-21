#ifndef EAButton_h
#define EAButton_h
#include "Events.h"
/*
���������� ������. ����� ��������� ��������� ��������
������� ������������ ���������� ���������, ���� ��� ������ ������, ����������������
����������� ������, �� ������ ��� ����� ������, ���������������� ������� ������.
������: ��� ��������� 0-100 1-200 2-300 �������� 175 ������������� ������� 1, �������� 255 - 2
��� ��������� ������ ������������ ������� evLevelChanged �� ��������� ������ ������ ������ 
������� � ���������� ������ ����� ����� �������������� � ������� �� ��������
*/


class EAButton : public EDevice {
	public:
      oid_t init(port_t port);
		uint8_t addLevel(int newLevel); //���������� ���������� �������
		virtual int handleEvent(Event& tmpEvent);
      virtual void idle();
      virtual void getName(char* result);
		virtual int getData();
   protected:
      Timer debounceTimer;
      uint8_t levelCount; //���������� �������
      int currentData;
      uint8_t currentLevel; 
      int levels[ABUTTONMAXLEVELS];
};      




#endif