#pragma once

#include "puzzle.h"
class Game {
public:
  Game() = default;
  virtual ~Game() = default;

  void Run();

private:
  /* data */
  void Init();
  void Update();
  void Draw();

  Puzzle puzzle;
};
