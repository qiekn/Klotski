#pragma once

#include <vector>
class Puzzle {
public:
  Puzzle();
  virtual ~Puzzle() = default;

  void ShuffleBoard();
  bool IsSolved();
  void MoveTile();
  void Draw();

private:
  /* data */
  std::vector<int> board_;
  int empty_index_;
};
