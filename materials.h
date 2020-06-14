#pragma once

#include "core.h"

namespace hiho {

struct Glass : MaterialConcept {
  std::optional<Vec3f> Scatter(const Vec3f &in,
                               const Vec3f &normal) const override {
    if (in.dot(normal) < 0) {
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

  std::optional<Vec3f> Scatter(const Vec3f &in,
                               const Vec3f &normal) const override {
    Vec3f n = SampleFromHemiSphere();
    if (n.dot(normal) < 0)
      n = -n;
    return n;
  }

  Vec3f GetPDF(const Vec3f &normal, const Vec3f &in,
               const Vec3f &out) const override {
    if (normal.dot(in) > 0 || normal.dot(out) < 0)
      return Vec3f{0, 0, 0};
    return Vec3f{0.2, 0.4, 0.8} * normal.dot(out) / PI;
  }
};

} // namespace hiho
