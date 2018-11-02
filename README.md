View this project on [CADLAB.io](https://cadlab.io/project/1407). 

# Arduino-based telescope focuser controller
Simple MoonLite-compatible focuser controller. Thought to be used with INDI.
Does not support temperature compensation yet (feel free to fork!).
It also adds a function to the MoonLite protocol (non-official command) to
change the state of the Arduino's digital and PWM pins.

### Overview
This MoonLite focuser uses an Arduino (Nano, in my case) to control
a stepper motor that moves the focuser knob using a belt.
There are also MOSFETS (think of them like relays) that are able to
control dew heaters or fans to cool your primary mirror.

<img align="center" src="Docs/SCH.png"><br>

### AstroAllInOne
<a href="https://github.com/SquareBoot/AstroAllInOne">AstroAllInOne</a>
is this firmware's companion INDI driver that allows the end user to
change the state of **every** pin of the board (pay attention! **do not change the
state of the stepper motor driver's pins**). Refer to its documentation on
how to configure it. Without AstroAllInOne, the Arduino will behave just like
a generic MoonLite focuser.<br>

<img align="right" src="Docs/1.jpg" width="350" >

### Autodesk Eagle project
In the "Eagle" directory there is the full circuit project, both schematics and PCB. 
It's the one I use for my telescope, but of course you can modify it to 
accomplish your necessities: for instance, you could add another dew heater port, 
or remove the Newton mirror cooler MOSFET. Refer to the AstroAllInOne readme on 
how to define new digital or PWM pins definitions.
<br>**Made with Eagle 9.0.1 Premium**<br>

<img align="left" src="Brackets/Sky-Watcher Newton Crayford/3.jpg" width="300" >

### Mounting brackets
Also includes mounting brackets for the common SkyWatcher 
dual-speed Crayford focuser (made for my 200mm Newton OTA).
**AutoCAD 2016** project and STL 3D exported file.

### Forking & issues
Feel free to submit pull requests and open issues! Also new brackets are welcome!