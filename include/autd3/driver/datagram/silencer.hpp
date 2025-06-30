#pragma once

#include <chrono>
#include <variant>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct FixedCompletionSteps {
  uint16_t intensity = 10;
  uint16_t phase = 40;
  bool strict = true;
  operator native_methods::FixedCompletionSteps() const {
    return native_methods::FixedCompletionSteps{.intensity = intensity, .phase = phase, .strict = strict};
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr() const noexcept { return native_methods::AUTDDatagramSilencerFromCompletionSteps(*this); }
};

struct FixedCompletionTime {
  friend struct Silencer;

  std::chrono::nanoseconds intensity = std::chrono::microseconds(250);
  std::chrono::nanoseconds phase = std::chrono::microseconds(1000);
  bool strict = true;

  operator native_methods::FixedCompletionTime() const {
    return native_methods::FixedCompletionTime{
        .intensity = native_methods::to_duration(intensity), .phase = native_methods::to_duration(phase), .strict = strict};
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr() const noexcept { return native_methods::AUTDDatagramSilencerFromCompletionTime(*this); }
};

struct FixedUpdateRate {
  friend struct Silencer;

  uint16_t intensity;
  uint16_t phase;

  operator native_methods::FixedUpdateRate() const { return native_methods::FixedUpdateRate{.intensity = intensity, .phase = phase}; }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr() const noexcept { return native_methods::AUTDDatagramSilencerFromUpdateRate(*this); }
};

struct Silencer final : Datagram, IntoDatagramTuple<Silencer> {
  AUTD3_API [[nodiscard]] static Silencer disable() noexcept {
    return Silencer(FixedCompletionSteps{
        .intensity = 1,
        .phase = 1,
    });
  }

  Silencer() : Silencer(FixedCompletionSteps{}) {}
  explicit Silencer(const std::variant<FixedCompletionSteps, FixedCompletionTime, FixedUpdateRate>& config) : config(config) {}

  std::variant<FixedCompletionSteps, FixedCompletionTime, FixedUpdateRate> config;

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const noexcept override {
    return std::visit([this](const auto& c) { return c.ptr(); }, config);
  }
};
}  // namespace autd3::driver
