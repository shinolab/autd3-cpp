#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class T>
class Fourier;

#define AUTD3_DEF_MOD_SINE(T, N, F)                                                                                          \
  class T final : public driver::Modulation<T> {                                                                             \
   public:                                                                                                                   \
    AUTD3_API explicit T(const F freq)                                                                                       \
        : Modulation(driver::SamplingConfig::Division(5120)),                                                                \
          _freq(freq),                                                                                                       \
          _intensity(std::numeric_limits<driver::EmitIntensity>::max()),                                                     \
          _offset(std::numeric_limits<driver::EmitIntensity>::max() / 2),                                                    \
          _phase(0.0 * driver::rad) {}                                                                                       \
    AUTD3_DEF_PROP(F, freq)                                                                                                  \
    AUTD3_DEF_PARAM(T, driver::EmitIntensity, intensity)                                                                     \
    AUTD3_DEF_PARAM(T, driver::EmitIntensity, offset)                                                                        \
    AUTD3_DEF_PARAM(T, driver::Angle, phase)                                                                                 \
    AUTD3_API friend Fourier<T> operator+(T&& lhs, const T& rhs);                                                            \
    AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry&) const override { \
      return N(_freq.hz(), _config, _intensity.value(), _offset.value(), _phase.to_radian(), _loop_behavior);                \
    }                                                                                                                        \
  };

AUTD3_DEF_MOD_SINE(SineExact, AUTDModulationSineExact, driver::Freq<uint32_t>)
AUTD3_DEF_MOD_SINE(SineExactFloat, AUTDModulationSineExactFloat, driver::Freq<double>)
AUTD3_DEF_MOD_SINE(SineNearest, AUTDModulationSineNearest, driver::Freq<double>)

#undef AUTD3_DEF_MOD_SINE

struct Sine final {
  AUTD3_API [[nodiscard]] static SineExact create(const driver::Freq<uint32_t> freq) { return SineExact(freq); }
  AUTD3_API [[nodiscard]] static SineExactFloat create(const driver::Freq<double> freq) { return SineExactFloat(freq); }
  AUTD3_API [[nodiscard]] static SineNearest with_freq_nearest(const driver::Freq<double> freq) { return SineNearest(freq); }
};

}  // namespace autd3::modulation
