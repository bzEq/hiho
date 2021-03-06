// Copyright (c) 2020 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include "core.h"

#include <memory>
#include <vector>

namespace hiho {

struct World : GeometryConcept {

  std::vector<std::unique_ptr<GeometryConcept>> objects;

  template <typename GeometryTy, typename... Args>
  void AddObject(Args &&... args) {
    objects.emplace_back(new GeometryTy(std::forward<Args>(args)...));
  }

  std::optional<Intersection> Intersect(const Ray &ray) const override {
    Intersection i{std::numeric_limits<FloatTy>::max(), nullptr};
    for (const auto &obj : objects) {
      auto ii = obj->Intersect(ray);
      if (ii && ii->distance < i.distance) {
        i.object = obj.get();
        i.distance = ii->distance;
      }
    }
    if (i.distance < std::numeric_limits<FloatTy>::max())
      return i;
    return std::nullopt;
  }
};

} // namespace hiho
