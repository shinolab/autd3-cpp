#pragma once

#include <vector>

#include "amplitude.hpp"
#include "autd3/driver/datagram/gain.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/constraint.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

struct GSOption {
  size_t repeat = 100;
  EmissionConstraint constraint =
      EmissionConstraint::Clamp(std::numeric_limits<driver::Intensity>::min(), std::numeric_limits<driver::Intensity>::max());

  operator native_methods::GSOption() const { return native_methods::GSOption{.constraint = constraint, .repeat = static_cast<uint32_t>(repeat)}; }
};

struct GS final : driver::Gain, driver::IntoDatagramTuple<GS> {
  AUTD3_API explicit GS(std::vector<std::pair<driver::Point3, Amplitude>> foci, const GSOption option) : foci(std::move(foci)), option(option) {}

  std::vector<std::pair<driver::Point3, Amplitude>> foci;
  GSOption option;

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    std::vector<native_methods::Point3> points;
    points.reserve(foci.size());
    std::ranges::transform(foci, std::back_inserter(points),
                           [&](const auto& f) { return native_methods::Point3{f.first.x(), f.first.y(), f.first.z()}; });
    std::vector<float> amps;
    amps.reserve(foci.size());
    std::ranges::transform(foci, std::back_inserter(amps), [&](const auto& f) { return f.second.pascal(); });
    return native_methods::AUTDGainHoloGSSphere(points.data(), amps.data(), static_cast<uint32_t>(foci.size()), option);
  }
};

}  // namespace autd3::gain::holo
