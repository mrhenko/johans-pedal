/**
 *  Code for LCD Debugger
 **/
#include <Wire.h>
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


int digitalButtons[7] = { 2, 3, 4, 13, 12, A5, 5 }; // Button 1-6 + Sustain pedal
//char analogButtons[3] = { "A7", "A6", "A5" };
boolean digitalButtonsIsDepressed[7] = { false, false, false, false, false, false, false };
//boolean analogButtonsIsDepressed[3] = { false, false, false };
boolean sustainPedalIsDepressed = false;

byte controlChangeNumber[7] = { 25, 26, 27, 28, 29, 30, 64 };
byte midiMessageForControlChange = 176;
byte midiChannel = 9;
byte controlChange = 0;

byte commandByte = 0;

void setup() {
  Serial.begin( 31250 ); // MIDI Baud Rate
  //Serial.begin( 9600 );
  
  controlChange = midiMessageForControlChange + midiChannel - 1;
  //Serial.println( controlChange );
  
  for ( int i = 0; i < 7; i++ ) {
    pinMode( digitalButtons[ i ], INPUT );
  }
  
/*  pinMode( A7, INPUT );
  pinMode( A6, INPUT );
  pinMode( A5, INPUT );
  */
  /*pinMode( led1, OUTPUT );
  digitalWrite( led1, LOW );
  pinMode( led2, OUTPUT );
  digitalWrite( led2, LOW );  */

  /**
   *  Code for LCD Debugger
   **/
  lcd.begin( 16, 2 );
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
  
  checkButtons();
  
  //checkMidi();
  
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
        digitalButtonsIsDepressed[ i ] = true;
        
        sendMidi( i, true );
        
        lcd.clear();
        lcd.print( i );
        /*Serial.print( i );
        Serial.println( " is currently being pressed.");*/
      }
    } else {
      
      if ( digitalButtonsIsDepressed[ i ] ) { // The button was just released
        digitalButtonsIsDepressed[ i ] = false;
        
        if ( i == 6 ) {
          sendMidi( i, false );
        }
        
        lcd.clear();
        /*Serial.print( i );
        Serial.println( " was just released.");*/
      }
    }
    
    // Analog
    /*buttonValue = analogRead( analogButtons[ i ] );
    if ( buttonValue > 1000 ) {
     // Serial.println( analogButtons[ i ] );
     // Serial.println( buttonValue);
    }*/
  }
  
  // Analog
  /*buttonValue = analogRead( A7 );
  if ( buttonValue > 1000 ) {
     // Serial.println( analogButtons[ i ] );
     // Serial.println( buttonValue);
     analogButtonsIsDepress[ 0 ] = true;
     sendMidi
  }*/
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

/*void checkMidi() {
  // Check if we have any serial data.
  if ( Serial.available() ) {
    if ( commandByte == 0 ) {
      // Read first byte (command).
      commandByte = Serial.read();
    }
  }

  if ( Serial.available() ) {
    // Just listen for note on and off.
    if ( commandByte == controlChange ) {
      byte values[ 2 ];
      getBytes( 2, values );        
      
      changeIndicatorLed( getLed( values[ 0 ] ), values[ 1 ] );
      commandByte = 0;
      
    }
  }
}*§/

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


/*int getLed( byte val ) {
  switch ( val ) {
    case 25:
      return led1;
      break;
    case 26:
      return led2;
      break;
    default:
      return 0;
  }
}*/
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
