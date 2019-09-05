/*
Author: J. C. Holder (jc.holder@thoughtworks.com)
*/

#include "Keyboard.h"

const bool DEBUG = false;

struct KeyboardButton {
  int pin;
  int clicks;
  bool clicked;
  bool down;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay;
  int keycode;
  char debugLabel[50];
  int lastState;
};

KeyboardButton spacebar = {
  4, // pin
  0, // clicks
  false, //clicked
  false, // down
  0, // lastDebouncetime
  50, // debounceDelay
  ' ', // keycode
  "I'm the spacebar!", // debugLabel
  LOW // lastState
};

KeyboardButton enterButton = {
  8, // pin
  0, // clicks
  false, //clicked
  false, // down
  0, // lastDebouncetime
  50, // debounceDelay
  KEY_RETURN, // keycode
  "I'm the enter key!", // debugLabel
  LOW // lastState
};

KeyboardButton resetButton = {
  14, // pin
  0, // clicks
  false, //clicked
  false, // down
  0, // lastDebouncetime
  50, // debounceDelay
  KEY_END, // keycode
  "I'm the reset key!!!", // debugLabel
  LOW // lastState
};

void setupKeyboardButton(KeyboardButton button) {
  pinMode(button.pin, INPUT);
  digitalWrite(button.pin, HIGH);
}

KeyboardButton checkKeyboardButton(KeyboardButton button) {
  // limit the max key press output to avoid flooding the
  // computer with key presses while debugging
  if (DEBUG && button.clicks > 50) {
    return button;
  }
  
  int buttonState = digitalRead(button.pin);

  // TODO: How long can we run before we risk data overflow on
  // the timer? Is that a practical issue? Worst case: Just
  // reset the controller

  if (buttonState != button.lastState) {
    button.lastDebounceTime = millis();
    button.down = false;
    button.clicked = false;
  }

  if ((millis() - button.lastDebounceTime) > button.debounceDelay
      && buttonState == LOW) {
    button.down = true;
  }

  if (button.down && not button.clicked) {
    if (DEBUG) {
      // Only count clicks while debugging to avoid
      // integer overflow during production
      button.clicks++;
  
      Serial.print("You pressed the " );
      Serial.print(button.debugLabel);
      Serial.print(" button ");
      Serial.print(button.clicks);
      Serial.println(" times.");
      Serial.print("Keycode: ");
      Serial.print((char)button.keycode);
      Serial.println();
    }
    
    button.clicked = true;

    Keyboard.press(button.keycode);
    Keyboard.release(button.keycode);
  }

  return button;
}

void setup() {
  setupKeyboardButton(spacebar);
  setupKeyboardButton(enterButton);
  setupKeyboardButton(resetButton);

  Keyboard.begin();
}

void loop() {
  spacebar = checkKeyboardButton(spacebar);
  enterButton = checkKeyboardButton(enterButton);
  resetButton = checkKeyboardButton(resetButton);
}

/* Staging area for potential keypresses


*/
