#ifndef ETimer_h
#define ETimer_h
#include "Events.h"

//������, ���������� �� ��������
//�������� ��������:  
//�� evTimerStart ���������� � ������ ��������
//�� evTimerStop ��������� ��������
//����� �������� �����, ���� ������ ���������, ������� ����� ������� 
//���� ������ �� ���������������, �� 
class ETimer : public EObject {
public:
   ETimer();                                          //�������������
      //���������� � �������� ���������� � ID
   ETimer(unsigned long interval);        
      //������ ��������, ���������, �������
   oid_t init(unsigned long interval, event_t evType=evTimerExpired, 
                        bool autorestart=false); 
   virtual int handleEvent(Event& tmpEvent);
   virtual void getName(char* result);
   virtual void idle();
   void start(); //������ �������
   void stop(); //��������� �������
private:
   Timer timer;
   bool stoped;  //������ ����������
   bool expired;  //������ ��� ��������
   bool autorestart;
};



#endif