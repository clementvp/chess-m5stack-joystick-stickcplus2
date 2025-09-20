#include "chooseColor.h"
#include "constants.h"
#include "joystick_utils.h"
#include "mcu-max.h"
#include "state.h"
#include <M5StickCPlus2.h>

extern int selectedColor;
extern State currentState;

void chooseColor() {
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  StickCP2.Display.drawString("Choose Color", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 5);

  StickCP2.Display.setTextSize(TEXT_SIZE_MEDIUM);
  StickCP2.Display.drawCenterString("White", StickCP2.Display.width() / 4,
                                    StickCP2.Display.height() / 2);
  StickCP2.Display.drawCenterString("Black", 3 * StickCP2.Display.width() / 4,
                                    StickCP2.Display.height() / 2);
  if (selectedColor == MCUMAX_BOARD_WHITE) {
    StickCP2.Display.drawRect(StickCP2.Display.width() / 4 - 40,
                              StickCP2.Display.height() / 2 - 20, 80, 60,
                              WHITE);
  } else {
    StickCP2.Display.drawRect(3 * StickCP2.Display.width() / 4 - 40,
                              StickCP2.Display.height() / 2 - 20, 80, 60,
                              WHITE);
  }

  if (joystick_isLeft()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    selectedColor = MCUMAX_BOARD_WHITE;
    StickCP2.Display.clear();
  }
  if (joystick_isRight()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    selectedColor = MCUMAX_BOARD_BLACK;
    StickCP2.Display.clear();
  }

  if (joystick_isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    currentState = CHOOSEDIFFICULTY;
  }
}
