#include "camera.h"
#include "photo.h"
#include "ray_tracer.h"
#include "sphere.h"
#include "world.h"

#include <gtest/gtest.h>

namespace {

using namespace hiho;

TEST(EigenTest, NumMul) {
  Vec3f x{1, 2, 3};
  Vec3f y{4, 5, 6};
  x = x.array() * y.array();
  EXPECT_TRUE(x[0] = 4);
  EXPECT_TRUE(x[1] = 10);
  EXPECT_TRUE(x[1] = 18);
}

TEST(PhotoTest, SetColor) {
  RGBPhoto film(1920, 1080);
  film.SetColor(0, 0, {0, 0, 0});
}

TEST(SphereTest, Intersection0) {
  Sphere s{
      {0, 0, 0},
      1,
  };
  Ray ray({0, 0, 100}, {0, 0, 1});
  auto i = s.Intersect(ray);
  EXPECT_FALSE(i);
}

TEST(SphereTest, Intersection1) {
  Sphere s{
      {0, 0, 100},
      80,
  };
  Ray ray({0.49875, -0.125938, 0}, {0.695253, -0.175556, 0.696996});
  auto i = s.Intersect(ray);
  EXPECT_TRUE(i);
}

TEST(SphereTest, Intersection2) {
  Sphere s{
      {0, 0, 100},
      80,
  };
  Ray ray({-1, -1, 0}, {0, 0, 1});
  auto i = s.Intersect(ray);
  EXPECT_TRUE(i);
}

TEST(World, AddObject) {
  World scene;
  scene.AddObject<Sphere>(Vec3f{0, 0, 0}, 1);
}

} // namespace
