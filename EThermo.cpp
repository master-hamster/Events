#include "EThermo.h"

#ifdef DEBUG_ETHERMO
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif

oid_t EThermo::init( const uint16_t timeout )
//Инициализация термометра с заданным интервалом и автозапуском
{
	oid_t tmp = EObject::init();
	if ( timeout == 0 ) {
		active = 0;
		timer.init( ETHERMO_THERMOMEASUREDELAY, true );
		DBG_PRINTLN ( F("EThermo.init passive timer init with delay") );
	} else {	
		active = 1;
		DBG_PRINTLN ( F("EThermo.init active timer init with delay") );
		timer.init( timeout,true );
	}
	currentData = getDataFromInput(); // make first measurement
	return tmp;
};

int EThermo::handleEvent( Event& tmpEvent )
//handle information request - clear event and create new event with data
{
	if ( eventForMe( tmpEvent ) ) { //we got info request - let's cleare event and tell temperature
		DBG_PRINTLN( F("EThermo::handleEvent(): Its my event!") );
		if ( tmpEvent.eventType == evTellMe ) {
			DBG_PRINTLN( F("EThermo::handleEvent(): evTellMe") );
			tmpEvent.eventType = evNone;
			tellThemperature();
			return 1;
		} else {
		// default handler
			return EObject::handleEvent( tmpEvent );
		}
	} else {
		return 0;
	}
}

void EThermo::idle()
{
	//timeout expired, let's measure temperature
	if ( timer.expired() ) {
		currentData = getDataFromInput();	
		DBG_PRINT( "T=" );
		DBG_PRINTLN( temperature );	
		if ( active ) {
			tellThemperature();
		}
	} //if timer.expired
}

void EThermo::getName(char* result)
{
	sprintf( result, "EThermo" );
};

float EThermo::getTemperature()
{
  return temperature;
};

void EThermo::tellThemperature()
{
//	int16_t currentData = round( temperature * 100 ); 
	DBG_PRINT( "Conversion:" );
	DBG_PRINT( temperature );
	DBG_PRINT( " data:" );
	DBG_PRINTLN( currentData );
	eventStack.pushEvent( evTemperature, getID(), 0, currentData ); 
}	

oid_t EThermoDs1820::init( DallasTemperature* dt, 
				const uint16_t timeout, 
				const uint8_t precision )
{
	ds1820 = dt;
	if ( !( ds1820->getAddress( insideThermometer, 0 ) ) ) {
		ERR_PRINTLN( F("Unable to find address for Device 0") ); 
	} else {
		// Set Precision!
		ds1820->setResolution( insideThermometer, precision );
		DBG_PRINT( "Dallas inited." );
	};	
	return EThermo::init( timeout );
 };

int16_t EThermoDs1820::getDataFromInput()
//Get temperature from sensor to internal variable
{
	ds1820->requestTemperatures();
	temperature = ds1820->getTempC( insideThermometer );
	currentData = round( temperature * 100 );
	DBG_PRINT( F("1820.getDataFromInput() temperature=") );
	DBG_PRINTLN( temperature );
	return currentData;
};

void EThermoDs1820::getName( char* result )
{
	sprintf( result, "EThermoDs1820 " );
};

//======================= EThermoLM35 ==================
oid_t EThermoLM35::init( const port_t port, const uint16_t timeout )
{
	return EThermo::init( timeout );
 };

int16_t EThermoLM35::getDataFromInput()
//Get temperature from sensor to internal variable
{
	temperature = ( 5 * analogRead( port ) * 100 ) / 1024;
	currentData = round( temperature * 100 );
	DBG_PRINT( "LM35.getDataFromInput() temperature=" );
	DBG_PRINTLN( temperature );
	return currentData;
};

void EThermoLM35::getName( char* result )
{
	sprintf( result, "EThermoLM35 " );
};



#ifdef DEBUG_ETHERMO
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif
