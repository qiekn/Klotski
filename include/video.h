#pragma once

#include <raylib.h>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>

class PuzzleVideo {
public:
  PuzzleVideo();
  virtual ~PuzzleVideo();

  void Update();
  void Draw(int index, int x, int y);

  bool IsValid() const { return is_valid_; }

private:
  bool GetVideoResolution(const std::string& file);
  void Init();

private:
  /* data */
  std::string decode_cmd_;
  std::string info_cmd_;

  int frame_width_ = 0;
  int frame_height_ = 0;
  size_t frame_size_ = 0;

  std::unique_ptr<uint8_t[]> frame_;

  FILE* pipe_;
  Texture2D texture_;
  bool is_valid_ = false;
};
