#pragma once

#include <chrono>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

class Silencer final : public IntoDatagramTuple<Silencer>,
                       public IntoDatagramWithTimeout<Silencer>,
                       public IntoDatagramWithParallelThreshold<Silencer> {
 public:
  class FixedUpdateRate final : public IntoDatagramTuple<FixedUpdateRate>,
                                public IntoDatagramWithTimeout<FixedUpdateRate>,
                                public IntoDatagramWithParallelThreshold<FixedUpdateRate> {
   public:
    AUTD3_API explicit FixedUpdateRate(const uint16_t update_rate_intensity, const uint16_t update_rate_phase) noexcept
        : _update_rate_intensity(update_rate_intensity), _update_rate_phase(update_rate_phase), _target(native_methods::SilencerTarget::Intensity) {}

    AUTD3_DEF_PROP(uint16_t, update_rate_intensity)
    AUTD3_DEF_PROP(uint16_t, update_rate_phase)
    AUTD3_DEF_PARAM(FixedUpdateRate, native_methods::SilencerTarget, target)

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return native_methods::AUTDDatagramSilencerFromUpdateRate(_update_rate_intensity, _update_rate_phase, _target);
    }
  };

  AUTD3_API [[nodiscard]] static FixedUpdateRate from_update_rate(const uint16_t update_rate_intensity, const uint16_t update_rate_phase) noexcept {
    return FixedUpdateRate(update_rate_intensity, update_rate_phase);
  }

  template <typename Rep, typename Period>
  AUTD3_API [[nodiscard]] static Silencer from_completion_time(const std::chrono::duration<Rep, Period> steps_intensity,
                                                               const std::chrono::duration<Rep, Period> steps_phase) noexcept {
    return Silencer{steps_intensity, steps_phase};
  }

  AUTD3_API [[nodiscard]] static Silencer disable() noexcept {
    return from_completion_time(std::chrono::microseconds(25), std::chrono::microseconds(25));
  }

  Silencer() : Silencer(std::chrono::microseconds(250), std::chrono::microseconds(1000)) {}

  AUTD3_DEF_PROP(std::chrono::nanoseconds, completion_time_intensity)
  AUTD3_DEF_PROP(std::chrono::nanoseconds, completion_time_phase)
  AUTD3_DEF_PARAM(Silencer, bool, strict_mode)
  AUTD3_DEF_PARAM(Silencer, native_methods::SilencerTarget, target)

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
    return native_methods::AUTDDatagramSilencerFromCompletionTime(_completion_time_intensity.count(), _completion_time_phase.count(), _strict_mode,
                                                                  _target);
  }

 private:
  Silencer(const std::chrono::nanoseconds completion_time_intensity, const std::chrono::nanoseconds completion_time_phase)
      : _completion_time_intensity(completion_time_intensity),
        _completion_time_phase(completion_time_phase),
        _strict_mode(true),
        _target(native_methods::SilencerTarget::Intensity) {}
};

}  // namespace autd3::driver
