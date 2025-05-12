#pragma once

#include <raylib.h>
#include <vector>

class Puzzle {
public:
  Puzzle();
  virtual ~Puzzle() = default;

  void ShuffleBoard();
  bool IsSolved();
  void MoveTile(int mouse_x, int mouse_y);
  void Update();
  void Draw();

private:
  void InitColor();

private:
  /* data */
  std::vector<int> board_;
  int empty_index_;

  std::vector<Color> colors_;
  bool show_hint_color_ = true;
};
