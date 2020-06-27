// Copyright (c) 2020 Kai Luo <gluokai@gmail.com>. All rights reserved.
// RUN: ./demo_scene > /tmp/demo.ppm

#include "camera.h"
#include "materials.h"
#include "sphere.h"
#include "world.h"

int main(int argc, char *argv[]) {
  using namespace hiho;
  const size_t width = 3840, height = 2160;
  const size_t spp = 4096, bounces = 12;
  RGBPhoto photo(width, height);
  World scene;
  Mirror mirror;
  Glass glass[] = {
      Glass(1.5),
      Glass(2.4),
  };
  Solid solid[] = {
      Solid(RGBColor{0.2, 0.4, 0.8}), // Blue.
      Solid(RGBColor{0.4, 0.4, 0.2}), // Yellow.
      Solid(RGBColor{0.1, 0.1, 0.1}), // Grey.
      Solid(RGBColor{0.4, 0.2, 0.2}), // Red.
      Solid(RGBColor{0.1, 0.4, 0.2}), // Green.
  };
  scene.AddObject<Sphere>(Vec3f{0, 0, 50}, 50, &solid[3], Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{-100, 200, 100}, 100, &solid[2],
                          Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{0, 100, 50}, 50, &mirror, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{100, 40, 25}, 25, &solid[1], Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{180, 20, 25}, 25, &glass[0], Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{150, 100, 40}, 40, &mirror, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{100, -100, 80}, 80, &mirror, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{100, 200, 50}, 50, &solid[4], Vec3f{0, 0, 0});
  // Light.
  scene.AddObject<Sphere>(Vec3f{0, 0, 1e3}, 200, &solid[2],
                          Vec3f{200, 200, 200});
  // Floor.
  scene.AddObject<Sphere>(Vec3f{0, 0, -1e4}, 1e4, &solid[0], Vec3f{0, 0, 0});
  Camera cam({500, 50, 200}, {0, 0, 0}, 0.4);
  cam.TakePhoto(scene, photo, spp, bounces,
                std::thread::hardware_concurrency());
  PPMWriter writer(photo);
  writer.Write(std::cout);
  return 0;
}
