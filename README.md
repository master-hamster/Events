Events
======

Event-driven framework for Arduino

Objects:
--------
  - Timer with example
  - Event
  - EventStack
  - Object
  - Application
  
  
Input devices:
--------------
  - ETimer (EObject) - timer object with event generation, with example
  - EDevice (EObject) - Abstract device connected to Arduino port
  -   EAnalogInput (EDevice)
  -   EInputDevice (EDevice) - General digital input device
  -     EButton (EInputDevice) - Button with event generation functionality, with example
  -     ESonic(EInputDevice) - Ultrasonic distance sensor
  -       ESonicRange(ESonic) - Ultrasonic sensor, rise event when something is in range
  -   EThermo (EInputDevice) - abstract Thermometer
  -     EThermoDallas1820 (EThermo) Thermometer on Dallas 18B20 chip, with two examples
  - Analog button

Output Devices:
---------------
  - EOutputDevice (EDevice) - Digital output device, connected to arduino digital port
  - EBeeper (EOutputDevice) - Beeper with beeper delay 
  - ELED (EOutputDevice) - LED, connected to arduino digital ports in on/off mode, not PWM!
  - ECandle (EOutputDevice) - Candle imitaion, pwm, flickering effect based on level changing

In active development, i don't recommend to use it now.
  
  
