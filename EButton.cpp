//#include "Event.h"
#include "EButton.h"
//2010-02-25 все проверено ВЩ


//================================== class EButton ===========================
EButton::EButton(): EInputDevice()
{
};

oid_t EButton::init(const port_t port, const bool reverseOn, const bool pullUp)
//Инициализация c учетом флага
{
	oid_t result = EInputDevice::init(port, imUpOnly);

#ifdef DEBUG_EBUTTON
	Serial.println("EButton::init_full()");
#endif
	debounceTimer.init(DEBOUNCEDELAY,false);
	doublePressTimer.init(DOUBLEPRESSDELAY,false);
	holdTimer.init(KEYHOLDDELAY,false);
	this->reverseOn=reverseOn;		  //зададим режим реверса
	pinMode(this->port,INPUT);		 // после этого запрограммируем порт в режим чтения
	int tmp = this->port;
	if (pullUp) {
		digitalWrite(this->port, HIGH);
#ifdef DEBUG_EBUTTON
		Serial.print("EButton::init PullUp port:");
		Serial.println(tmp);
#endif
	}; 
	getData();									// считаем данные с учетом флага реверса
	this->lastState=this->currentState;
	return result;
};

/*
int EButton::handleEvent(Event& tmpEvent)
{
	EObject::handleEvent(tmpEvent);
};
*/

void EButton::getName(char* result)
{
	sprintf(result,"EButton: ID=%d port=%d ",getID(),this->port);
};

void EButton::idle()
 //процедура проверки текущего состояния.
 {
	uint16_t eventType = evNone;

	if ( debouncingStarted ) {
		if (debounceTimer.expired()) {
			//закончилась задержка дребезга - время считать показания устройства
			this->debouncingStarted=false;
			getData();
			if ( this->currentState!=this->lastState) {
				//данные изменились, теперь следует понять, не послать ли событие?
#ifdef DEBUG_EBUTTON
				int tmpID = this->getID();
				Serial.print("EButton::idle() ID=");
				Serial.print(tmpID);
				Serial.print(" Input changed!! currentState=");
				Serial.print(this->currentState);
				Serial.print("	lastState=");
				Serial.print(this->lastState);
				Serial.println("");
#endif
				if ( this->currentState==0) {
					//данные имеют низкий уровень -> нужно сформировать событие для некоторых условий
					if (doublePressTimer.expired()) {
						//таймер двойного нажатия просрочен, запустим событие нажатия
						//и заодно перезапустим таймер двойного нажатия
						if (holdTimer.expired()) {
							eventType=evKeyHold;
						  } else {
							eventType=evKeyPressed;
						}	
						doublePressTimer.start();
					} else {
						eventType=evKeyDoublePressed;
					}
				} 
				//теперь если задан какой-то тип события - надо поднимать событие
				if ( eventType!=evNone) {
#ifdef DEBUG_EBUTTON
					Serial.print("EButton::idle: eventType=");
					Serial.println(eventType);
#endif
					eventStack.pushEvent(eventType,this->getID(),0,this->currentState);
				}
				else {
					holdTimer.start();
				};
				//теперь сохраним время и значение последнего состояни
				this->lastState=this->currentState;
			}
		}
	}
	else {
		//если мы здесь - то обработка дребезга не идет, надо посмотреть состояние
		//ввода и дейстовать соответственно
		getData();
		if ( this->currentState!=this->lastState) {
#ifdef DEBUG_EBUTTON
			Serial.print("EButton::idle: start debouncing, newstate=");
			Serial.println(this->currentState);
#endif
			//считали данные и они не соответствуют тем, что были раньше
			//надо запустить антидребезг
			this->debouncingStarted=true;
			debounceTimer.start();
		}
	}
};
