# Smart Switch Interface

OK, it's an old project I used tou create a Touch+Toucless User Interface for controlling multichannle Light Switch. 
The original part included a *ZigBee CC2538 TI chip* managing to receive and send commands from nodes. It using UART to interface with Touch pannel.
The touc pannel that is also touchless pannel included a Atmel QTouch ring that controlled with **bold** ATMega88 and 4 IR sensors for touchless interface.
## Touch Interface
**QTouch** is wellknown and you can find more information about it in this link: https://www.microchip.com/mplab/avr-support/qtouch-tools

## Touchless Interface
I used 4 IR sensors to manage the interface. Each sensor sends IR beam and receive the reflect to detect if anything is in front of it.
the Receiver returns an Analog voltage used by ADC to read, then microcontroller can decide (based on adjasted sensitivity) to consider it should be sense as object or not.
I said **4 sensor**, lets talk about gestures I needed to detect and algorithm I used for that:

### Gestures
As we all used to conventional light switches, moving switch up to down for turning light ON and viceversa for turning OFF.
Instead of simple ON/OFF touch spotes (that are hard to find each time you want to turn a light on or off) that used on most of smart switches, I tried to make **easy gestures** just same as everybody used to, so used 4 IR sensors to detect *hand movement* infront of the touch plate.
in this picture you can see what Touch+Touchless sensors working in my invented Smart Switch

![Touch+Toucless interface](https://github.com/Behn4m/TouchUI/blob/master/SmartSwitch%20UI.png)
