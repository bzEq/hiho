#pragma once

#include "core.h"
#include "photo.h"
#include "ray_tracer.h"
#include "support.h"
#include "world.h"

#include <boost/asio.hpp>
#include <thread>

namespace hiho {

class Camera {
  Vec3f position_, lookat_, focal_;
  Vec3f localx_, localy_;

  void InitLocalXY() {
    Vec3f localz = (position_ - lookat_).normalized();
    if (IsEqual(localz[0], 0) && IsEqual(localz[1], 0)) {
      localx_ = Vec3f{1, 0, 0};
      localy_ = Vec3f{0, 1, 0};
      return;
    }
    if (IsEqual(localz[0], 0)) {
      localx_ = Vec3f{1, 0, 0};
    } else if (IsEqual(localz[1], 0)) {
      localx_ = Vec3f{0, 1, 0};
    } else {
      localx_ = Vec3f{-1, localz[0] / localz[1], 0}.normalized();
    }
    localy_ = localz.cross(localx_).normalized();
  }

public:
  explicit Camera(const Vec3f &position, const Vec3f &lookat,
                  FloatTy focal_length)
      : position_(position), lookat_(lookat),
        focal_(
            Ray(position, (position - lookat).normalized()).At(focal_length)) {
    InitLocalXY();
  }

  Ray EmitRay(FloatTy x, FloatTy y) const {
    Vec3f point = position_ + localx_ * x + localy_ * y;
    Vec3f direction = (point - focal_).normalized();
    return Ray(point, direction);
  }

  static RGBColor EnergyToColor(Vec3f energy) { return Clamp(energy); }

  void TakePhoto(const World &scene, RGBPhoto &photo, size_t num_samples,
                 size_t ray_bounces) const {
    size_t width = photo.width(), height = photo.height();
    auto CreateRayFor = [&](FloatTy x, FloatTy y) {
      FloatTy a = 1.0f * x / width - 0.5;
      FloatTy b = 1.0f * y / width - (0.5f * height / width);
      return EmitRay(a, b);
    };
    auto Filter = [](FloatTy x) { return std::exp(-(x * x)); };
    boost::asio::thread_pool pool(std::thread::hardware_concurrency());
    // boost::asio::thread_pool pool(1U);
    RayTracer tracer(scene);
    size_t done = 0, total = width * height;
    std::mutex mu;
    for (size_t i = 0; i < width; ++i) {
      for (size_t j = 0; j < height; ++j) {
        boost::asio::post(pool, [&, i, j]() {
          Random rnd;
          Vec3f energy{0, 0, 0};
          for (size_t k = 0; k < num_samples; ++k) {
            FloatTy x = (FloatTy)i + rnd.Next() - 0.5f;
            FloatTy y = (FloatTy)j + rnd.Next() - 0.5f;
            Ray ray = CreateRayFor(x, y);
            FloatTy r = std::sqrt((x - i) * (x - i) + (y - j) * (y - j));
            energy += Filter(r) * tracer.Trace(ray, ray_bounces) /
                      (FloatTy)num_samples;
          }
          assert(energy[0] >= 0 && energy[1] >= 0 && energy[2] >= 0);
          RGBColor color = EnergyToColor(energy);
          photo.SetColor(i, j, color);
          std::unique_lock<std::mutex> l(mu);
          done += 1;
          std::cerr << done << "/" << total << "\r";
        });
      }
    }
    // std::cout << tracer.hit_count_ << std::endl;
    pool.join();
  }
};

} // namespace hiho
