#pragma once

namespace hiho {

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

inline Vec3f Reflect(Vec3f in, Vec3f normal) {
  return in - 2 * (normal.dot(in)) * normal;
}

inline FloatTy Cosine(Vec3f a, Vec3f b) {
  return a.dot(b) / (a.norm() * b.norm());
}

inline bool LessEqual(FloatTy x, FloatTy y) { return x < y || IsEqual(x, y); }

} // namespace hiho
