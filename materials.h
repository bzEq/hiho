#pragma once

#include "core.h"

namespace hiho {

struct Mirror : MaterialConcept {
  std::vector<Scattery> Scatter(const Vec3f &point, const Vec3f &in,
                                const Vec3f &normal) const override {
    std::vector<Scattery> result;
    Scattery s;
    Vec3f n(normal);
    if (in.dot(n) > 0)
      n = -n;
    s.scattered.origin = point;
    s.scattered.direction = Reflect(in, n);
    s.pdf = Vec3f{1, 1, 1};
    result.push_back(std::move(s));
    return result;
  }
};

struct Solid : MaterialConcept {
  RGBColor color;

  explicit Solid(const RGBColor &color) : color(color) {}

  std::vector<Scattery> Scatter(const Vec3f &point, const Vec3f &in,
                                const Vec3f &normal) const override {
    std::vector<Scattery> result;
    Vec3f n(normal);
    if (in.dot(n) > 0)
      n = -n;
    Scattery s;
    Vec3f out = SampleFromSphere();
    if (out.dot(n) < 0)
      out = -out;
    s.scattered.origin = point;
    s.scattered.direction = out;
    s.pdf = color * n.dot(-in) / PI;
    result.push_back(std::move(s));
    return result;
  }
};

} // namespace hiho
