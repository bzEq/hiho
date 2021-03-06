// Copyright (c) 2020 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include <eigen3/Eigen/Dense>
#include <iostream>
#include <optional>
#include <random>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

namespace hiho {

using FloatTy = float;
using IntTy = int32_t;
using Vec3f = Eigen::Matrix<FloatTy, 3, 1>;
using Vec3i = Eigen::Matrix<IntTy, 3, 1>;
using Mat3f = Eigen::Matrix<FloatTy, 3, 3>;
using RGBColor = Vec3f;

constexpr FloatTy EPSILON = 1e-6;
const FloatTy PI = 4 * std::atan(1);

class Random {
public:
  explicit Random() : generator_(device_()), distribution_(0, 1) {}

  FloatTy Next() { return distribution_(generator_); }

  IntTy NextInt() { return int_distribution_(generator_); }

private:
  std::random_device device_;
  std::mt19937_64 generator_;
  std::uniform_real_distribution<FloatTy> distribution_;
  std::uniform_int_distribution<IntTy> int_distribution_;
};

struct Ray {
  Vec3f origin, direction;

  Ray() = default;

  explicit Ray(Vec3f origin, Vec3f direction)
      : origin(origin), direction(direction.normalized()) {}

  Vec3f At(FloatTy distance) const { return origin + distance * direction; }
};

struct GeometryConcept;

struct Intersection {
  FloatTy distance;
  const GeometryConcept *object;
};

struct Scattery {
  // scattered := -wi.
  Ray scattered;
  // Calculated via wi and wo.
  Vec3f pdf;
};

struct GeometryConcept {
  virtual std::optional<Intersection> Intersect(const Ray &) const {
    return std::nullopt;
  }

  // in := -wo.
  virtual std::vector<Scattery> Scatter(const Vec3f &in,
                                        const Vec3f &point) const {
    return {};
  }

  virtual Vec3f EmitEnergy(const Vec3f &point, const Vec3f &direction) const {
    return Vec3f{0, 0, 0};
  }

  virtual ~GeometryConcept() {}
};

struct MaterialConcept {
  virtual std::vector<Scattery> Scatter(const Vec3f &point, const Vec3f &in,
                                        const Vec3f &normal) const {
    return {};
  }
};

} // namespace hiho
