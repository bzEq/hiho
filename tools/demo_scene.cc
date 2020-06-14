#include "camera.h"
#include "materials.h"
#include "sphere.h"
#include "world.h"

#include <string>

int main(int argc, char *argv[]) {
  using namespace hiho;
  const size_t width = 3840, height = 2160;
  RGBPhoto photo(width, height);
  World scene;
  Mirror mirror;
  Solid solid;
  Glass glass(1.05);
  scene.AddObject<Sphere>(Vec3f{0, 0, 50}, 50, &solid, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{-200, -200, 120}, 120, &mirror, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{-100, 200, 100}, 100, &solid, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{0, 100, 50}, 50, &mirror, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{100, 40, 20}, 20, &solid, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{150, 100, 40}, 40, &mirror, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{100, -100, 80}, 80, &mirror, Vec3f{0, 0, 0});
  // scene.AddObject<Sphere>(Vec3f{150, 0, 80}, 80, &glass, Vec3f{0, 0, 0});
  // Light.
  scene.AddObject<Sphere>(Vec3f{1e7, 0, 0}, 1e7 - 1e5, &solid,
                          Vec3f{10, 10, 10});
  // Floor.
  scene.AddObject<Sphere>(Vec3f{0, 0, -1e4}, 1e4, &solid, Vec3f{0, 0, 0});
  Camera cam({500, 0, 400}, {0, 0, 100}, 0.4);
  cam.TakePhoto(scene, photo, 4096, 16);
  PPMWriter writer(photo);
  writer.Write(std::cout);
  return 0;
}
