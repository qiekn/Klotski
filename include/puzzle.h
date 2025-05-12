#pragma once

#include <vector>
class Puzzle {
public:
  Puzzle();
  virtual ~Puzzle() = default;

  void ShuffleBoard();
  bool IsSolved();
  void MoveTile(int mouse_x, int mouse_y);
  void Draw();

private:
  /* data */
  std::vector<int> board_;
  int empty_index_;
};
