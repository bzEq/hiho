#include "camera.h"
#include "materials.h"
#include "sphere.h"
#include "world.h"

#include <string>

int main(int argc, char *argv[]) {
  using namespace hiho;
  const size_t width = 800, height = 600;
  RGBPhoto photo(width, height);
  World scene;
  Glass glass;
  Solid solid;
  scene.AddObject<Sphere>(Vec3f{100, 100, 100}, 10, &solid, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{0, 0, -200}, 100, &solid, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{-1000, 0, 0}, 800, &glass, Vec3f{0, 0, 0});
  scene.AddObject<Sphere>(Vec3f{0, 0, -2000}, 1000, &solid,
                          Vec3f{100, 100, 100});
  scene.AddObject<Sphere>(Vec3f{2000, 0, 0}, 1000, &solid, Vec3f{20, 40, 80});
  scene.AddObject<Sphere>(Vec3f{0, 2000, 0}, 1000, &solid, Vec3f{20, 20, 20});
  Camera cam({1000, 0, 500}, {0, 0, 0}, 1.8);
  cam.TakePhoto(scene, photo, 1024, 256);
  PPMWriter writer(photo);
  writer.Write(std::cout);
  return 0;
}
