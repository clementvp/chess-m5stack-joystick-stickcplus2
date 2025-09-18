#pragma once

// Enum pour l'état du jeu
enum State {
  INIT,
  CHOOSECOLOR,
  CHOOSEDIFFICULTY,
  INGAME,
  PLAYERMOVE,
  AIMOVE,
  GAMEOVER
};

// Enum pour la couleur sélectionnée
enum ColorSelection { MCUMAX_BOARD_WHITE, MCUMAX_BOARD_BLACK };
