#pragma once

#include <chrono>
#include <optional>
#include <variant>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

class Silencer;

template <class C>
concept with_samplng_config = requires(const C& c) {
  { c.sampling_config_intensity() } -> std::same_as<std::optional<SamplingConfig>>;
  { c.sampling_config_phase() } -> std::same_as<std::optional<SamplingConfig>>;
};

struct FixedCompletionTime {
  friend class Silencer;

  std::chrono::nanoseconds intensity;
  std::chrono::nanoseconds phase;

 private:
  template <with_samplng_config C>
  AUTD3_API [[nodiscard]] bool is_valid(const C& c, const bool strict_mode, const native_methods::SilencerTarget target) const noexcept {
    return AUTDDatagramSilencerFixedCompletionTimeIsValid(raw_ptr(strict_mode, target),
                                                          c.sampling_config_intensity().value_or(SamplingConfig(0xFFFF)),
                                                          c.sampling_config_phase().value_or(SamplingConfig(0xFFFF)));
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr raw_ptr(const bool strict_mode, const native_methods::SilencerTarget target) const noexcept {
    return AUTDDatagramSilencerFromCompletionTime(intensity.count(), phase.count(), strict_mode, target);
  }
};

struct FixedUpdateRate {
  friend class Silencer;

  uint16_t intensity;
  uint16_t phase;

 private:
  template <with_samplng_config C>
  AUTD3_API [[nodiscard]] bool is_valid(const C& c, const bool strict_mode, const native_methods::SilencerTarget target) const noexcept {
    return AUTDDatagramSilencerFixedUpdateRateIsValid(raw_ptr(strict_mode, target), c.sampling_config_intensity().value_or(SamplingConfig(0xFFFF)),
                                                      c.sampling_config_phase().value_or(SamplingConfig(0xFFFF)));
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr raw_ptr(const bool, const native_methods::SilencerTarget target) const noexcept {
    return AUTDDatagramSilencerFromUpdateRate(intensity, phase, target);
  }
};

class Silencer final : public IntoDatagramTuple<Silencer>,
                       public IntoDatagramWithTimeout<Silencer>,
                       public IntoDatagramWithParallelThreshold<Silencer> {
 public:
  AUTD3_API [[nodiscard]] static Silencer disable() noexcept {
    return Silencer(FixedCompletionTime{
        .intensity{std::chrono::microseconds(25)},
        .phase{std::chrono::microseconds(25)},
    });
  }

  Silencer()
      : Silencer(FixedCompletionTime{
            .intensity{std::chrono::microseconds(250)},
            .phase{std::chrono::microseconds(1000)},
        }) {}
  explicit Silencer(std::variant<FixedCompletionTime, FixedUpdateRate> s)
      : _strict_mode(true), _target(native_methods::SilencerTarget::Intensity), _inner(std::move(s)) {}

  AUTD3_DEF_PARAM(Silencer, bool, strict_mode)
  AUTD3_DEF_PARAM(Silencer, native_methods::SilencerTarget, target)

  template <with_samplng_config C>
  AUTD3_API [[nodiscard]] bool is_valid(const C& c) const noexcept {
    return std::visit([this, &c](const auto& s) { return s.is_valid(c, _strict_mode, _target); }, _inner);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const noexcept {
    return std::visit([this](const auto& s) { return s.raw_ptr(_strict_mode, _target); }, _inner);
  }

 private:
  std::variant<FixedCompletionTime, FixedUpdateRate> _inner;
};

}  // namespace autd3::driver
