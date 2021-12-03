# EmbeddedLab7
Lab 7 of Embedded Systems - section 4. This lab was worked on by Seth Freni and Vincent Del Tufo.
## Lab7.c
This is the C code for the lab. The adc array records the values from a photoresistor and a thermristor. The raw ADC value is then converted into a string and transmitted using UART. The delay between each transmit needs to be at least 15 seconds so that the GUI from Form1.cs will be able to send the data to ThingSpeak correctly. The letter 'A' being transmitted first in the loop is used to set the 'flag_sensor' value in the GUI.
## Form1.cs
This is the code for the C# GUI. It sends the data from the two sensors to the desired ThingSpeak channel. The parts that are in brackets, such as `[channel id]` in line 66 should be replaced with the specific user's ThingSpeak values. The GUI should be layed out as below.

![GUI](https://i.gyazo.com/471488ebb19b856ed61bb2a92c925e0c.png)

'Serial start' corresponds to ``button1_Click``, which is what allows the GUI to send the data to ThingSpeak. 'Serial stop' corresponds to ``button1_Click`` and stops the GUI from sending data. The data that is received shows up in the textbox.
