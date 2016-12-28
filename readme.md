#Insula 

https://www.youtube.com/watch?v=Azrm98Bf4nk

Insula is a wearable device that translates physiological events from sensor data (EEG, ECG, EMG, breath rate, hand gestures) into music in real-time. A heart beat turns into a drum beat, the rise and fall of a breath turns into the tension and release of a chord progression, etc. Here you can find the code that runs under the hood. 

The goal of Insula is to make biodata interesting, meaningful and useful to a diverse set of users. This technology allows users to become mindful of the processes in their bodies through the universal medium of music. I have created all custom hardware and software for the device, building on platforms including Arduino, ChucK, OpenBCI and Processing. 

I’ve separated Insula into two components: biological (v1), and tactile (v2). V1 consists of the electrocardiogram, electromyogram, and breath rate sensors, all of which were part of the [original thesis project] (https://drive.google.com/open?id=0B-Kp5PAG6589emdFNExwUmRqOG8)
that started Insula. V2 is a new component that introduces flex sensors on each finger and an accelerometer for more intentional control of the music. EEG processing, which ran on OpenBCI cannot be found here, but it was part of the original V1. Message me if you’re interested on that. I've redesigned Insula with modularity in mind, so sensors can be added in or removed based on the user's preference. I still need to update the ChucK code for modularity, though... 
 
The basic control flow of data is as follows: Data is fed in from the Arduino through Serial, and into ChucK. Sound files are triggered for some events (heart rate, breath), and parameters of sound oscillators are controlled for the flex sensors, accelerometer, and muscles. 
 
##Hardware Overview:
###**Microcontroller:** [Arduino Nano] (https://www.arduino.cc/en/Main/ArduinoBoardNano)
###**Sensors: Insula V1**
1. **Heart-rate monitor:** [Pulse Sensor] (http://pulsesensor.com/)

2. **Electromyogram:** [MyoWare] (http://www.advancertechnologies.com/p/myoware.html)

3. **Breath Rate Monitor:** Novel design. It consists of a [conductive rubber resistor] (https://www.adafruit.com/products/519) in line with a resistor to create a voltage divider. Optional capacitor across the sensor as an RC filter to tease out noise. Data is good, but needs to be cleaned via Arduino, and processed to spit out a boolean based on inhale/exhale. 

###Sensors: Insula V2
1. **[Flex Sensors] (https://www.adafruit.com/products/1070)** on each finger, in line with a voltage divider. Optional capacitor across the sensor as an RC filter to tease out noise. 

2. **[Accelerometer] (https://learn.sparkfun.com/tutorials/mma8452q-accelerometer-breakout-hookup-guide)**

###Bluetooth 
A  [Wireless Bluetooth Transceiver] (https://www.amazon.com/gp/product/B0093XAV4U/ref=ya_st_dp_summary) is hooked up to the TX/RX pins of the Arduino and provides Serial communication to the laptop where ChucK lives. 

###Schematic: 
The circuit is pretty straight forward, as most of the sensors are plug and play (voltage, ground, and analog out). The Arduino Nano allows for 8 analog inputs, so after accounting for the flex sensors and accelerometer you can add in two additional sensors from V1. I think EMG and breath rate sound best. Just make sure to adjust the arduino code to print the correct values to serial. If you are using a different arduino, be cautious of lining up SDA and SCL communication pins correctly. See the arduino site for specifications for your board. 

![schematic](/documentation/insulaSchematic_2.png)

##Software Overview: 
###Arduino: 
Each sensor feeds data into analog inputs on the Arduino, and smoothing/signal processing algorithms, and rate-of-change algorithms are applied as need be. Each sensor can be turned on or off by boolean controls at the top of the main sketch. Insula V1 sensors are also modular with V2. 

###ChucK 
[ChucK] (http://chuck.cs.princeton.edu/) is a  programming language for real-time sound synthesis and music creation. I chose ChucK because you can easily generate Ugens, MIDI, and sound oscillators, it is cross-platform, and it allows for super quick experimentation. A few cons are that it is syntactically confusing (similar to C with stupid anomalies) and it is not well documented. 

###Python 
A quick python script is used to record data and save as a .csv. This can be used for graphing in Excel for testing, or you can optimize the file for ChucK, so you can experiment with creating music without needing the hardware on you (wires and circuits can be a bit off putting in the wrong circles of coffee shops). 

##Conclusion: 

###Future: 
I am hoping to integrate a Processing GUI, so that users can select notes and instruments and a Processing visualizer, probably using an OpenSoundControl setup. Making the glove look less like an explosive would be nice, too. 

The name Insula comes from the insular cortex of the brain, which is responsible for helping us recognize our own biology. Your insula is what allows you to feel your heartbeat echo through your body when you’re excited, and what makes your feel thirsty after a long run. And that’s why Insula was built: as an avenue to recognize, respond to, and think more creatively and holistically about our own biology.
