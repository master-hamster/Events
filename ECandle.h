//
#ifndef ECandle_h
#include "Events.h"

//��������� � ������������ ��������� �������
//� �������� �����
class ECandle : public EOutputDevice {
public:  
 //  LEDCandle(int value);
   oid_t init(port_t ledPort,     //����
         uint16_t fadeinTimeout=5,  //������� ����������
         uint16_t fadeoutTimeout=15, //������� ���������   
         uint8_t maxLevel=CANDLEMAXIMALLIGHTLEVEL, //����������� �������
         uint8_t minLevel=CANDLEMINIMALLIGHTLEVEL);//������������ �������
   virtual void getName(char* result);
//  void gracefullyOn(){};
//  void gracefullyOff(){};
   virtual int handleEvent(Event& tmpEvent);
   void idle();  
//  void liteFire(){};
//  void setDelta(int value){};
   static float fscale( float originalMin, float originalMax,  //������ ������ �������
                     float newBegin, float newEnd, 
                     float inputValue, float curve);
   static int calcLightLevel(long startTime,long timeOut, //
                     int minLevel, int maxLevel);
private:
   void setLevel(int value);
   void on();
   void off();   
   void fadeIn();   
   void fadeOut();   
   void startFlickering();   
   void setState(CandleState newState);

   void setMaxLevel(int value);
   void setMinLevel(int value);
//  int getMaxLevel(){};
//  int getMinLevel(){};
//  int getCurrentLevel(){};

   uint8_t currentLevel;     // 0 - candle is off
   CandleState currentState;     // ������� ���������
   uint8_t currentMaxLevel;  //������������ ������� �����, 255 �� ���������
   uint8_t currentMinLevel;  //����������� ������� �����, 4 �� ���������
   Timer fadeinTimer;  //������ ����������
   Timer fadeoutTimer;  //������ ����������
   unsigned long fadeinTimeout; // ����� ���������/��������� 
   unsigned long fadeoutTimeout; // ����� ���������/��������� 
   unsigned long timeOfLastEvent;    //����� ���������� ���������/����������
};



#endif
//2010-02-26
