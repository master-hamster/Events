//
#ifndef ECandle_h
#define ECandle_h
#include "EDevice.h"


//светодиод с возможностью изменения яркости
//и эффектом свечи
class ECandle : public EOutputDevice {
public:
 //  LEDCandle(int value);
	oid_t init(port_t ledPort,	  //порт
	uint16_t fadeinTimeout = 5,  
	uint16_t fadeoutTimeout = 15, 
	uint8_t maxLevel = CANDLEMAXIMALLIGHTLEVEL, //минимальный уровень
	uint8_t minLevel = CANDLEMINIMALLIGHTLEVEL);//максимальный уровень
	virtual void getName( char* result );
//  void gracefullyOn(){};
//  void gracefullyOff(){};
	virtual int handleEvent( Event& tmpEvent );
	void idle();
//  void liteFire(){};
//  void setDelta(int value){};
	static float fscale( float originalMin, float originalMax,  //расчет уровня сигнала
	float newBegin, float newEnd,
	float inputValue, float curve );
	static int calcLightLevel( long startTime,long timeOut, //
	int minLevel, int maxLevel );
private:
	void setLevel( const int value );
	void on();
	void off();
	void fadeIn();
	void fadeOut();
	void startFlickering();
	void setState( const CandleState newState );
	void setMaxLevel( const int value );
	void setMinLevel( const int value );
//  int getMaxLevel(){};
//  int getMinLevel(){};
//  int getCurrentLevel(){};

	uint8_t currentLevel;				// 0 - candle is off
	CandleState currentState;			// текущее состояние
	uint8_t currentMaxLevel;			//максимальный уровень свечи, 255 по умолчанию
	uint8_t currentMinLevel;			//минимальный уровень свечи, 4 по умолчанию
	Timer fadeinTimer;					//таймер разгорания
	Timer fadeoutTimer;					//таймер разгорания
	unsigned long fadeinTimeout;		// время потухания
	unsigned long fadeoutTimeout;		// время загорания
	unsigned long timeOfLastEvent;	 //время последнего включения/выключения
};


#endif

