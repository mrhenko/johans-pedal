int incomingMidiMessage;

int digitalButtons[4] = { 2, 3, 4, 5 };
int analogButtons[3] = { 3, 4, 5 };
boolean digitalButtonsIsDepressed[3] = { false, false, false, false };

byte controlChangeNumber[6] = { 25, 26, 27, 64, 28, 29, 30 };
byte midiMessageForControlChange = 176;
byte midiChannel = 9;
byte controlChange = 0; 

int buttonValue = 0;

void setup() {
  Serial.begin( 31250 ); // MIDI Baud Rate
  //Serial.begin( 9600 );
  
  controlChange = midiMessageForControlChange + midiChannel - 1;
  Serial.println( controlChange );
  
  for ( int i = 0; i < 3; i++ ) {
    pinMode( digitalButtons[ i ], INPUT );
  }
  
}

void loop() {
  
  checkButtons();
  
  /* MIDI Thru */
/*  if ( Serial.available() ) {
    incomingMidiMessage = Serial.read();
    
    Serial.write( incomingMidiMessage );
  }*/
  /* End of MIDI Thru */
  
}

/**
 * This functions loops over the arrays containing the
 * inputs (digital and analog) used for buttons to see
 * what or which buttons are pressed.
 *
 * Any pressed button will get a TRUE value in its corresponding
 * place in the digitalButtonsIsDepressed array. Buttons not
 * being pressed will get a false.
 **/
void checkButtons() {
  for ( int i = 0; i < 3; i++ ) {
    
    // Digital
    if ( digitalRead( digitalButtons[ i ] ) == HIGH ) {
      if ( digitalButtonsIsDepressed[ i ] == false ) { // If the button isn't already registrered as pressed
        digitalButtonsIsDepressed[ i ] = true;
        
        sendMidi( i, true );
        
        /*Serial.print( i );
        Serial.println( " is currently being pressed.");*/
      }
    } else {
      
      if ( digitalButtonsIsDepressed[ i ] ) { // The button was just released
        digitalButtonsIsDepressed[ i ] = false;
        
        sendMidi( i, false );
        
        /*Serial.print( i );
        Serial.println( " was just released.");*/
      }
    }
    
    // Analog
    buttonValue = analogRead( analogButtons[ i ] );
    if ( buttonValue > 1000 ) {
     // Serial.println( analogButtons[ i ] );
     // Serial.println( buttonValue);
    }
  }
}

void sendMidi( int buttonNumber, boolean isOn ) {
  
  Serial.write( controlChange );
  Serial.write( controlChangeNumber[ buttonNumber ] );
  
  if ( isOn ) {
    Serial.write( 127 );
  } else {
    Serial.write( 0 );
  }
  
}
