#include "core.h"
#include "photo.h"

int main(int argc, char *argv[]) {
  using namespace hiho;
  const size_t width = 800, height = 600;
  RGBPhoto photo(width, height);
  for (size_t i = 0; i < width; ++i) {
    for (size_t j = 0; j < height; ++j) {
      FloatTy x = 1.0f * i / width;
      FloatTy y = 1.0f * j / height;
      FloatTy z = 1.0f * i * j / (width * height);
      photo.SetColor(i, j, {z, y, x});
    }
  }
  PPMWriter writer(photo);
  writer.Write(std::cout);
  return 0;
}
