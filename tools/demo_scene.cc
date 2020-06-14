#include "camera.h"
#include "materials.h"
#include "sphere.h"
#include "world.h"

#include <string>

int main(int argc, char *argv[]) {
  using namespace hiho;
  const size_t width = 1280, height = 720;
  RGBPhoto photo(width, height);
  World scene;
  Glass glass;
  Solid solid;
  scene.AddObject<Sphere>(Vec3f{0, 0, -200}, 100, &solid);
  scene.AddObject<Sphere>(Vec3f{0, 0, 100}, 100, &glass);
  scene.AddObject<Sphere>(Vec3f{200, 0, 0}, 100, &solid);
  Camera cam({1000, 1000, 1000}, {0, 0, 0}, 2);
  cam.TakePhoto(scene, photo, 128, 32);
  PPMWriter writer(photo);
  writer.Write(std::cout);
  return 0;
}
