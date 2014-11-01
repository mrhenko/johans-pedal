# Johan's MIDI pedal #

*Version: 1.1

This is the Arduino code for a MIDI pedal that I'm currently building for Johan Nordin, the keyboard player of Marigold. It is meant to be used with Apple MainStage.

## Usage with MainStage ##
* Connect MIDI out from the pedal to a MIDI in of your choice on your computers MIDI interface.
* Create the desired amout of buttons in MainStage (preferably 6).
* Map the footswitches to the buttons using the *Learn* function.
* Set the MainStage buttons *Type* to "Press / Release".
* Set *MIDI thru* to "Do not pass thru".
* Set *Send value* to a MIDI out port of your choice.
* Connect that output to the input of the pedal.


## Version history ##

### 1.1 ###
Button 4 & 5 alternates between sending CC with value 0 and value 127.
All other buttons send CC with value 127 on button press, value 0 on button release.
