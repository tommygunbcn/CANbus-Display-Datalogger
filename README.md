# CANbus-Display-Datalogger
A Display for an Electric Vehicle that uses CAN bus messaging. Also , there is the possibility of sending via UART some values, making this an inexpensive and interesting tool for developement.

So, Do you need to visualize data flowing in your CAN bus?

I needed to, so I built this simple Display with this elements:

- arduino UNO board ( actually I used seeeduino because of the micro USB connector)
- Display used in this project is the 2,8" TFT touch shield, v2.1 by seeed studio
- Can BUS shield v1.2 by seeed studio
- A small joystick like this, which is "clickable":

![Image](https://github.com/tommygunbcn/CANbus-Display-Datalogger/blob/main/images/joystick.jpg)

The seeeduino and the shields match perfectly and are enclosed in a box in the handlebar, so data can be seen while riding:

![Image](https://github.com/tommygunbcn/CANbus-Display-Datalogger/blob/main/images/IMG_20201013_171731.jpg)


As the project is meant for an e-bike, the joystick is used to navigate between screens, instead of the touchscreen capabilities of the screen, for practical reasons.
To navigate between the screen, move the joystick left or right.

Basically, each screen consists in this elements:
- a big number, showing the speed of the bike. present in each screen
- a title of the screen (in blue)
- five lines of info, with the variable name and the variable value
- A small strip in the bottom, showing some warning/error info

![Image](https://github.com/tommygunbcn/CANbus-Display-Datalogger/blob/main/images/IMG_20201014_125320.jpg)
![Image](https://github.com/tommygunbcn/CANbus-Display-Datalogger/blob/main/images/IMG_20201014_181310.jpg)
![Image](https://github.com/tommygunbcn/CANbus-Display-Datalogger/blob/main/images/IMG_20201014_181351.jpg)


Additionally:
- There is the possibility to build a blank  screen in which some text will be  shown ( useful in case of error/warning situations)
- There is the possibility to build a screen in which its parameters can be edited. ( This implies that the arduino will send a configuration message to a node. You need to know  which is the protocol to modify values via CAN in your node)
- There is the possibility of sending via UART frames containing info, in cyclic way.  This is specially interesting for datalogger purposes.

I hope it can be as useful for you as it is for me :)


