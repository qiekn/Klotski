#include "video.h"
#include <_stdio.h>
#include <raylib.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include "constants.h"
#include "utils.h"

PuzzleVideo::PuzzleVideo() {}

PuzzleVideo::~PuzzleVideo() {
  UnloadTexture(texture_);
  pclose(pipe_);
}

bool PuzzleVideo::GetVideoResolution(const std::string& file) {
  info_cmd_ =
      "ffprobe "
      "-v error "                           // set log level
      "-select_streams v:0 "                // choose 1st video stream
      "-show_entries stream=width,height "  // only show resolution info
      "-of csv=p=0 "  // output format CSV and disable key name
      + file;
  // read video resolution from meta info
  FILE* info_pipe = popen(info_cmd_.c_str(), "r");
  if (!info_pipe) {
    std::cerr << "Error opening ffmpeg for information. Did you install ffmpeg?"
              << std::endl;
    return false;
  }
  char line[128];
  if (fgets(line, sizeof(line), info_pipe)) {
    if (sscanf(line, "%d,%d", &frame_width_, &frame_height_) != 2) {
      std::cerr << "Error: Failed to ffprobe video resolution" << std::endl;
    }
  }
  pclose(info_pipe);

  // check frame width & height
  if (frame_width_ <= 0 || frame_height_ <= 0) {
    std::cerr << "Error: invalid video resolution" << std::endl;
    return false;
  }

  frame_size_ = frame_width_ * frame_height_ * 4;  // RGBA: 4 byte each pixel
  return true;
}

void PuzzleVideo::Init() {
  // construct commands
  const std::string file = "mouse.mp4";
  decode_cmd_ =
      "ffmpeg -re -i " + file + " -f rawvideo -pix_fmt rgba - 2>/dev/null";

  if (!GetVideoResolution(file)) return;

  // read frame
  pipe_ = popen(decode_cmd_.c_str(), "r");
  if (!pipe_) {
    std::cerr << "Error opening ffmpeg for frames. Did you install ffmpeg?\n";
    return;
  }

  frame_ = std::make_unique<uint8_t[]>(frame_size_);

  Image image = {.data = frame_.get(),
                 .width = frame_width_,
                 .height = frame_height_,
                 .mipmaps = 1,
                 .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

  texture_ = LoadTextureFromImage(image);
  TraceLog(LOG_INFO, "frame_size_ %d", frame_size_);
  if (texture_.id == 0) {
    std::cerr << "Error: Failed to load texture from image" << std::endl;
    pclose(pipe_);
    pipe_ = nullptr;
    return;
  }
  is_valid_ = true;
}

void PuzzleVideo::Update() {
  if (!IsValid()) {
    Init();
  }
  if (!IsValid()) {
    std::cerr << "Cannot update: Video not initialized" << std::endl;
    return;
  }

  size_t read_bytes = fread(frame_.get(), 1, frame_size_, pipe_);

  if (read_bytes < frame_size_) {
    if (feof(pipe_)) {
      std::cerr << "Get the end of the video, Replay\n";
      pclose(pipe_);
      pipe_ = popen(decode_cmd_.c_str(), "r");
      read_bytes = fread(frame_.get(), 1, frame_size_, pipe_);
    } else {
      std::cerr << "Error while reading a frame" << std::endl;
      return;
    }
  }

  UpdateTexture(texture_, frame_.get());
}

void PuzzleVideo::Draw(int index, int x, int y) {
  if (!IsValid()) {
    std::cerr << "Cannot draw: Video not initialized" << std::endl;
    return;
  }
  float size = std::min(frame_width_, frame_height_);
  float grid_size = size * 1.0 / kTileCount;

  float offset_x = (frame_width_ - size) / 2;
  float offset_y = (frame_height_ - size) / 2;

  auto [texture_x, texture_y] = GetXY(index);
  // clang-format off
  DrawTexturePro(
      texture_,
      Rectangle{static_cast<float>(texture_x * grid_size + offset_x),
                static_cast<float>(texture_y * grid_size + offset_y),
                static_cast<float>(grid_size),
                static_cast<float>(grid_size)},
      Rectangle{static_cast<float>(x),
                static_cast<float>(y),
                static_cast<float>(kTileSize - kBorder),
                static_cast<float>(kTileSize - kBorder)},
      Vector2{0, 0}, 0.0f, WHITE);
  // clang-format on
}
