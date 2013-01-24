/*
Test_EButton

*/

//#include <Arduino.h>
//#include <EAButton.h>
//#include <EAEventButton.h>
//#include <EBeeper.h>
#include <EButton.h>
//#include <ERGBLED.h>
#include <Events.h>
#include <EApplication.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define DEBUG_TEST_EBUTTON

//Inputs
//Digital Inputs
#define STATE_BUTTON_PIN      2

#define evStateButton         131

enum BoxState {
        bsLightOff,
        bsLowLight,
        bsFullLight,
        bsMotionDetected
};
 


//=====================================================================
class MyApplication : public EApplication {
public:
	void init();
	void parseEvent();

        void setLightOff();
        void setLowLight();
        void setFullLight();
        void setMotionDetected();
        void setNextState();
        
        oid_t stateButtonID;

        EButton stateButton;
private:
        BoxState currentState;
};
        
void MyApplication::init()
{

        stateButtonID   = stateButton.init( STATE_BUTTON_PIN, true);   
            stateButton.setEvents( evStateButton );

	addObject( &stateButton );

        currentState = bsLightOff;
        
        delay(1000);

};

   

void MyApplication::parseEvent()
{
        switch ( currentEvent.eventType ){
	case evStateButton:
                setNextState();
                break;
	}
};


void MyApplication::setNextState()
{
#ifdef DEBUG_TEST_EBUTTON
        Serial.print("Change mode from ");
        Serial.println(currentState);
#endif          
        switch ( currentState ) {
        case bsLightOff: 
                setLowLight();
                break;
        case bsLowLight:
                setFullLight();
                break;
        case bsFullLight:
                setLightOff();        
                break;
        case bsMotionDetected:
                setFullLight();
                break;
        }        
}

/*
void MyApplication::switchToState(BoxState newState)
{
          
}
*/

void MyApplication::setLightOff()
{
        currentState = bsLightOff;
        Serial.println(" bsLightOff");
};

void MyApplication::setLowLight()
{
        currentState = bsLowLight;
        Serial.println(" bsLowLight");
};

void MyApplication::setFullLight()
{
        currentState = bsFullLight;
        Serial.println(" bsFullLight");
};

void MyApplication::setMotionDetected()
{
        currentState = bsMotionDetected;
        Serial.println(" bsMotionDetected");
};



//====================================================END OF MyApp definition
MyApplication mainApp;

void setup()
{
	Serial.begin(9600);
	Serial.println("Test_EButton Loading..");
 
	mainApp.init();
	Serial.println("Current objects:");
	mainApp.printNames();
	Serial.println("Loading done!");

};


void loop()
{ 
	if (mainApp.getEvent()) {  
#ifdef DEBUG_TEST_EBUTTON
		mainApp.printEvent();
#endif
		mainApp.parseEvent();
		mainApp.handleEvent();
	}
	mainApp.idle();
//delay ( 500);
};

    



 

