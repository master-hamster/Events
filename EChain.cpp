//==================================== class ELED =========================
#include "ECHAIN.h"
#define DEBUG_ECHAIN

#ifdef DEBUG_ECHAIN
	#define DBG_PRINTLN(a) Serial.println(a)
	#define DBG_PRINT(a) Serial.print(a)
#else
	#define DBG_PRINTLN(a)
	#define DBG_PRINT(a)
#endif



oid_t EChain::init(const port_t ledPort, const int16_t nLeds,  CRGB * buff,
					const long transTime)
//					const long transTime)
{
    //сохраняем правильные состояния
    this->isOn=false;
	this->port=ledPort;
	this->chainMode = ecmStop;
    //инициируем порт в выключенном режиме
	pinMode(this->port,OUTPUT);
	//digitalWrite(this->port,LOW);
	ledBuffer = buff;
	if (nLeds > MAX_ECHAIN_SIZE) {
		numLeds = MAX_ECHAIN_SIZE;
	} else {
		numLeds = nLeds;
    }
	transitionTime = transTime;
	//transitionTime = transTime;
	//!!!!!!!!!!to change
	chainTransition = ectBlink;
	chainMode = ecmRun;
	//!!!!!!!!!!!!
	
	startTime = millis();
	
	return getID();
};

void EChain::idle()
{
	//Serial.println("Blink!");
	//blinkChain();
	caterpillerChain();
	/*
	// if we have to run - run
    if (( chainMode == ecmRun ) && ( isOn ))	{
	// here we have to set RGB buffer data	
		switch ( chainTransition ) {
		case ectBlink:
			blinkChain();
			break;
		case ectSmooth:
			smoothChain();
			break;
		default:
			break;
		}
    }
	*/
};

void EChain::getName(char* result)
{
   sprintf(result,"EChain: ID=%d port=%d ",getID(),this->port);
};


void EChain::on()
{
    for(int iLed = 0; iLed < numLeds; iLed++) {
        memset(ledBuffer, 255,  numLeds * sizeof(struct CRGB));
	}
#ifdef DEBUG_ECHAIN
	DBG_PRINT( "EChain::on():ON ID=" );
	int port = getID();
	DBG_PRINTLN(port);
#endif   
//	EOutputDevice::on();
	isOn = true;
};

void EChain::off()
{
    for(int iLed = 0; iLed < numLeds; iLed++) {
        memset(ledBuffer, 0,  numLeds * sizeof(struct CRGB));
	}
#ifdef DEBUG_ECHAIN
	DBG_PRINT( "EChain::on():ON ID=" );
	int port = getID();
	DBG_PRINTLN(port);
#endif   
	//EOutputDevice::on();
	isOn = false;
	
};

void EChain::run()
{
#ifdef DEBUG_ECHAIN
	DBG_PRINT( "EChain::run():ON ID=" );
	int port = getID();
	DBG_PRINTLN(port);
#endif   
	chainMode = ecmRun;
	
	
};

void EChain::stop()
{
#ifdef DEBUG_ECHAIN
	DBG_PRINT( "EChain::stop():ON ID=" );
	int port = getID();
	DBG_PRINTLN(port);
#endif   
	chainMode = ecmStop;

	
};

//convert from HSV to RGB array
void EChain::setRGBArray()
{
}	

//Заполнение мигающими диодами
void EChain::blinkChain()
{
	long timeNow;
	byte stepNumber;
	CRGB tmpCell;
	
	timeNow = millis();
	//получаем целую и дробную часть.
	//целая часть - номер периода
	//делим ее на 3 и определяем, каким цветом заполнять.
	//дробная часть - время внутри этапа. Сейчас не нужно.
	stepNumber = ( timeNow - startTime ) / transitionTime % 3;
/*			ledBuffer[0].r = 255; 
			ledBuffer[2].g = 5; 
			ledBuffer[4].b = 5; 
*/
	for ( int i = 0; i < numLeds; i++ ) {
//		ledBuffer[i].r = 25; 
		switch ( stepNumber ) {
		case 0: //red
			ledBuffer[i].r = 25; 
			ledBuffer[i].g = 0; 
			ledBuffer[i].b = 0; 
			break;
		case 1: //green
			ledBuffer[i].r = 0; 
			ledBuffer[i].g = 25; 
			ledBuffer[i].b = 0; 
			break;
		case 2: //blue
			ledBuffer[i].r = 0; 
			ledBuffer[i].g = 0; 
			ledBuffer[i].b = 25; 
			break;
		}
	}
}

//заполнение плавными переливами
void EChain::smoothChain()
{
	long timeNow;
	byte stepNumber;
	CRGB tmpCell;
	
	timeNow = millis();
	//получаем целую и дробную часть.
	//целая часть - номер периода
	//делим ее на 3 и определяем, каким цветом заполнять.
	//дробная часть - время внутри этапа. Сейчас не нужно.
	stepNumber = ( timeNow - startTime ) / transitionTime % 3;

	for ( int i = 0; i < numLeds; i++ ) {
//		ledBuffer[i].r = 25; 
		switch ( stepNumber ) {
		case 0: //red
			ledBuffer[i].r = MAX_LED_LEVEL; 
			ledBuffer[i].g = 0; 
			ledBuffer[i].b = 0; 
			break;
		case 1: //green
			ledBuffer[i].r = 0; 
			ledBuffer[i].g =MAX_LED_LEVEL; 
			ledBuffer[i].b = 0; 
			break;
		case 2: //blue
			ledBuffer[i].r = 0; 
			ledBuffer[i].g = 0; 
			ledBuffer[i].b = MAX_LED_LEVEL; 
			break;
		}
	}
}


void EChain::caterpillerChain()
//перемещение гусеницей - цвета меняются последовательно
{
	long timeNow;
	word stepNumber;
	word ledNumber;
    word stepPart;
	byte newColorValue, oldColorValue;

	CRGB tmpCell;
	
	timeNow = millis() - startTime;
	//получаем целую и дробную часть.
	//целая часть - номер периода
	//а взяв ее по модулю от длины цепочки получаем номер светодиода
    stepNumber = ( timeNow / transitionTime ) % 3;//according switch branch num
	stepPart   = timeNow % transitionTime;  //часть шага 0..transitionTime-1
   // ledNumber  = stepPart * numLeds / transitionTime;
	
	//ledNumber=1;
	//ledNumber = timeNow  % transitionTime % numLeds;

	//дробная часть - время внутри этапа.
   // stepPart = timeNow % transitionTime;
//  m= map (0, transitionTime, 0, 255 )!!!
	//определим текущие значения цвета
//    newColorValue = map (stepPart % numLeds, 0, transitionTime % numLeds, 0, 50);  
//    newColorValue = map (stepPart % numLeds, 0, transitionTime % numLeds, 0, 50);  
	ledNumber = map( stepPart, 0, transitionTime, 0, numLeds ); 
	
    newColorValue = constrain (map ( stepPart % (transitionTime / numLeds)+1, 
					0, transitionTime / numLeds, 0, MAX_LED_LEVEL), 0, MAX_LED_LEVEL);  
	//newColorValue = 50;
	oldColorValue = MAX_LED_LEVEL - newColorValue;
	//А теперь установим текущие значения цвета!
	switch ( stepNumber ) { //!!!правильный порядок изменения цветов!
	case 0: //red
		ledBuffer[ledNumber].r = newColorValue; 
		ledBuffer[ledNumber].g = 0; 
		ledBuffer[ledNumber].b = oldColorValue; 
		break;
	case 1: //green
		ledBuffer[ledNumber].r = oldColorValue; 
		ledBuffer[ledNumber].g = newColorValue; 
		ledBuffer[ledNumber].b = 0; 
		break;
	case 2: //blue
		ledBuffer[ledNumber].r = 0; 
//		ledBuffer[ledNumber].g = oldColorValue; 
//		ledBuffer[ledNumber].b = newColorValue; 
		ledBuffer[ledNumber].g = 0; 
		ledBuffer[ledNumber].b = 0; 
		break;
	}

//	if ( millis() % 5000 == 0 ) {
//	if ( tmpLedNumber != ledNumber ) {
	if (( tmpLedNumber != ledNumber ) || ( millis() % 5100 == 0 )) {
		tmpLedNumber = ledNumber;
		DBG_PRINTLN( "------------");
		DBG_PRINT( "caterpiller:timeNow: ");
		DBG_PRINTLN( timeNow );
		DBG_PRINT( "caterpiller:stepNumber: ");
		DBG_PRINTLN( stepNumber );
		DBG_PRINT( "caterpiller:stepPart: ");
		DBG_PRINTLN( stepPart );
		DBG_PRINT( "caterpiller:ledNumber: ");
		DBG_PRINTLN( ledNumber);
		DBG_PRINT( "caterpiller::newColorValue: ");
		DBG_PRINTLN( newColorValue );
		DBG_PRINTLN( "map ( stepPart % numLeds, 0, transitionTime / numLeds, 0, 50);");
		DBG_PRINT( "caterpiller::stepPart % numLeds: ");
		DBG_PRINTLN( stepPart % numLeds );
		DBG_PRINT( "caterpiller:: transitionTime / numLeds: ");
		DBG_PRINTLN( transitionTime / numLeds );
		DBG_PRINTLN();
	}

}







void HSV2RGB_Adv (unsigned char &r, unsigned char &g, unsigned char &b,
unsigned int hue, unsigned char sat, unsigned char val)
{
  if (!sat) { // Acromatic color (gray). Hue doesn't mind.
    r=g=b=val;
  } else  { 

    
    //hue = hue%384;
    uint8_t base = ((255 - sat) * val)>>8;

    switch(hue>>8) {
    case 0:
        r = val;
        g = (((val-base)*hue)/256)+base;
        b = base;
    break;

    case 1:
        r = (((val-base)*(256-(hue%256)))/256)+base;
        g = val;
        b = base;
    break;

    case 2:
        r = base;
        g = val;
        b = (((val-base)*(hue%256))/256)+base;
    break;

    case 3:
        r = base;
        g = (((val-base)*(256-(hue%256)))/256)+base;
        b = val;
    break;

    case 4:
        r = (((val-base)*(hue%256))/256)+base;
        g = base;
        b = val;
    break;

    case 5:
        r = val;
        g = base;
        b = (((val-base)*(256-(hue%256)))/256)+base;
    break;
    }
  }   
 } // HSV 2 RGB Binary





#ifdef DEBUG_ECHAIN
	#undef DBG_PRINTLN(a)
	#undef DBG_PRINT(a)
#endif