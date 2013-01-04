#ifndef EThermo_h
#define EThermo_h

#include "Events.h"
#include <DallasTemperature.h>

//default themperature measure delay
#define ETHERMO_THERMOMEASUREDELAY 1000
#define evTemperature 132

class EThermo : public EObject{
public:   
   oid_t init(uint16_t timeout = 0);
   virtual void getName(char* result);    
   virtual int handleEvent(Event& tmpEvent);
   float  getTemperature();
   virtual void doMeasure(){};
   virtual void idle();
protected:
   void tellThemperature(); // rise event with temperature
   float temperature;
   uint8_t active; //ждет события или генерирует его по таймеру
   Timer timer; 
} ;

class EThermoDallas1820 : public EThermo{
public:
   oid_t init(DallasTemperature* dt, uint16_t timeout = 0, uint8_t precision = 9);
   DeviceAddress insideThermometer;
   virtual void getName(char* result);    
   virtual void doMeasure();
protected:
  DallasTemperature* dallas1820;
};

#endif