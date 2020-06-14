#pragma once

#include "core.h"

namespace hiho {

struct Glass : MaterialConcept {
  std::optional<Vec3f> Scatter(const Vec3f &in,
                               const Vec3f &normal) const override {
    if (LessEqual(in.dot(normal), 0)) {
      return Reflect(in, normal);
    }
    return std::nullopt;
  }

  Vec3f GetPDF(const Vec3f &normal, const Vec3f &in,
               const Vec3f &out) const override {
    return Vec3f{1, 1, 1};
  }
};

struct Solid : MaterialConcept {
  Vec3f pdf;

  explicit Solid(const Vec3f &pdf) : pdf(pdf) {}

  std::optional<Vec3f> Scatter(const Vec3f &in,
                               const Vec3f &normal) const override {
    Vec3f n = SampleFromSphere();
    return normal + n;
  }

  Vec3f GetPDF(const Vec3f &normal, const Vec3f &in,
               const Vec3f &out) const override {
    if (normal.dot(in) > 0 || normal.dot(out) < 0)
      return Vec3f{0, 0, 0};
    return Vec3f{1 / PI, 1 / PI, 1 / PI} * std::abs(Cosine(normal, out));
  }
};

} // namespace hiho
