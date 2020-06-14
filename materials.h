#pragma once

#include "core.h"

namespace hiho {

struct Glass : MaterialConcept {
  // FloatTy refrac;
  // explicit Glass(FloatTy refrac) : refrac(refrac) {}

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

} // namespace hiho
