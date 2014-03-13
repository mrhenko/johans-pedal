# Johan's MIDI pedal #

This is the Arduino code for a MIDI pedal that I'm currently building for Johan Nordin, the keyboard player of Marigold. It is meant to be used with Apple MainStage.

## Usage with MainStage ##
* Connect MIDI out from the pedal to a MIDI in of your choice on your computers MIDI interface.
* Create the desired amout of buttons in MainStage (preferably 6).
* Map the footswitches to the buttons using the *Learn* function.
* Set the MainStage buttons *Type* to "Press / Release".
* Set *MIDI thru* to "Do not pass thru".
* Set *Send value* to a MIDI out port of your choice.
* Connect that output to the input of the pedal.
