#define DEBUG_EVENT 

#ifdef DEBUG_EVENT
//#define PRINTNAMES
//#define DEBUG_EOBJECT
//#define DEBUG_EVENTNAMES
//#define DEBUG_EVENTSTACK
//#define DEBUG_EINPUTDEVICE
//#define DEBUG_EAPPLICATION
//#define DEBUG_EBEEPER
//#define DEBUG_ETIMER
//#define DEBUG_EANALOGINPUT
#define DEBUG_EBUTTON
//#define DEBUG_EDEVICE
//#define DEBUG_ELED
//#define DEBUG_ERGBLED
//#define DEBUG_ECANDLE
//#define DEBUG_ECANDLE1
//#define DEBUG_ETHERMO
//#define ESONIC
//
#define DEBUG_ERROR 
#endif
#define ERR_PRINTLN(a) Serial.println(a)
#define ERR_PRINT(a) Serial.print(a)

