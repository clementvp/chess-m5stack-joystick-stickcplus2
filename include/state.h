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

// Enum pour la sélection du mouvement du joueur
enum PlayerMoveSelection { FROMROW, FROMCOL, TOROW, TOCOL };
