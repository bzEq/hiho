#pragma once

#include "core.h"
#include "world.h"

#include <limits>

namespace hiho {

class RayTracer {
  const World &scene_;

public:
  explicit RayTracer(const World &scene) : scene_(scene) {}

  Vec3f Trace(Ray ray, size_t bounces) {
    // L = L0 + L1*F0 + L2*F0*F1 + L3*F0*F1*F2 + ...
    Vec3f energy{0, 0, 0}, f{1, 1, 1};
    while (bounces) {
      auto i = scene_.Intersect(ray);
      if (!i)
        break;
      Vec3f point = ray.At(i->distance);
      energy = energy.array() +
               i->object->EmitEnergy(point, -ray.direction).array() * f.array();
      auto s = i->object->Scatter(ray.direction, point);
      if (!s)
        break;
      Vec3f pdf = i->object->GetPDF(point, -s->direction, -ray.direction);
      f = f.array() * pdf.array();
      ray.origin = s->At(0.01);
      ray.direction = s->direction;
      --bounces;
    }
    energy = energy.array() +
             scene_.EmitEnergy(ray.origin, -ray.direction).array() * f.array();
    return energy;
  }
};

} // namespace hiho
