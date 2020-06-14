#pragma once

#include "core.h"
#include "support.h"

namespace hiho {

struct Sphere : GeometryConcept {
  Vec3f center;
  FloatTy radius;
  MaterialConcept *material;

  explicit Sphere(const Vec3f &center, FloatTy radius,
                  MaterialConcept *material)
      : center(center), radius(radius), material(material) {}

  explicit Sphere(const Vec3f &center, FloatTy radius)
      : center(center), radius(radius), material(nullptr) {}

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
    auto s = material->Scatter(in, normal);
    if (!s)
      return std::nullopt;
    return Ray(point, *s);
  }

  Vec3f GetNaturalNormal(const Vec3f &point) const {
    return (point - center).normalized();
  }

  Vec3f GetPDF(const Vec3f &point, const Vec3f &in,
               const Vec3f &out) const override {
    Vec3f normal = GetNaturalNormal(point);
    return material->GetPDF(normal, in, out);
  }
};

} // namespace hiho
