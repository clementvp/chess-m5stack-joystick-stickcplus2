#pragma once

enum State {
  INIT,
  CHOOSECOLOR,
  CHOOSEDIFFICULTY,
  INGAME,
  PLAYERMOVE,
  AIMOVE,
  GAMEOVER
};

enum PlayerMoveSelection { FROMROW, FROMCOL, TOROW, TOCOL };
