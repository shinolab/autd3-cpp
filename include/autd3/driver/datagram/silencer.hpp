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
  bool strict_mode = true;
  operator native_methods::FixedCompletionSteps() const {
    return native_methods::FixedCompletionSteps{.intensity = intensity, .phase = phase, .strict_mode = strict_mode};
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const native_methods::SilencerTarget target) const noexcept {
    return AUTDDatagramSilencerFromCompletionSteps(*this, target);
  }
};

struct FixedCompletionTime {
  friend struct Silencer;

  std::chrono::nanoseconds intensity = std::chrono::microseconds(250);
  std::chrono::nanoseconds phase = std::chrono::microseconds(1000);
  bool strict_mode = true;

  operator native_methods::FixedCompletionTime() const {
    return native_methods::FixedCompletionTime{
        .intensity = native_methods::to_duration(intensity), .phase = native_methods::to_duration(phase), .strict_mode = strict_mode};
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const native_methods::SilencerTarget target) const noexcept {
    return AUTDDatagramSilencerFromCompletionTime(*this, target);
  }
};

struct FixedUpdateRate {
  friend struct Silencer;

  uint16_t intensity;
  uint16_t phase;

  operator native_methods::FixedUpdateRate() const { return native_methods::FixedUpdateRate{.intensity = intensity, .phase = phase}; }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const native_methods::SilencerTarget target) const noexcept {
    return AUTDDatagramSilencerFromUpdateRate(*this, target);
  }
};

struct Silencer final : Datagram, IntoDatagramTuple<Silencer> {
  AUTD3_API [[nodiscard]] static Silencer disable() noexcept {
    return Silencer(
        FixedCompletionSteps{
            .intensity = 1,
            .phase = 1,
        },
        native_methods::SilencerTarget::Intensity);
  }

  Silencer() : Silencer(FixedCompletionSteps{}, native_methods::SilencerTarget::Intensity) {}
  explicit Silencer(const std::variant<FixedCompletionSteps, FixedCompletionTime, FixedUpdateRate>& config,
                    const native_methods::SilencerTarget target)
      : config(config), target(target) {}

  std::variant<FixedCompletionSteps, FixedCompletionTime, FixedUpdateRate> config;
  native_methods::SilencerTarget target;

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const noexcept override {
    return std::visit([this](const auto& c) { return c.ptr(target); }, config);
  }
};
}  // namespace autd3::driver
