#pragma once

#include <vector>

#include "amplitude.hpp"
#include "autd3/driver/datagram/gain.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/constraint.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

struct LMOption {
  float eps_1 = 1e-8f;
  float eps_2 = 1e-8f;
  float tau = 1e-3f;
  size_t k_max = 5;
  std::vector<float> initial;
  EmissionConstraint constraint =
      EmissionConstraint::Clamp(std::numeric_limits<driver::EmitIntensity>::min(), std::numeric_limits<driver::EmitIntensity>::max());

  operator native_methods::LMOption() const {
    return native_methods::LMOption{
        .constraint = constraint,
        .eps_1 = eps_1,
        .eps_2 = eps_2,
        .tau = tau,
        .k_max = static_cast<uint32_t>(k_max),
        .initial = initial.data(),
        .initial_len = static_cast<uint32_t>(initial.size()),
    };
  }
};

template <backend B>
struct LM final : driver::Gain, driver::IntoDatagramTuple<LM<B>> {
  AUTD3_API explicit LM(std::vector<std::pair<driver::Point3, Amplitude>> foci, LMOption option, std::shared_ptr<B> backend)
      : foci(std::move(foci)), option(std::move(option)), backend(std::move(backend)) {}

  std::vector<std::pair<driver::Point3, Amplitude>> foci;
  LMOption option;
  std::shared_ptr<B> backend;

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    std::vector<native_methods::Point3> points;
    points.reserve(foci.size());
    std::ranges::transform(foci, std::back_inserter(points),
                           [&](const auto& f) { return native_methods::Point3{f.first.x(), f.first.y(), f.first.z()}; });
    std::vector<float> amps;
    amps.reserve(foci.size());
    std::ranges::transform(foci, std::back_inserter(amps), [&](const auto& f) { return f.second.pascal(); });
    return backend->lm(reinterpret_cast<const float*>(points.data()), amps.data(), static_cast<uint32_t>(foci.size()), option);
  }
};

}  // namespace autd3::gain::holo
