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
   float  getTemperature(); // return current temperature
   virtual void doMeasure(){}; // get data from sensor and store it in .temperature
   virtual void idle();
protected:
   void tellThemperature(); // rise event with temperature
   float temperature;
   uint8_t active; //0 - waiting for info request, 1 - create events after timeout
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