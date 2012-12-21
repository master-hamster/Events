#ifndef EAnalogInput_h
#define EAnalogInput_h
#include "Events.h"
/*
Аналоговый ввод. Через заданное при инициализации время генерируется
событие AIData в поле DATA которого запихнуто текущее значение,
считанное из порта. Дребезг не обрабатывается
*/
class EAnalogInput : public EDevice {
   public:
      //      инициация с ID объекта и временем передачи в mS
      oid_t init(port_t port, uint16_t refreshTime=5000);
      virtual void idle();
      virtual int handleEvent(Event& tmpEvent);
      virtual void getName(char* result);
		virtual int getData();
   protected:
      int currentData; //последние считанные данные
      Timer refreshTimer;//таймер для отправления информации
//      InputMode inputMode; //в каком режиме работает устройство
//      bool reverseOn; //работает с инвертированием ввода
//      bool debouncingStarted; //запущена обработка дребезга
//      int16_t currentState; //текущее состояние
//      int16_t lastState; //последнее состояние
//      unsigned long lastTime; //время последнего изменения состояни
};   


#endif