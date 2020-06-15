// Copyright (c) 2020 Kai Luo <gluokai@gmail.com>. All rights reserved.
// RUN: ./random_scene > /tmp/random.ppm

#include "camera.h"
#include "materials.h"
#include "sphere.h"
#include "support.h"
#include "world.h"

#include <math.h>

template <class T, size_t N>
constexpr inline size_t array_length(T (&)[N]) {
  return N;
}

int main(int argc, char *argv[]) {
  using namespace hiho;
  const size_t width = 800, height = 600;
  const size_t spp = 1024, bounces = 12;
  RGBPhoto photo(width, height);
  World scene;
  Mirror mirror;
  Glass glass[] = {
      Glass(1.5),
      Glass(2.4),
  };
  Solid solid[] = {
      Solid(RGBColor{0.4, 0.4, 0.2}), // Yellow.
      Solid(RGBColor{0.1, 0.1, 0.1}), // Grey.
      Solid(RGBColor{0.4, 0.2, 0.2}), // Red.
      Solid(RGBColor{0.1, 0.4, 0.2}), // Green.
  };
  Solid blue(RGBColor{0.2, 0.4, 0.8});
  auto ChooseMaterial = [&]() -> MaterialConcept * {
    FloatTy p = UniformSample();
    if (p < 4.f / 10) {
      return &mirror;
    } else if (p < 5.f / 10) {
      return &glass[(size_t)floor(array_length(glass) * UniformSample())];
    }
    return &solid[(size_t)floor(array_length(solid) * UniformSample())];
  };
  auto RandomSphere = [&]() -> Sphere {
    MaterialConcept *m = ChooseMaterial();
    FloatTy radius = 40.f;
    FloatTy a = 2 * PI * UniformSample();
    FloatTy r = UniformSample() * 480;
    FloatTy x = r * std::cos(a);
    FloatTy y = r * std::sin(a);
    FloatTy z = radius;
    return Sphere({x, y, z}, radius, m, Vec3f{0, 0, 0});
  };
  for (size_t i = 0; i < 12; ++i) {
    scene.AddObject<Sphere>(RandomSphere());
  }
  // Light.
  scene.AddObject<Sphere>(Vec3f{0, 0, 1e3}, 200, &solid[1],
                          Vec3f{200, 200, 200});
  // Floor.
  scene.AddObject<Sphere>(Vec3f{0, 0, -1e4}, 1e4, &blue, Vec3f{0, 0, 0});
  Camera cam({400, 0, 200}, {0, 0, 0}, 0.4);
  cam.TakePhoto(scene, photo, spp, bounces);
  PPMWriter writer(photo);
  writer.Write(std::cout);
  return 0;
}
