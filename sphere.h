#pragma once

#include "core.h"
#include "support.h"

namespace hiho {

struct Sphere : GeometryConcept {
  Vec3f center;
  FloatTy radius;

  explicit Sphere(const Vec3f &center, FloatTy radius)
      : center(center), radius(radius) {}

  std::optional<Intersection> Intersect(const Ray &ray) const override {
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

  std::optional<Ray> Scatter(const Vec3f &in,
                             const Vec3f &point) const override {
    Vec3f normal = GetNaturalNormal(point);
    FloatTy cosine = Cosine(in, normal);
    if (LessEqual(cosine, 0)) {
      return Ray(point, Reflect(in, normal));
    }
    return std::nullopt;
  }

  Vec3f GetNaturalNormal(const Vec3f &point) const {
    return (point - center).normalized();
  }

  Vec3f GetPDF(const Vec3f &point, const Vec3f &in,
               const Vec3f &out) const override {
    static const FloatTy PI = acos(-1);
    return Vec3f{1 / PI, 1 / PI, 1 / PI};
  }
};

} // namespace hiho
