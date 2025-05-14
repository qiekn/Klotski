#include <raylib.h>
#include "game.h"

int main() {
  SetTraceLogLevel(LOG_WARNING);

  Game game;
  game.Run();

  CloseWindow();
  return 0;
}
