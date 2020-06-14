#pragma once

#include "core.h"
#include "support.h"

namespace hiho {

struct Sphere : GeometryConcept {
  Vec3f center;
  FloatTy radius;
  MaterialConcept *material;
  Vec3f energy;

  explicit Sphere(const Vec3f &center, FloatTy radius,
                  MaterialConcept *material, const Vec3f &energy)
      : center(center), radius(radius), material(material), energy(energy) {}

  explicit Sphere(const Vec3f &center, FloatTy radius)
      : center(center), radius(radius), material(nullptr), energy{0, 0, 0} {}

  std::optional<Intersection> Intersect(const Ray &ray) const override {
    Vec3f d = ray.origin - center;
    FloatTy a = ray.direction.dot(ray.direction);
    FloatTy b = 2 * d.dot(ray.direction);
    FloatTy c = d.dot(d) - radius * radius;
    FloatTy x0, x1;
    bool ok = SolveQuadratic(a, b, c, x0, x1);
    if (!ok)
      return std::nullopt;
    FloatTy t = x0 < 0 ? x1 : x0;
    if (t > 0) {
      return Intersection{t, this};
    }
    return std::nullopt;
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

  Vec3f EmitEnergy(const Vec3f &point, const Vec3f &direction) const override {
    Vec3f normal = GetNaturalNormal(point);
    FloatTy f = normal.dot(direction);
    if (f < 0)
      return Vec3f{0, 0, 0};
    return energy;
  }
};

} // namespace hiho
