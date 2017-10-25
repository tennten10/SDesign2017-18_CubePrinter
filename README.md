# SDesign2017-18_CubePrinter

#Code functions we will need to create:
1. Turn on/standby/off
2. LED control - on during layer printing, off during standby and off, off during print bed adjustments
3. Print bed motor control - interfacing with stepper/servo motor drivers and controlling position. Must keep coordinated with layer of the print.
4. LCD masking - Create inverse image of the object we are creating, and store X number of layers it will take to build. Maybe create a 2 dimensional array system to keep track of commands. 
5. Locked position (and mechanism?) for during launch and when the print is finished. 

(optional)6. If necessary due to pinout or memory, separate arduinos can be used for different systems and the clocks can be synced.
