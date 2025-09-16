#include "M5StickCPlus2.h"
#include "Wire.h"
#include "constants.h"
#include "move_utils.h"
#include "square_utils.h"
#include <Arduino.h>
#include <mcu-max.h>

// Constantes pour l'adresse du joystick et le seuil
#define JOY_ADDR 0x38

int8_t x_data, y_data, button_data;
int8_t prev_x = 0, prev_y = 0, prev_button = 1;

bool isUp() { return (x_data < -JOY_THRESHOLD && prev_x >= -JOY_THRESHOLD); }

bool isDown() { return (x_data > JOY_THRESHOLD && prev_x <= JOY_THRESHOLD); }

bool isRight() { return (y_data > JOY_THRESHOLD && prev_y <= JOY_THRESHOLD); }

bool isLeft() { return (y_data < -JOY_THRESHOLD && prev_y >= -JOY_THRESHOLD); }

bool isButtonPressed() { return (button_data == 0 && prev_button == 1); }

enum ColorSelection { MCUMAX_BOARD_WHITE, MCUMAX_BOARD_BLACK };

int currentIndexDifficulty = 0;

int mcu_nodes = difficulty_levels[0].node_max;
int mcu_depth = difficulty_levels[0].depth_max;

enum State {
  INIT,
  CHOOSECOLOR,
  CHOOSEDIFFICULTY,
  INGAME,
  PLAYERMOVE,
  AIMOVE,
  GAMEOVER
};

State currentState = INIT;
ColorSelection selectedColor = MCUMAX_BOARD_WHITE;

enum PlayerMoveSelection { FROMROW, FROMCOL, TOROW, TOCOL };
PlayerMoveSelection currentPlayerMoveSelection = FROMROW;

int fromRow = 0;
int fromCol = 0;
int toRow = 0;
int toCol = 0;

mcumax_move move;

void initGame() {
  mcumax_init();
  StickCP2.Display.drawString("M5 Chess", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 3);
  StickCP2.Display.setTextSize(TEXT_SIZE_SMALL);
  StickCP2.Display.drawString("Press BTN Joystick for new game",
                              StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.setTextSize(TEXT_SIZE_MEDIUM);
  if (isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    currentState = CHOOSECOLOR;
  }
}

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

  if (isLeft()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    selectedColor = MCUMAX_BOARD_WHITE;
    StickCP2.Display.clear();
  }
  if (isRight()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    selectedColor = MCUMAX_BOARD_BLACK;
    StickCP2.Display.clear();
  }

  if (isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    currentState = CHOOSEDIFFICULTY;
  }
}

void chooseDifficulty() {
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  StickCP2.Display.drawString("Choose Difficulty", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 5);
  StickCP2.Display.setTextSize(TEXT_SIZE_MEDIUM);
  StickCP2.Display.drawCenterString(
      String("Level: ") +
          String(difficulty_levels[currentIndexDifficulty].level),
      StickCP2.Display.width() / 2, StickCP2.Display.height() / 2);

  if (isDown()) {
    if (currentIndexDifficulty > 0) {
      StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
      currentIndexDifficulty--;
      StickCP2.Display.clear();
    }
  }
  if (isUp()) {
    if (currentIndexDifficulty < 14) {
      StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
      currentIndexDifficulty++;
      StickCP2.Display.clear();
    }
  }
  if (isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    mcu_nodes = difficulty_levels[currentIndexDifficulty].node_max;
    mcu_depth = difficulty_levels[currentIndexDifficulty].depth_max;
    currentState = INGAME;
  }
}

void inGame() {
  if (selectedColor == MCUMAX_BOARD_WHITE) {
    currentState = PLAYERMOVE;
  } else {
    currentState = AIMOVE;
  }
}

void playerMove() {
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  if (mcumax_is_checkmate(selectedColor)) {
    StickCP2.Display.drawString("Player in Checkmate!",
                                StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2);
    delay(CHECK_DELAY);
    StickCP2.Display.clear();
    currentState = GAMEOVER;
    return;
  }
  if (mcumax_is_check(selectedColor)) {
    StickCP2.Display.drawString("Player in Check!",
                                StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2);
    delay(CHECK_DELAY);
    StickCP2.Display.clear();
  }
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  StickCP2.Display.drawString("Player Move", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 5);
  StickCP2.Display.setTextSize(TEXT_SIZE_MEDIUM);
  StickCP2.Display.drawString("->", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  StickCP2.Display.drawString(String(playerRow[fromRow]),
                              StickCP2.Display.width() / 2 - 60,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.drawString(String(playerCol[fromCol]),
                              StickCP2.Display.width() / 2 - 30,
                              StickCP2.Display.height() / 2);

  StickCP2.Display.drawString(String(playerRow[toRow]),
                              StickCP2.Display.width() / 2 + 30,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.drawString(String(playerCol[toCol]),
                              StickCP2.Display.width() / 2 + 60,
                              StickCP2.Display.height() / 2);

  if (currentPlayerMoveSelection == FROMROW) {
    StickCP2.Display.drawRect(StickCP2.Display.width() / 2 - 75,
                              StickCP2.Display.height() / 2 - 15, 30, 30,
                              WHITE);
  }
  if (currentPlayerMoveSelection == FROMCOL) {
    StickCP2.Display.drawRect(StickCP2.Display.width() / 2 - 45,
                              StickCP2.Display.height() / 2 - 15, 30, 30,
                              WHITE);
  }
  if (currentPlayerMoveSelection == TOROW) {
    StickCP2.Display.drawRect(StickCP2.Display.width() / 2 + 15,
                              StickCP2.Display.height() / 2 - 15, 30, 30,
                              WHITE);
  }
  if (currentPlayerMoveSelection == TOCOL) {
    StickCP2.Display.drawRect(StickCP2.Display.width() / 2 + 45,
                              StickCP2.Display.height() / 2 - 15, 30, 30,
                              WHITE);
  }

  if (isRight()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    if (currentPlayerMoveSelection == FROMROW) {
      currentPlayerMoveSelection = FROMCOL;
    } else if (currentPlayerMoveSelection == FROMCOL) {
      currentPlayerMoveSelection = TOROW;
    } else if (currentPlayerMoveSelection == TOROW) {
      currentPlayerMoveSelection = TOCOL;
    }
    StickCP2.Display.clear();
  }
  if (isLeft()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    if (currentPlayerMoveSelection == TOCOL) {
      currentPlayerMoveSelection = TOROW;
    } else if (currentPlayerMoveSelection == TOROW) {
      currentPlayerMoveSelection = FROMCOL;
    } else if (currentPlayerMoveSelection == FROMCOL) {
      currentPlayerMoveSelection = FROMROW;
    }
    StickCP2.Display.clear();
  }
  if (isUp()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    if (currentPlayerMoveSelection == FROMROW) {
      if (fromRow < 7)
        fromRow++;
    } else if (currentPlayerMoveSelection == FROMCOL) {
      if (fromCol < 7)
        fromCol++;
    } else if (currentPlayerMoveSelection == TOROW) {
      if (toRow < 7)
        toRow++;
    } else if (currentPlayerMoveSelection == TOCOL) {
      if (toCol < 7)
        toCol++;
    }
    StickCP2.Display.clear();
  }
  if (isDown()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    if (currentPlayerMoveSelection == FROMROW) {
      if (fromRow > 0)
        fromRow--;
    } else if (currentPlayerMoveSelection == FROMCOL) {
      if (fromCol > 0)
        fromCol--;
    } else if (currentPlayerMoveSelection == TOROW) {
      if (toRow > 0)
        toRow--;
    } else if (currentPlayerMoveSelection == TOCOL) {
      if (toCol > 0)
        toCol--;
    }
    StickCP2.Display.clear();
  }

  if (isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    String fromSquareStr = get_square_str(fromRow, fromCol);
    String toSquareStr = get_square_str(toRow, toCol);
    mcumax_move move = (mcumax_move){
        get_square((char *)fromSquareStr.c_str()),
        get_square((char *)toSquareStr.c_str()),
    };
    mcumax_move valid_moves[max_moves];
    uint32_t valid_moves_num =
        mcumax_search_valid_moves(valid_moves, max_moves);
    bool is_valid_move = false;
    for (uint32_t i = 0; i < valid_moves_num; i++)
      if ((valid_moves[i].from == move.from) && (valid_moves[i].to == move.to))
        is_valid_move = true;

    if (is_valid_move) {
      StickCP2.Display.clear();
      mcumax_play_move(move);
      fromRow = 0;
      fromCol = 0;
      toRow = 0;
      toCol = 0;
      currentState = AIMOVE;
    } else {
      StickCP2.Display.clear();
      StickCP2.Display.drawString("Invalid Move", StickCP2.Display.width() / 2,
                                  StickCP2.Display.height() / 2);
      delay(INVALID_MOVE_DELAY);
      currentState = PLAYERMOVE;
      StickCP2.Display.clear();
    }
  }
}

void AiMove() {
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  static bool moveDisplayed = false;
  if (!moveDisplayed) {
    if (mcumax_is_checkmate(selectedColor == MCUMAX_BOARD_WHITE
                                ? MCUMAX_BOARD_BLACK
                                : MCUMAX_BOARD_WHITE)) {
      StickCP2.Display.drawString("AI in Checkmate!",
                                  StickCP2.Display.width() / 2,
                                  StickCP2.Display.height() / 2);
      delay(CHECK_DELAY);
      StickCP2.Display.clear();
      currentState = GAMEOVER;
    }
    if (mcumax_is_check(selectedColor == MCUMAX_BOARD_WHITE
                            ? MCUMAX_BOARD_BLACK
                            : MCUMAX_BOARD_WHITE)) {
      StickCP2.Display.drawString("AI in Check!", StickCP2.Display.width() / 2,
                                  StickCP2.Display.height() / 2);
      delay(CHECK_DELAY);
      StickCP2.Display.clear();
    }
    StickCP2.Display.clear();
    StickCP2.Display.drawString("AI is thinking...",
                                StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 3);

    mcumax_move move = mcumax_search_best_move(mcu_nodes, mcu_depth);
    if (move.from == MCUMAX_SQUARE_INVALID) {
      currentState = GAMEOVER;
      return;
    }
    mcumax_play_move(move);
    StickCP2.Display.clear();
    print_move(move);
    moveDisplayed = true;
  }
  if (isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    currentState = PLAYERMOVE;
    moveDisplayed = false;
  }
}

void gameOver() {
  StickCP2.Display.setTextSize(TEXT_SIZE_LARGE);
  StickCP2.Display.drawString("Game Over", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 2);
  if (isButtonPressed()) {
    StickCP2.Speaker.tone(TONE_FREQ, TONE_DURATION);
    StickCP2.Display.clear();
    currentState = INIT;
  }
}

void setup() {
  auto cfg = M5.config();
  StickCP2.begin(cfg);
  Wire.begin(0, 26, 100000UL);
  StickCP2.Display.setRotation(3);
  StickCP2.Display.setTextColor(WHITE);
  StickCP2.Display.setTextDatum(middle_center);
  StickCP2.Display.setTextSize(TEXT_SIZE_MEDIUM);
}

void loop() {
  StickCP2.update();
  Wire.beginTransmission(JOY_ADDR);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(JOY_ADDR, 3);

  if (Wire.available()) {
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();
  }

  switch (currentState) {
  case INIT:
    initGame();
    break;
  case CHOOSECOLOR:
    chooseColor();
    break;
  case CHOOSEDIFFICULTY:
    chooseDifficulty();
    break;
  case INGAME:
    inGame();
    break;
  case PLAYERMOVE:
    playerMove();
    break;
  case AIMOVE:
    AiMove();
    break;
  case GAMEOVER:
    gameOver();
    break;
  }

  prev_x = x_data;
  prev_y = y_data;
  prev_button = button_data;
}
