Events
======

Event-driven framework for Arduino

Objects:
--------
  - Timer
  - Event
  - EventStack
  - Object
  - Application
  
  
Input devices:
--------------
  - EDevice (EObject) - Abstract device connected to Arduino port
  - EAnalogInput (EDevice)
  - EInputDevice (EDevice) - General digital input device
  - EButton (EInputDevice) - Button with event generation functionality
  - Analog button
  - EThermo (EInputDevice) - abstract Thermometer
  - EThermoDallas1820 (EThermo) Thermometer on Dallas 18B20 chip

Output Devices:
---------------
  - EOutputDevice (EDevice) - Digital output device, connected to arduino digital port
  - EBeeper (EOutputDevice) - Beeper with beeper delay 
  - ELED (EOutputDevice) - LED, connected to arduino digital ports in on/off mode, not PWM!
  - ECandle (EOutputDevice) - Candle imitaion, pwm, flickering effect based on level changing


  
  
