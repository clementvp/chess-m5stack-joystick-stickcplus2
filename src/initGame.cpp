#include "initGame.h"
#include "constants.h"
#include "joystick_utils.h"

#include "state.h"
#include <M5StickCPlus2.h>
#include <mcu-max.h>

extern State currentState;

void initGame() {
  mcumax_init();
  StickCP2.Display.drawString("M5 Chess", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 3);
  StickCP2.Display.setTextSize(TEXT_SIZE_SMALL);
  StickCP2.Display.drawString("Press BTN Joystick for new game",
                              StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.setTextSize(TEXT_SIZE_MEDIUM);
  if (joystick_isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    currentState = CHOOSECOLOR;
  }
}
