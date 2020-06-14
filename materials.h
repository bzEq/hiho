#pragma once

#include "core.h"

namespace hiho {

struct Glass : MaterialConcept {
  const FloatTy index;

  explicit Glass(FloatTy index) : index(index) {}

  std::optional<Vec3f> Scatter(const Vec3f &in,
                               const Vec3f &normal) const override {
    Vec3f n(normal);
    FloatTy i = index;
    FloatTy f = n.dot(in);
    if (f > 0) {
      i = 1 / i;
      n = -n;
      f = -f;
    }
    FloatTy r = (1 - f * f) / (i * i);
    if (r > 1) {
      return std::nullopt;
    }
    Vec3f out = (in - f * n) * (1 / i) - n * std::sqrt(1 - r);
    return out;
  }

  Vec3f GetPDF(const Vec3f &normal, const Vec3f &in,
               const Vec3f &out) const override {
    return Vec3f{1, 1, 1};
  }
};

struct Mirror : MaterialConcept {
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
    Vec3f n = SampleFromSphere();
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
