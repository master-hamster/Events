#include "EThermo.h"

oid_t EThermo::init( const uint16_t timeout )
//Инициализация термометра с заданным интервалом и автозапуском
{
	oid_t tmp = EObject::init();
	if ( timeout == 0 ) {
		active = 0;
		timer.init( ETHERMO_THERMOMEASUREDELAY, true );
#ifdef DEBUG_ETHERMO		
		Serial.println ( "EThermo.init passive timer init with delay" );
#endif
	} else {	
		active = 1;
#ifdef DEBUG_ETHERMO		
		Serial.println ( "EThermo.init active timer init with delay" );
#endif
		timer.init(timeout,true);
	}
	currentData = getDataFromInput(); // make first measurement
	return tmp;
};

int EThermo::handleEvent( Event& tmpEvent )
//handle information request - clear event and create new event with data
{
	if ( eventForMe( tmpEvent ) ) { //we got info request - let's cleare event and tell temperature
#ifdef DEBUG_ETHERMO
		Serial.println("EThermo::handleEvent(): Its my event!");
#endif
		if ( tmpEvent.eventType == evTellMe ) {
#ifdef DEBUG_ETHERMO
			Serial.println("EThermo::handleEvent(): evTellMe");
#endif
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
#ifdef DEBUG_ETHERMO  
		Serial.print( "T=" );
		Serial.println( temperature );	
#endif 
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
#ifdef DEBUG_ETHERMO
	Serial.print( "Conversion:" );
	Serial.print( temperature );
	Serial.print( " data:" );
	Serial.println( currentData );
#endif	
	eventStack.pushEvent( evTemperature, getID(), 0, currentData ); 
}	

oid_t EThermoDallas1820::init( DallasTemperature* dt, 
				const uint16_t timeout, 
				const uint8_t precision )
{
	dallas1820 = dt;
	if ( !( dallas1820->getAddress( insideThermometer, 0 ) ) ) {
		Serial.println( "Unable to find address for Device 0" ); 
	} else {
		// Set Precision!
		dallas1820->setResolution( insideThermometer, precision );
#ifdef DEBUG_ETHERMO		
		Serial.println("Dallas inited.");
#endif		
	};	
	return EThermo::init(timeout);
 };

int16_t EThermoDallas1820::getDataFromInput()
//Get temperature from sensor to internal variable
{
	dallas1820->requestTemperatures();
	temperature = dallas1820->getTempC( insideThermometer );
	currentData = round( temperature * 100 );
#ifdef DEBUG_ETHERMO
	Serial.print( "1820.doMeasure temperature=" );
	Serial.println( temperature );
#endif
	return currentData;
};

void EThermoDallas1820::getName( char* result )
{
	sprintf( result, "EThermo" );
};

