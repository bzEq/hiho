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
      // Update energy.
      energy =
          energy.array() +
          f.array() * i->object->EmitEnergy(ray.At(i->distance), -ray.direction)
                          .array();
      // TODO: Update pdf.
      auto s = i->object->Scatter(ray, ray.At(i->distance));
      if (!s)
        break;
      ray = *s;
      --bounces;
    }
    return energy;
  }
};

} // namespace hiho
