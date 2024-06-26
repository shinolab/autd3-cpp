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
        : _update_rate_intensity(update_rate_intensity), _update_rate_phase(update_rate_phase) {}

    AUTD3_DEF_PROP(uint16_t, update_rate_intensity)
    AUTD3_DEF_PROP(uint16_t, update_rate_phase)

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return native_methods::AUTDDatagramSilencerFromUpdateRate(_update_rate_intensity, _update_rate_phase);
    }
  };

  class FixedCompletionTime final : public IntoDatagramTuple<FixedCompletionTime>,
                                    public IntoDatagramWithTimeout<FixedCompletionTime>,
                                    public IntoDatagramWithParallelThreshold<FixedCompletionTime> {
   public:
    AUTD3_API explicit FixedCompletionTime(const std::chrono::nanoseconds completion_time_intensity,
                                           const std::chrono::nanoseconds completion_time_phase) noexcept
        : _completion_time_intensity(completion_time_intensity), _completion_time_phase(completion_time_phase), _strict_mode(true) {}

    AUTD3_DEF_PROP(std::chrono::nanoseconds, completion_time_intensity)
    AUTD3_DEF_PROP(std::chrono::nanoseconds, completion_time_phase)
    AUTD3_DEF_PARAM(FixedCompletionTime, bool, strict_mode)

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return native_methods::AUTDDatagramSilencerFromCompletionTime(_completion_time_intensity.count(), _completion_time_phase.count(), _strict_mode);
    }
  };

  AUTD3_API [[nodiscard]] static FixedUpdateRate from_update_rate(const uint16_t update_rate_intensity, const uint16_t update_rate_phase) noexcept {
    return FixedUpdateRate(update_rate_intensity, update_rate_phase);
  }

  AUTD3_API [[nodiscard]] static Silencer from_completion_steps(const uint16_t steps_intensity, const uint16_t steps_phase) noexcept {
    return {steps_intensity, steps_phase};
  }

  template <typename Rep, typename Period>
  AUTD3_API [[nodiscard]] static FixedCompletionTime from_completion_time(const std::chrono::duration<Rep, Period> steps_intensity,
                                                                          const std::chrono::duration<Rep, Period> steps_phase) noexcept {
    return FixedCompletionTime{steps_intensity, steps_phase};
  }

  AUTD3_API [[nodiscard]] static Silencer disable() noexcept { return from_completion_steps(1, 1); }

  Silencer() : Silencer(10, 40) {}

  AUTD3_DEF_PROP(uint16_t, step_intensity)
  AUTD3_DEF_PROP(uint16_t, step_phase)
  AUTD3_DEF_PARAM(Silencer, bool, strict_mode)

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
    return native_methods::AUTDDatagramSilencerFromCompletionSteps(_step_intensity, _step_phase, _strict_mode);
  }

 private:
  Silencer(const uint16_t steps_intensity, const uint16_t steps_phase)
      : _step_intensity(steps_intensity), _step_phase(steps_phase), _strict_mode(true) {}
};

}  // namespace autd3::driver
