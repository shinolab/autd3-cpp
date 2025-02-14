#pragma once

#include <vector>

#include "amplitude.hpp"
#include "autd3/driver/datagram/gain.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/constraint.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

struct GSPATOption {
  size_t repeat = 100;
  EmissionConstraint constraint =
      EmissionConstraint::Clamp(std::numeric_limits<driver::EmitIntensity>::min(), std::numeric_limits<driver::EmitIntensity>::max());

  operator native_methods::GSPATOption() const {
    return native_methods::GSPATOption{.constraint = constraint, .repeat = static_cast<uint32_t>(repeat)};
  }
};

template <backend B>
struct GSPAT final : driver::Gain, driver::IntoDatagramTuple<GSPAT<B>> {
  AUTD3_API explicit GSPAT(std::vector<std::pair<driver::Point3, Amplitude>> foci, const GSPATOption option, std::shared_ptr<B> backend)
      : foci(std::move(foci)), option(option), backend(std::move(backend)) {}

  std::vector<std::pair<driver::Point3, Amplitude>> foci;
  GSPATOption option;
  std::shared_ptr<B> backend;

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    std::vector<native_methods::Point3> points;
    points.reserve(foci.size());
    std::ranges::transform(foci, std::back_inserter(points),
                           [&](const auto& f) { return native_methods::Point3{f.first.x(), f.first.y(), f.first.z()}; });
    std::vector<float> amps;
    amps.reserve(foci.size());
    std::ranges::transform(foci, std::back_inserter(amps), [&](const auto& f) { return f.second.pascal(); });
    return backend->gspat(reinterpret_cast<const float*>(points.data()), amps.data(), static_cast<uint32_t>(foci.size()), option);
  }
};

template <backend B>
GSPAT(std::vector<std::pair<driver::Point3, Amplitude>> foci, GSPATOption option, std::shared_ptr<B> backend) -> GSPAT<B>;

}  // namespace autd3::gain::holo
