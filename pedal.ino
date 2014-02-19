int digitalButtons[3] = { 2, 3, 4 };
int analogButtons[3] = { 3, 4, 5 };
boolean digitalButtonsIsDepressed[3] = { false, false, false };

byte controlChangeNumber[6] = { 25, 26, 27, 28, 29, 30 };
byte midiMessageForControlChange = 176;
byte midiChannel = 9;
byte controlChange = 0;

boolean isControlChange = false;
byte midiParameters[2] = { -1, -1 };

int buttonValue = 0;

int led1 = 6;
int led2 = 7;

void setup() {
  Serial.begin( 31250 ); // MIDI Baud Rate
  //Serial.begin( 9600 );
  
  controlChange = midiMessageForControlChange + midiChannel - 1;
  //Serial.println( controlChange );
  
  for ( int i = 0; i < 3; i++ ) {
    pinMode( digitalButtons[ i ], INPUT );
  }
  
  pinMode( led1, OUTPUT );
  digitalWrite( led1, LOW );
  pinMode( led2, OUTPUT );
  digitalWrite( led2, LOW );  
  pinMode( 8, OUTPUT );
  digitalWrite( 8, LOW ); 
}

void loop() {
  
  checkButtons();
  
  checkMidiForCC();
  
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
        
        //sendMidi( i, false );
        
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

int checkMidiForCC() {
  if ( Serial.available() ) {
    
    // Read the first serial message
    int newMessage = Serial.read();
    
    // Check if it's any of out CCs
    if ( isControlChange ) {
      if ( midiParameters[0] == -1) {
        midiParameters[0] = newMessage;
      } else {
        midiParameters[1] = newMessage;
        int tempLed = matchMidiCCToLed();
        if ( tempLed > 0 ) {
          changeIndicatorLed( tempLed );
        }
          isControlChange = false;
          midiParameters[0] = -1;
          midiParameters[1] = -1;
      }
    } else if ( newMessage == controlChange ) {
      isControlChange = true;
      return 0;
    } else {
      return 0;
    }
    
  }
}

int matchMidiCCToLed() {
  switch ( midiParameters[0] ) {
    case 25:
      return led1;
      break;
    case 26:
      return led2;
      break;
    default:
      //return 8;
      return 0;
  }
}

void changeIndicatorLed( int led ) {
  if ( midiParameters[1] == 0 ) {
    digitalWrite( led, LOW );
  } else if ( midiParameters[1] == 127 ) {
    digitalWrite( led, HIGH );
  }
}
