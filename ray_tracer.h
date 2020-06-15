#pragma once

#include "core.h"
#include "world.h"

#include <limits>

namespace hiho {

class RayTracer {
  const World &scene_;

public:
  explicit RayTracer(const World &scene) : scene_(scene) {}

  Vec3f Trace(const Ray &ray, size_t bounces) {
    // L = L0 + L1*F0 + L2*F0*F1 + L3*F0*F1*F2 + ...
    Vec3f energy{0, 0, 0};
    std::vector<Scattery> worklist;
    worklist.emplace_back(Scattery{ray, {1, 1, 1}});
    while (bounces && !worklist.empty()) {
      Scattery s(std::move(worklist.back()));
      worklist.pop_back();
      auto i = scene_.Intersect(s.scattered);
      if (!i)
        continue;
      Vec3f point = s.scattered.At(i->distance);
      energy = energy.array() +
               i->object->EmitEnergy(point, -s.scattered.direction).array() *
                   s.pdf.array();
      auto t = i->object->Scatter(s.scattered.direction, point);
      if (t.empty())
        continue;
      for (auto &e : t) {
        e.scattered.origin = e.scattered.At(0.01);
        e.pdf = s.pdf.array() * e.pdf.array();
      }
      worklist.insert(worklist.end(), t.begin(), t.end());
      --bounces;
    }
    return energy;
  }
};

} // namespace hiho
