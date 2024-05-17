#pragma once

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

class Silencer final {
 public:
  class FixedUpdateRate final {
   public:
    AUTD3_API explicit FixedUpdateRate(const uint16_t update_rate_intensity, const uint16_t update_rate_phase) noexcept
        : _update_rate_intensity(update_rate_intensity), _update_rate_phase(update_rate_phase) {}

    AUTD3_DEF_PROP(uint16_t, update_rate_intensity)
    AUTD3_DEF_PROP(uint16_t, update_rate_phase)

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return validate(native_methods::AUTDDatagramSilencerFixedUpdateRate(_update_rate_intensity, _update_rate_phase));
    }
  };

  class FixedCompletionSteps final {
   public:
    AUTD3_API explicit FixedCompletionSteps(const uint16_t steps_intensity, const uint16_t steps_phase) noexcept
        : _step_intensity(steps_intensity), _step_phase(steps_phase), _strict_mode(true) {}

    AUTD3_DEF_PROP(uint16_t, step_intensity)
    AUTD3_DEF_PROP(uint16_t, step_phase)
    AUTD3_DEF_PARAM(FixedCompletionSteps, bool, strict_mode)

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return validate(native_methods::AUTDDatagramSilencerFixedCompletionSteps(_step_intensity, _step_phase, _strict_mode));
    }
  };

  AUTD3_API [[nodiscard]] static FixedUpdateRate fixed_update_rate(const uint16_t update_rate_intensity, const uint16_t update_rate_phase) noexcept {
    return FixedUpdateRate(update_rate_intensity, update_rate_phase);
  }

  AUTD3_API [[nodiscard]] static FixedCompletionSteps fixed_completion_steps(const uint16_t steps_intensity, const uint16_t steps_phase) noexcept {
    return FixedCompletionSteps(steps_intensity, steps_phase);
  }

  AUTD3_API [[nodiscard]] static FixedCompletionSteps disable() noexcept { return fixed_completion_steps(1, 1); }

  AUTD3_API [[nodiscard]] static FixedCompletionSteps default_() noexcept { return fixed_completion_steps(10, 40); }
};

}  // namespace autd3::driver
