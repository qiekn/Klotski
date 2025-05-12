#include <raylib.h>
#include "game.h"

int main() {
  SetTraceLogLevel(LOG_DEBUG);

  Game game;
  game.Run();

  CloseWindow();
  return 0;
}
