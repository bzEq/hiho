#pragma once

#include "core.h"

#include <fstream>
#include <string>

namespace hiho {

class RGBPhoto {
  using StorageTy = Eigen::Matrix<RGBColor, Eigen::Dynamic, Eigen::Dynamic>;
  size_t width_, height_;
  StorageTy film_;

public:
  explicit RGBPhoto(size_t width, size_t height)
      : width_(width), height_(height), film_(width_, height_) {}

  size_t width() const { return width_; }

  size_t height() const { return height_; }

  RGBColor GetColor(size_t x, size_t y) const {
    assert(x < width_ && y < height_);
    return film_(x, y);
  }

  void SetColor(size_t x, size_t y, RGBColor color) { film_(x, y) = color; }
};

class PPMWriter {
  RGBPhoto photo_;

public:
  explicit PPMWriter(RGBPhoto &photo) : photo_(photo) {}

  static uint8_t Normalize(FloatTy x) {
    assert(x >= 0 && x <= 1);
    return (uint8_t)(255 * x);
  }

  void Write(std::ostream &out) {
    size_t width = photo_.width(), height = photo_.height();
    out << "P3\n" << width << " " << height << "\n255\n";
    for (size_t j = 0; j < height; ++j) {
      for (size_t i = 0; i < width; ++i) {
        RGBColor color = photo_.GetColor(i, height - 1 - j);
        // std::cout << color << std::endl;
        uint8_t r = Normalize(color[0]), g = Normalize(color[1]),
                b = Normalize(color[2]);
        out << std::to_string(r) << " " << std::to_string(g) << " "
            << std::to_string(b) << "\n";
      }
    }
  }
};

} // namespace hiho
