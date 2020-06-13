#pragma once

#include "core.h"
#include "support.h"

namespace hiho {

struct Sphere : GeometryConcept {
  Vec3f center;
  FloatTy radius;

  explicit Sphere(const Vec3f &center, FloatTy radius)
      : center(center), radius(radius) {}

  std::optional<Intersection> Intersect(const Ray &ray) const {
    Vec3f d = ray.origin - center;
    FloatTy a = ray.direction.dot(ray.direction);
    FloatTy b = 2 * d.dot(ray.direction);
    FloatTy c = d.dot(d) - radius * radius;
    FloatTy x0, x1;
    bool ok = SolveQuadratic(a, b, c, x0, x1);
    if (!ok)
      return std::nullopt;
    FloatTy t = LessEqual(x0, 0) ? x1 : x0;
    if (LessEqual(t, 0))
      return std::nullopt;
    return Intersection{t, this};
  }

  std::optional<Ray> Scatter(const Ray &ray, const Vec3f &point) const {
    // Vec3f normal = GetNaturalNormal(point);
    // FloatTy cosine = Cosine(ray.direction, normal);
    // if (LessEqual(cosine, 0)) {
    //   return Ray(point, Reflect(-ray.direction, normal));
    // }
    return std::nullopt;
  }

  Vec3f EmitEnergy(const Vec3f &point, const Vec3f &direction) const {
    Vec3f normal = GetNaturalNormal(point);
    FloatTy cosine = Cosine(direction, normal);
    if (LessEqual(cosine, 0))
      return Vec3f{0, 0, 0};
    return Vec3f{20, 40, 80} * cosine;
  }

  Vec3f GetNaturalNormal(Vec3f point) const {
    return (point - center).normalized();
  }
};

} // namespace hiho
