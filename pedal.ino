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

boolean debugMode = false;

int ledOutputs[6] = { 6, 7, 8, 9, 10, 11 };
int digitalButtons[7] = { 2, 3, 4, 13, 12, A5, 5 }; // Button 1-6 + Sustain pedal
boolean digitalButtonsIsDepressed[7] = { false, false, false, false, false, false, false };
boolean sustainPedalIsDepressed = false;

byte controlChangeNumber[7] = { 25, 26, 27, 28, 29, 30, 64 };
byte midiMessageForControlChange = 176;
byte midiChannel = 9;
byte controlChange = 0;

byte commandByte = 0;
byte midiParameters[2] = { -1, -1 };

unsigned long lastAction = 0;
int holdFor = 150; // ms from one command to the next

void setup() {
  
  if ( debugMode ) {
    Serial.begin( 9600 );
  } else {
    Serial.begin( 31250 );
  }
  
  controlChange = midiMessageForControlChange + midiChannel - 1;
  
  for ( int i = 0; i < 7; i++ ) {
    pinMode( digitalButtons[ i ], INPUT );
  }
  
  for ( int i = 0; i < 7; i++ ) {
    pinMode( ledOutputs[ i ], OUTPUT );
    digitalWrite( ledOutputs[ i ], LOW );
  }

  /**
   *  Code for LCD Debugger
   **/
 /* lcd.begin( 16, 2 );
  lcd.backlight();
  lcd.clear();
  lcd.setCursor( 0, 0 );
  lcd.print( "Startar..." );
  lcd.blink();
  /**
   *  End of code for LCD Debugger
   **/

}

void loop() {
  
  if ( ( millis() - holdFor ) > lastAction ) { // If lastAction hasn't changed for 200 ms
    lastAction = 0; // Reset it
  }
  
  checkButtons();
  
  checkMidi();
  
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
        
         /* lcd.clear();
          lcd.print( i );*/
          
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
        
          /*  lcd.clear(); */
          
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

void checkMidi() {
  // Check if we have any serial data.
  if ( Serial.available() ) {
    
    //digitalWrite( ledOutputs[ 0 ], HIGH );
    
    if ( commandByte == 0 ) {
      // Read first byte (command).
      commandByte = Serial.read();
    }
  }

  if ( Serial.available() ) {
    // Just listen for note on and off.
    if ( commandByte == controlChange ) {
      
      //digitalWrite( ledOutputs[ 1 ], HIGH );
      
      if ( midiParameters[ 0 ] == -1 ) {
        midiParameters[ 0 ] = Serial.read();
        
        //digitalWrite( ledOutputs[ 2 ], HIGH );
        
      } else {
        midiParameters[ 1 ] = Serial.read();
        
        //digitalWrite( ledOutputs[ 3 ], HIGH );
        
      }
      
      if ( ( midiParameters[ 0 ] != -1 ) && ( midiParameters[ 1 ] != -1 ) ) {
        // A complete message has been received.
        
        //digitalWrite( ledOutputs[ 4 ], HIGH );
        
        int myPin = ledOutputs[ getLedIndex( midiParameters[ 0 ] ) ];
        
        /**** HERE IS TROUBLE!!! ****
        ** I need to use the LCD and a prototype to log what's
         * actually stored in midiParameters[ 0 ]
        **/     
        if ( midiParameters[ 1 ] > 63 ) { // HIGH "velocity"
          digitalWrite( myPin, HIGH );
        } else {
          digitalWrite( myPin, LOW );
        }
        
        // Reset
        commandByte = 0;
        midiParameters[ 0 ] = -1;
        midiParameters[ 1 ] = -1;
      }
      
      /*byte values[ 2 ];
      getBytes( 2, values );        
      
      changeIndicatorLed( getLed( values[ 0 ] ), values[ 1 ] );
      commandByte = 0;*/
      
    } else {
      commandByte = 0;
    }
  }
}


/*void getBytes( int expectedBytes, byte savedBytes[] ) {
  int receivedBytes = 0;

  do {
    if ( Serial.available() ) {
      savedBytes[ receivedBytes ] = Serial.read();
      receivedBytes++;
    }
  } 
  while( receivedBytes < expectedBytes );
}*/


int getLedIndex( byte val ) {
  switch ( val ) {
    case 25:
      return 0;
      break;
    case 26:
      return 1;
      break;
    case 27:
      return 2;
      break;
    case 28:
      return 3;
      break;
    case 29:
      return 4;
      break;
    case 30:
      return 5;
      break;
    default:
      digitalWrite( ledOutputs[ 5 ], HIGH );
  }
}
/*
void changeIndicatorLed( int led, byte val ) {
  if ( led ) {
    if ( val == 0 ) { // For some reason I never end up here.
      digitalWrite( led, LOW );
    } else if ( val == 127 ) {
      digitalWrite( led, HIGH );
    }
  }
}*/
