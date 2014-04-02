/**
 *  Code for LCD Debugger
 **/
/*#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

byte index = 0;
char buffer[16]; // the buffer;
char inByte;
bool strComplete = false;

/**
 *  End of code for LCD Debugger
 **/

boolean debugMode = true;

int ledOutputs[7] = { 6, 6, 7, 8, 9, 10, 11 };
int digitalButtons[7] = { 2, 3, 4, 13, 12, A5, 5 }; // Button 1-6 + Sustain pedal
boolean digitalButtonsIsDepressed[7] = { false, false, false, false, false, false, false };
boolean sustainPedalIsDepressed = false;

// A4 is the input for the expression pedal

byte controlChangeNumber[7] = { 25, 26, 27, 28, 29, 30, 64 };
byte midiMessageForControlChange = 176;
byte midiChannel = 9;
byte controlChange = 0;

int midiBytes[3] = { 0, -1, -1 };

unsigned long lastAction = 0;
int holdFor = 150; // ms from one command to the next

// Calibration for the expression pedal
int expressionFloor = 59;
int expressionCeiling = 1020;
int lastMidiValue = 0;

void setup() {
  
  if ( debugMode ) {
    Serial.begin( 9600 );
  } else {
    Serial.begin( 31250 );
  }
  
  controlChange = midiMessageForControlChange + midiChannel - 1;
  
  for ( int i = 0; i < 8; i++ ) {
    pinMode( digitalButtons[ i ], INPUT );
  }
  
  for ( int i = 1; i < 8; i++ ) {
    pinMode( ledOutputs[ i ], OUTPUT );
    digitalWrite( ledOutputs[ i ], LOW );
  }
}

void loop() {
  
  if ( ( millis() - holdFor ) > lastAction ) { // If lastAction hasn't changed for 200 ms
    lastAction = 0; // Reset it
  }
  
  checkButtons();
  
  checkExpressionPedal();
  
  checkMidi();
  
}

/**
 * This function will meassure the input on the Analog pin
 * that the expression pedal is connected to.
 **/
void checkExpressionPedal() {
  int expressionValue = analogRead( A4 );
  
  //Serial.println( expressionValue );
  
  int midiValue = map( expressionValue, expressionFloor, expressionCeiling, 0, 128 );
  
  if ( midiValue > 127 ) {
    midiValue = 127;
  }
    
  if ( midiValue < 0 ) {
    midiValue = 0;
  }
  
  if ( midiValue != lastMidiValue ) {
    lastMidiValue = midiValue;
    
    if ( debugMode ) {
      Serial.println ( midiValue );
    } else {
      sendExpression( midiValue );
    }
    
  }
  
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
  for ( int i = 0; i < 7; i++ ) {
    
    // Digital
    if ( digitalRead( digitalButtons[ i ] ) == HIGH ) {
      if ( digitalButtonsIsDepressed[ i ] == false ) { // If the button isn't already registrered as pressed

        if ( lastAction == 0 ) { // This means nothing has happened for a while, so go ahead
        
          lastAction = millis();
        
          digitalButtonsIsDepressed[ i ] = true;
          
          if ( debugMode ) {
            Serial.print( i );
            Serial.println( " is currently being pressed.");
          } else {
            sendMidi( i, true );
          }
        }
      }
    } else {
      
      if ( digitalButtonsIsDepressed[ i ] ) { // The button was just released
      
        if ( lastAction == 0 ) { // This means nothing has happened for a while, so go ahead
        
          lastAction = millis();
      
          digitalButtonsIsDepressed[ i ] = false;
        
          if ( debugMode ) {
            Serial.print( i );
            Serial.println( " was just released.");
          } else {
            sendMidi( i, false );
          }
          
        }
      }
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

void sendExpression( int midiValue ) {
  
  Serial.write( controlChange );
  Serial.write( controlChangeNumber[ 31 ] );
  Serial.write( midiValue );
  
}

void checkMidi() {
  // Check to see if we have collected a full MIDI Message
  if ( ( midiBytes[ 0 ] != 0 ) && ( midiBytes[ 1 ] != -1 ) && ( midiBytes[ 2 ] != -1 ) ) {
    
    int myLed;
    
    if ( myLed = getLedIndex( midiBytes[ 1 ] ) ) { // If the CC number matches a LED
      if ( midiBytes[ 2 ] > 63 ) { // A HIGH value
        digitalWrite( ledOutputs[ myLed ], HIGH );
      } else {
        digitalWrite( ledOutputs[ myLed ], LOW );
      }
    }
    
    midiBytes[ 1 ] = -1;
    midiBytes[ 2 ] = -1;
  }
  
  if ( Serial.available() ) { // Serial data exists
    
    byte serialByte = Serial.read();
    
    if ( serialByte > 127 ) { // Status byte
      midiBytes[ 0 ] = serialByte;
    } else {
      // Make sure a status byte is present
      if ( midiBytes[ 0 ] != 0 ) {
        // Is the first parameter set?
        if ( midiBytes[ 1 ] != -1 ) { // Set the second parameter
          midiBytes[ 2 ] = serialByte;
        } else { // Set as the first parameter
          midiBytes[ 1 ] = serialByte;
        }
      }
    }
  }

}

int getLedIndex( byte val ) {
  switch ( val ) {
    case 25:
      return 1;
      break;
    case 26:
      return 2;
      break;
    case 27:
      return 3;
      break;
    case 28:
      return 4;
      break;
    case 29:
      return 5;
      break;
    case 30:
      return 6;
      break;
    default:
      return 0;
  }
}
