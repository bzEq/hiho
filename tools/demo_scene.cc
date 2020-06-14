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
  scene.AddObject<Sphere>(Vec3f{0, 0, -100}, 100, &glass);
  scene.AddObject<Sphere>(Vec3f{0, 0, 100}, 100, &solid);
  scene.AddObject<Sphere>(Vec3f{200, 0, 0}, 100, &glass);
  Camera cam({1000, 1000, 1000}, {0, 0, 0}, 3);
  cam.TakePhoto(scene, photo, 512, 8);
  PPMWriter writer(photo);
  writer.Write(std::cout);
  return 0;
}
