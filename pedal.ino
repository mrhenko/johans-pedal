int incomingMidiMessage;

int digitalButtons[3] = { 2, 3, 4 };
int analogButtons[3] = { 3, 4, 5 };
boolean digitalButtonsIsDepressed[3] = { false, false, false };

int buttonValue = 0;

byte midiChannel = 1001; // Channel 9

void setup() {
  //Serial.begin( 31250 ); // MIDI Baud Rate
  Serial.begin( 9600 );
  
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
        Serial.print( i );
        Serial.println( " is currently being pressed.");
      }
    } else {
      
      if ( digitalButtonsIsDepressed[ i ] ) { // The button was just released
        digitalButtonsIsDepressed[ i ] = false;
        Serial.print( i );
        Serial.println( " was just released.");
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
