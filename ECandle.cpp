#include "ECandle.h"

//==================================== class ECandle ======================
oid_t ECandle::init(port_t ledPort, uint16_t fadeinTimeout, uint16_t fadeoutTimeout, 
							uint8_t maxLevel, uint8_t minLevel)
{
	//сохраняем правильные состояни
//	this->isOn=false;
	this->port=ledPort;
	//инициируем порт в выключенном режиме
	pinMode(this->port,OUTPUT);
	this->setLevel(0);
	this->currentMaxLevel = maxLevel;
	this->currentMinLevel = minLevel;
	this->fadeinTimeout = fadeinTimeout*1000;
	this->fadeoutTimeout = fadeoutTimeout*1000;
	fadeinTimer.init(this->fadeinTimeout);
	fadeoutTimer.init(this->fadeoutTimeout);
	return getID();
};

void ECandle::setLevel(const int value)
{
	currentLevel=constrain(value,0,255);
	int i = this->currentLevel;
	analogWrite(this->port,this->currentLevel);
//#ifdef DEBUG_ECANDLE
//	Serial.print("ECandle::setCurrentLevel level=");
//	Serial.println(i);
//#endif	 
}; 

void ECandle::setMinLevel(const int value)
{
	 currentMinLevel=constrain(value,CANDLEMINIMALLIGHTLEVEL+1,
												CANDLEMAXIMALLIGHTLEVEL-1);
};

void ECandle::setMaxLevel(const int value)
{
	 currentMaxLevel=constrain(value,CANDLEMINIMALLIGHTLEVEL+1,
												CANDLEMAXIMALLIGHTLEVEL-1);
};

void ECandle::getName(char* result)
{
	sprintf(result,"ECandle: ID=%d port=%d reverseOn=%d",getID(),this->port,this->reverseOn);
};	


int ECandle::handleEvent(Event& tmpEvent)
{	
	
 //  Serial.println("!11");
	EOutputDevice::handleEvent(tmpEvent); //прокинем вниз на унаследованный обработчик
//	Serial.println("!22");
	if (eventForMe(tmpEvent)) {
//	if (1) {	
#ifdef DEBUG_ECANDLE
		Serial.println("ECandle::handleEvent(): Its my event!");
#endif
		switch (this->currentState) {
		case csOff:  //свет полностью включен
			if (tmpEvent.eventType==evTurnOn) {
				setState(csFadeIn);
			};
			break;
		
		case csFadeIn:
			if (tmpEvent.eventType==evTurnOff) {
				setState(csFadeOut);
			};
			if (tmpEvent.eventType==evTurnOn) {
				setState(csOn);
			};
			break;

		case csOn:
			if (tmpEvent.eventType==evTurnOff) {
				setState(csFadeOut);
			};
			break;

		case csFadeOut:
			if (tmpEvent.eventType==evTurnOn) {
				setState(csFadeIn);
			}; 
			if (tmpEvent.eventType==evTurnOff) {
				setState(csOff);
			}; 
			break;

		case csFlickering:
			break;

		};
		return 1;
	} // это было наше событие!
	else return 0;
};

void ECandle::setState(const CandleState newState)
//переключение в новое состояние
//начата 2010-02-14 ВЩ
{
#ifdef DEBUG_ECANDLE
	Serial.print("ECandle::setState() ID=");
	int tmp = getID();
	Serial.print(tmp);
	Serial.print(" new State=");
	Serial.println(newState);
#endif	
	switch (newState) {
		case csOn:
			this->currentState=csOn;
			on();
			break;
		case csOff:
			this->currentState=csOff;
			off();
			break;
		case csFadeIn: //?????
			this->currentState=csFadeIn;
			fadeIn();
			break;
		case csFadeOut:  //?????
			this->currentState=csFadeOut;
			fadeOut();
			break;		
		case csFlickering: //??????
			this->currentState=csFlickering;
			startFlickering();
			break;		
	} //switch newState		
};		


void ECandle::idle()
//процедура очередного такта
//пробег по состояниям, для затухания, разгорания и свечки
//нужно расчитать новое значение яркости
{
	int newLevel;
	switch (this->currentState) {
		case csFadeIn:
			if (fadeinTimer.expired()) {
				setState(csOn); //таймер истек, переходим в полное включение
			} else {
			  //расчитаем уровень света и зададим его!
//				newLevel=calcLightLevel(timeOfLastEvent,FadeInDelay,1,255);
				newLevel=calcLightLevel(fadeinTimer.getStartTime(),this->fadeinTimeout,1,255);
#ifdef DEBUG_ECANDLE
				Serial.print("ECandle::idle() newLevel=");				
				Serial.println(newLevel);
#endif				
				this->setLevel(newLevel);
			};	  
			break; 
		case csFadeOut:
			if (fadeoutTimer.expired()) {
				setState(csOff); //таймер истек, переходим в полное включение
			} else {
			  //расчитаем уровень света и зададим его!
//				newLevel=calcLightLevel(timeOfLastEvent,FadeInDelay,1,255);
				newLevel=calcLightLevel(fadeoutTimer.getStartTime(),this->fadeoutTimeout,255,1);
#ifdef DEBUG_ECANDLE
				Serial.print("ECandle::idle() newLevel=");				
				Serial.println(newLevel);
#endif				
				this->setLevel(newLevel);
			};	  
			break; 
	};
};

int ECandle::calcLightLevel(long startTime,long timeOut, int minLevel, int maxLevel)
/*
функция расчитывает и возвращает значение уровня света в данный момент времени
исходя из запомненного времени последнего события timeOfLastEvent
*/
{
	
#ifdef DEBUG_ECANDLE1
	Serial.print("calcLightLevel: startTime=");	
	Serial.print(startTime);
	Serial.print(", timeOut=");	
	Serial.print(timeOut);
	Serial.print(", minLevel");	
	Serial.print(minLevel);
	Serial.print(", maxLevel=");	
	Serial.print(maxLevel);
#endif	
	int tmp, res;
	int scale = 4;
	if (minLevel<maxLevel) { scale = -1 * scale;}; 
	tmp=map(millis(),startTime,startTime+timeOut,minLevel,maxLevel);
	if (minLevel<maxLevel) {
		tmp = constrain(tmp,minLevel,maxLevel);
	} else {
		tmp = constrain(tmp,maxLevel,minLevel);
	}
  //пересчитаем по плавающей логарифмической шкале
 //res = fscale(minLevel,maxLevel,minLevel,maxLevel,tmp,scale);
//	res=tmp;		
	res = fscale(startTime,startTime+timeOut,minLevel,maxLevel,millis(),scale); 
//  Serial.println(res);

#ifdef DEBUG_ECANDLE1
	Serial.print(", res=");
	Serial.println(res);
#endif
	return res;
};


void ECandle::on()
{
	setLevel(CANDLEMAXIMALLIGHTLEVEL);
#ifdef DEBUG_ECANDLE
	Serial.print("ECandle::on() ID=");
	int tmp = getID();
	Serial.println(tmp);
#endif	
};

void ECandle::fadeIn()
{
	unsigned long timeOfLastEvent;
	unsigned long now=millis();
	this->fadeinTimer.start();
	if (this->currentLevel>0) { //!!!???надо бы отладить получше, т.к. не согласуются уровни
  //уже горел свет, значит переход не из состояния Off, сдвинем дату последнего события
  // в прошлое, чтобы скомпенсировать долю времени, обсуловленную тем, что свет уже горит.
  //посчитаем условное время предыдущего события
  //правило: если текущее время совпадает с временем последнего события, значит затухание только началось 
  // и яркость должна быть максимальна, а последнее событие (время начала увеличения уровня)
  // нужно сдвинуть назад на FadeInDelay, есои впемя совпадает 
		//timeOfLastEvent=map(now,timeOfLastEvent,timeOfLastEvent+FadeOutDelay,now-FadeInDelay,now);
		timeOfLastEvent=map(now,
				this->fadeoutTimer.getStartTime(),this->fadeoutTimer.getStartTime()+this->fadeoutTimeout,
				now-this->fadeinTimeout,now);
  //на всякий случай ограничим время последнего события разумными рамками
		//timeOfLastEvent=constrain(timeOfLastEvent,now-FadeInDelay,now);
		timeOfLastEvent=constrain(timeOfLastEvent,now-this->fadeinTimeout,now);
		this->fadeinTimer.setStartTime(timeOfLastEvent);
	};
#ifdef DEBUG_ECANDLE
	Serial.print("ECandle::fadeIn() ID=");
	int tmp = getID();
	Serial.println(tmp);
#endif
};	

void ECandle::fadeOut()
{
	unsigned long timeOfLastEvent;
	unsigned long now=millis();
	this->fadeoutTimer.start();	
	if (this->currentLevel<this->currentMaxLevel) {
  //уже горел свет, значит переход не из состояния Off, сдвинем дату последнего события
  // в прошлое, чтобы скомпенсировать долю времени, обсуловленную тем, что свет уже горит.
  //посчитаем условное время предыдущего события
  //правило: если текущее время совпадает с временем последнего события, значит затухание только началось 
  // и яркость должна быть максимальна, а последнее событие (время начала увеличения уровня)
  // нужно сдвинуть назад на FadeInDelay, есои впемя совпадает 
		//timeOfLastEvent=map(now,timeOfLastEvent,timeOfLastEvent+FadeOutDelay,now-FadeInDelay,now);
		timeOfLastEvent=map(now,
				this->fadeinTimer.getStartTime(),this->fadeinTimer.getStartTime()+this->fadeinTimeout,
				now-this->fadeoutTimeout,now);
  //на всякий случай ограничим время последнего события разумными рамками
		//timeOfLastEvent=constrain(timeOfLastEvent,now-FadeInDelay,now);
		timeOfLastEvent=constrain(timeOfLastEvent,now-this->fadeoutTimeout,now);
		this->fadeoutTimer.setStartTime(timeOfLastEvent);
	};
	
#ifdef DEBUG_ECANDLE
	Serial.print("ECandle::fadeOut() ID=");
	int tmp = getID();
	Serial.println(tmp);
#endif	
};	


void ECandle::off()
{
#ifdef DEBUG_ECANDLE
	Serial.print("ECandle::fadeOff() ID=");
	int tmp = getID();
	Serial.println(tmp);
#endif	
	setLevel(0);
}; 

void ECandle::startFlickering()
{
#ifdef DEBUG_ECANDLE
	Serial.print("ECandle::fadeOff() ID=");
	int tmp = getID();
	Serial.println(tmp);
#endif	
	this->setLevel(0);
}; 


float ECandle::fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve)
/* Floating Point Autoscale Function V0.1
 Paul Badger 2007
 Modified from code by Greg Shakar

 This function will scale one set of floating point numbers (range) to another set of floating point numbers (range)
 It has a "curve" parameter so that it can be made to favor either the end of the output. (Logarithmic mapping)

 It takes 6 parameters

 originalMin - the minimum value of the original range - this MUST be less than origninalMax
 originalMax - the maximum value of the original range - this MUST be greater than orginalMin

 newBegin - the end of the new range which maps to orginalMin - it can be smaller, or larger, than newEnd, to facilitate inverting the ranges
 newEnd - the end of the new range which maps to originalMax  - it can be larger, or smaller, than newBegin, to facilitate inverting the ranges

 inputValue - the variable for input that will mapped to the given ranges, this variable is constrained to originaMin <= inputValue <= originalMax
 curve - curve is the curve which can be made to favor either end of the output scale in the mapping. Parameters are from -10 to 10 with 0 being
			 a linear mapping (which basically takes curve out of the equation)
*/
{

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
	Serial.println(curve * 100, DEC);	// multply by 100 to preserve resolution  
	Serial.println(); 
	*/

  // Check for out of range inputValues
  if (inputValue < originalMin) {
	 inputValue = originalMin;
  }
  if (inputValue > originalMax) {
	 inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){ 
	 NewRange = newEnd - newBegin;
  }
  else
  {
	 NewRange = newBegin - newEnd; 
	 invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;	// normalize to 0 - 1 float

 #ifdef DEBUG_ECANDLE
 /*
	Serial.print(OriginalRange, DEC);  
	Serial.print("	");  
	Serial.print(NewRange, DEC);  
	Serial.print("	");  
	Serial.println(zeroRefCurVal, DEC);  
	Serial.println();  
	*/
#endif
  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
  if (originalMin > originalMax ) {
	 return 0;
  }

  if (invFlag == 0){
	 rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else	  // invert the ranges
  {	
	 rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
  }

  return rangedValue;
};
