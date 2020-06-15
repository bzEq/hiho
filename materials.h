#pragma once

#include "core.h"

namespace hiho {

struct Mirror : MaterialConcept {
  std::vector<Scattery> Scatter(const Vec3f &point, const Vec3f &in,
                                const Vec3f &normal) const override {
    std::vector<Scattery> result;
    Scattery s;
    if (in.dot(normal) < 0) {
      s.scattered.origin = point;
      s.scattered.direction = Reflect(in, normal);
      s.pdf = Vec3f{1, 1, 1};
      result.push_back(std::move(s));
    }
    return result;
  }
};

struct Solid : MaterialConcept {
  RGBColor color;

  explicit Solid(const RGBColor &color) : color(color) {}

  std::vector<Scattery> Scatter(const Vec3f &point, const Vec3f &in,
                                const Vec3f &normal) const override {
    std::vector<Scattery> result;
    if (in.dot(normal) > 0)
      return result;
    Scattery s;
    Vec3f out = SampleFromSphere();
    if (out.dot(normal) < 0)
      out = -out;
    s.scattered.origin = point;
    s.scattered.direction = out;
    s.pdf = color * normal.dot(-in) / PI;
    result.push_back(std::move(s));
    return result;
  }
};

} // namespace hiho
