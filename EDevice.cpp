#include "EDevice.h"
#ifdef DEBUG_EDEVICE
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif

//============================== EDevice ==================================================
EDevice::EDevice() : EObject()
{
};

oid_t EDevice::init( const port_t port )
{
   this->port=port;
   return getID();
};

void EDevice::getName( char* result ) const
{
   sprintf( result, "EDevice: ID=%d port=%d ", getID(), this->port );
};


//========================= EInputDevice =================
EInputDevice::EInputDevice() : 
EDevice(){
   debounceTimer.init(DEBOUNCEDELAY,false);
};

oid_t EInputDevice::initReverse(const port_t port, const InputMode im)
{
   debounceTimer.init(DEBOUNCEDELAY,false);
   return EInputDevice::init( port, im, true, true );  //????!!! - порты подтягиваем, это не всегда правильно
};

oid_t EInputDevice::init(const port_t port, const InputMode im, const bool reverseOn, bool pullUp)
{
   oid_t result=EDevice::init( port );       
   DBG_PRINTLN( F("EInputdevice::init_full()") );
   debounceTimer.init( DEBOUNCEDELAY, false );
   this->inputMode = im;               //зададим режим создания событий устройства
   this->reverseOn = reverseOn;        //зададим режим реверса
   pinMode( this->port, INPUT );       // после этого запрограммируем порт в режим чтения, подтяжку порта не делаем
#ifdef DEBUG_EINPUTDEVICE
   int tmp = this->port;
#endif
   if (pullUp) {
      digitalWrite( this->port, HIGH );
      DBG_PRINT( F("EInputDevice::init PullUp port:") );
      DBG_PRINTLN( tmp );
   } 
   else {
      digitalWrite( this->port, LOW );
      DBG_PRINT( F("EInputDevice::init PullDown port:") );
      DBG_PRINTLN( tmp );
   }
   getDataFromInput();                           // считаем данные с учетом флага реверса
   this->currentData = this->currentState;
   return result;
};


void EInputDevice::idle()
//основная процедура
//при каждом проходе проверяется состояние,
//сравнивается с предыдущим состоянием и на основании этого генерируется событие
//в целом событие может генерироваться и не сразу, а на следующем проходе этой процедуры
//2010-01-16 - отдадка  2010-01-19 - отладка
{
	//обработка делается только для случая когда не начата процедура обработки дребезга или
	//она начата и уже завершена
	uint16_t eventType = evNone;

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!КОПАТЬ ЗДЕСЬ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if ( debouncingStarted ) {
		if ( debounceTimer.expired() ) {
         //закончилась задержка дребезга - время считать показания устройства
			this->debouncingStarted = false;
			getDataFromInput();
			if ( this->currentState != this->currentData ) {
				//данные изменились, теперь следует понять, не послать ли событие?
#ifdef DEBUG_EDEVICE
				int tmpID = this->getID();
				Serial.print( F("EInputDevice::idle() ID=") );
				Serial.print(tmpID);
				Serial.print( F(" Input changed!! currentState=") );
				Serial.print(this->currentState);
				Serial.print( F("   lastState=") );
				Serial.println(this->lastState);
#endif
				if ( this->currentState == 0 ) {
					//данные имеют низкий уровень -> нужно сформировать событие для некоторых условий
					switch ( this->inputMode ) {
					case imUpDown:
					case imDownOnly:
						eventType = evInputDown;
						break;
					case imToggle:
						eventType = evInputToggle;
						break;
					default:
						break;
					}
				} else {
					//если мы здесь ->уровень вырос
					switch ( this->inputMode ) {
					case imUpDown:
					case imUpOnly:
						eventType = evInputUp;
						break;
					case imToggle:
						eventType = evInputToggle;
						break;
					default:
						break;
					}
				} //конец ветки по высокому/низкому считанному уровню
				//теперь если задан какой-то тип события - надо поднимать событие
				if ( eventType != evNone ) {
					DBG_PRINT( F("EInputDevice::idle: eventType=") );
					DBG_PRINTLN( eventType );
					if ( this->isEnabled ) {
						eventStack.pushEvent( eventType, this->getID(), 0, this->currentState );
					}
				
				}
				//теперь сохраним время и значение последнего состояни
				this->currentState = this->currentData;
			}
		}
	} else {
      //если мы здесь - то обработка дребезга не идет, надо посмотреть состояние
      //ввода и дейстовать соответственно
		getDataFromInput();
		if ( this->currentState != this->currentData ) {
			DBG_PRINT( F("EInputDevice::idle: start debouncing, newstate=") );
			DBG_PRINTLN( this->currentState );
			//считали данные и они не соответствуют тем, что были раньше
			//надо запустить антидребезг
			this->debouncingStarted = true;
			debounceTimer.start();
		}
	}
};


int16_t EInputDevice::getData() const
//считывание данных из currentState
{
   return this->currentData;
};

int16_t EInputDevice::getDataFromInput()
//считывание данных из порта в currentState с учетом флага реверса и нормализации (для других объектов)
{
   return this->currentData = ( this->reverseOn ^ digitalRead( this->port ) );
};

void EInputDevice::riseEvent(const event_t evType) const
{
	if ( this->isEnabled ) {
		eventStack.pushEvent( evType, this->getID(), 0, this->currentState );
	}
};


void EInputDevice::getName( char* result ) const
{
   long interval = this->debounceTimer.getInterval();
   sprintf( result,"InputDevice: ID=%d port=%d reverseOn=%d debTime,ms=%ld",
            getID(), this->port, this->reverseOn, interval );
};


//========================== EOutputDevice ================
EOutputDevice::EOutputDevice() : 
EDevice(){
};

oid_t EOutputDevice::init( const port_t port, const bool reverse )
{
   oid_t result;
   result = EDevice::init( port );
   pinMode( this->port, OUTPUT );
   //set OFF at start according to reverse flag
   if ( (reverseOn = reverse) ) {
      digitalWrite( this->port, HIGH );
   } else {
      digitalWrite( this->port, LOW );
   }
   return result;
};

oid_t EOutputDevice::initReverse( const uint16_t port )
{
   return EOutputDevice::init( port, true );
};

int EOutputDevice::handleEvent( Event& tmpEvent )
{
	if ( eventForMe( tmpEvent ) ) {
		if ( this->isEnabled ) {
			switch ( tmpEvent.eventType ) {
			case evTurnOn:
				on();
				return this->getID();
				break;
			case evTurnOff:
				off();
				return this->getID();
				break;
			default:
				break;
			}
		} 
		return EDevice::handleEvent( tmpEvent );
	}
	return 0;
};

void EOutputDevice::getName( char* result ) const
{
   sprintf( result, "EOutputDevice: ID=%d port=%d reverseOn=%d", getID(), this->port, this->reverseOn );
};

void EOutputDevice::on()
//включение устройства с сохранением соответствующих полей
{
   isOn = true;
   if ( this->reverseOn ) {
      digitalWrite( this->port, LOW );
	  DBG_PRINTLN( F("EOD:on() REVERSE, ON") );
   } else {
      digitalWrite( this->port, HIGH );
	  DBG_PRINTLN( F("EOD:on() NO REVERSE, ON") );
   }
};

void EOutputDevice::off()
//выключение устройства с сохранением соответствующих полей
{
   isOn = false;
   if ( this->reverseOn ) {
      digitalWrite( this->port, HIGH );
	  DBG_PRINTLN( F("EOD:off() REVERSE, OFF") );
   }  else {
      digitalWrite( this->port, LOW );
 	  DBG_PRINTLN( F("EOD:off() NO REVERSE, OFF") );
  };
};

void EOutputDevice::toggle()
{
  isOn?off():on();
}

#ifdef DEBUG_EDEVICE
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif
