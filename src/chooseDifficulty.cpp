#include "chooseDifficulty.h"
#include "constants.h"
#include "joystick_utils.h"
#include "state.h"
#include <M5StickCPlus2.h>

extern int currentIndexDifficulty;
extern int mcu_nodes;
extern int mcu_depth;
extern State currentState;

void chooseDifficulty() {
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  StickCP2.Display.drawString("Choose Difficulty", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 5);
  StickCP2.Display.setTextSize(TEXT_SIZE_MEDIUM);
  StickCP2.Display.drawCenterString(
      String("Level: ") +
          String(difficulty_levels[currentIndexDifficulty].level),
      StickCP2.Display.width() / 2, StickCP2.Display.height() / 2);

  if (joystick_isDown()) {
    if (currentIndexDifficulty > 0) {
      StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
      currentIndexDifficulty--;
      StickCP2.Display.clear();
    }
  }
  if (joystick_isUp()) {
    if (currentIndexDifficulty < 24) {
      StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
      currentIndexDifficulty++;
      StickCP2.Display.clear();
    }
  }
  if (joystick_isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    mcu_nodes = difficulty_levels[currentIndexDifficulty].node_max;
    mcu_depth = difficulty_levels[currentIndexDifficulty].depth_max;
    currentState = INGAME;
  }
}
