#include "gameover.h"
#include "M5StickCPlus2.h"
#include "constants.h"
#include "joystick_utils.h"
#include "state.h"
#include <Arduino.h>

extern State currentState;

void gameOver() {
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  StickCP2.Display.drawString("Game Over", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 2);
  if (joystick_isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    currentState = INIT;
  }
}
