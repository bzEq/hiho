// Copyright (c) 2020 Kai Luo <gluokai@gmail.com>. All rights reserved.

#pragma once

#include "core.h"

namespace hiho {

inline FloatTy Clamp(FloatTy x) { return x < 0 ? 0 : x > 1 ? 1 : x; }

inline Vec3f Clamp(const Vec3f &x) {
  return Vec3f{Clamp(x[0]), Clamp(x[1]), Clamp(x[2])};
}

inline bool IsEqual(FloatTy a, FloatTy b) { return std::abs(a - b) < EPSILON; }

inline bool IsEqual(Vec3f a, Vec3f b) { return (a - b).norm() < EPSILON; }

inline bool SolveQuadratic(FloatTy a, FloatTy b, FloatTy c, FloatTy &x0,
                           FloatTy &x1) {
  FloatTy delta = b * b - 4 * a * c;
  if (delta < 0)
    return false;
  FloatTy d = std::sqrt(delta);
  FloatTy t0 = (-b + d) / (2 * a);
  FloatTy t1 = (-b - d) / (2 * a);
  x0 = std::min(t0, t1);
  x1 = std::max(t0, t1);
  return true;
}

inline Vec3f Reflect(const Vec3f &in, const Vec3f &normal) {
  return in - 2 * (normal.dot(in)) * normal;
}

inline FloatTy Cosine(const Vec3f &a, const Vec3f &b) {
  return a.dot(b) / (a.norm() * b.norm());
}

inline bool LessEqual(FloatTy x, FloatTy y) { return x < y || IsEqual(x, y); }

inline FloatTy UniformSample() {
  static thread_local std::random_device device;
  static thread_local std::mt19937 gen(device());
  static thread_local std::uniform_real_distribution<FloatTy> dis(0, 1);
  return dis(gen);
}

inline Vec3f SampleFromHemiSphere() {
  FloatTy u = 2 * PI * UniformSample();
  FloatTy v = UniformSample();
  return Vec3f{std::cos(u) * std::sqrt(v), std::sin(u) * std::sqrt(v),
               std::sqrt(1 - v)};
}

inline Vec3f SampleFromSphere() {
  Vec3f v = SampleFromHemiSphere();
  return UniformSample() < 0.5 ? v : -v;
}

inline bool Refract(const Vec3f &in, const Vec3f &normal, FloatTy index,
                    Vec3f &out) {
  FloatTy f = in.dot(normal);
  FloatTy discriminant = 1 - index * index * (1 - f * f);
  if (discriminant < 0)
    return false;
  out = index * (in - normal * f) - normal * std::sqrt(discriminant);
  return true;
}

} // namespace hiho
