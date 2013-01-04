#include "EThermo.h"

oid_t EThermo::init(uint16_t timeout)
{
   oid_t tmp = EObject::init();
   //Инициализация термометра с заданным интервалом и автозапуском
   if (timeout == 0) {
      active = 0;
      timer.init(ETHERMO_THERMOMEASUREDELAY,true);
   } else {   
      active = 1;
      timer.init(timeout,true);
   }
   doMeasure();
   return tmp;
};

void EThermo::getName(char* result)
{
  sprintf(result,"EThermo");
};

float EThermo::getTemperature()
{
  return temperature;
};


int EThermo::handleEvent(Event& tmpEvent)
//Получение температуры
//Неоходимо получить температуру и зафиксировать ее во внутреннем поле
{
   //handle information request - clear event and create new event with data
   if (eventForMe(tmpEvent)) {
#ifdef DEBUG_ETHERMO
      Serial.println("EThermo::handleEvent(): Its my event!");
#endif
      if (tmpEvent.eventType==evTellMe) {
#ifdef DEBUG_ETHERMO
         Serial.println("EThermo::handleEvent(): evTellMe");
#endif
         tmpEvent.eventType = evNone;
         tellThemperature();
         return 1;
      } else {
      // default handler
         return EObject::handleEvent(tmpEvent);
      }
   } else {
      return 0;
   }
}

void EThermo::idle()
{
   //если задержка истекла - получить и запомнить температуру
   if (timer.expired()) {
      doMeasure();   
#ifdef DEBUG_ETHERMO  
 //     Serial.print("T=");
 //     Serial.println(temperature);   
#endif 
   } else {
#ifdef DEBUG_ETHERMO  
      Serial.print(".");
#endif 
   }//if timer.expired
   if (active) {
      tellThemperature();
   } // if active 
}

void EThermo::tellThemperature()
{
   int16_t tmpTemp = round(temperature * 100); 
#ifdef DEBUG_ETHERMO
   Serial.print("Conversion:");
   Serial.print(temperature);
   Serial.print(" data:");
   Serial.println(tmpTemp);
#endif   
   eventStack.pushEvent(evTemperature, getID(), 0, tmpTemp); 
}   

oid_t EThermoDallas1820::init(DallasTemperature* dt, uint16_t timeout, uint8_t precision)
{
   dallas1820 = dt;
   if (! (dallas1820->getAddress(insideThermometer, 0))) {
      Serial.println("Unable to find address for Device 0"); 
   } else {
#ifdef DEBUG_ETHERMO      
// установить точность!!!!!
      dallas1820->setResolution(insideThermometer, precision);
      Serial.println("Dallas inited.");
#endif      
   };   
   return EThermo::init(timeout);
 };   

void EThermoDallas1820::doMeasure()
//Получить температуру
{
   dallas1820->requestTemperatures();
   temperature = dallas1820->getTempC(insideThermometer);
#ifdef DEBUG_ETHERMO
   Serial.print("1820.doMeasure temperature=");
   Serial.println(temperature);
#endif   
};

void EThermoDallas1820::getName(char* result)
{
  sprintf(result,"EThermo");
};

