#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class T>
class Fourier;

#define AUTD3_DEF_MOD_SINE(T, F)                                                                                              \
  class T final : public driver::Modulation<T> {                                                                              \
   public:                                                                                                                    \
    explicit T(const F freq)                                                                                                  \
        : Modulation(driver::SamplingConfig::Division(5120)),                                                                 \
          _freq(freq),                                                                                                        \
          _intensity(std::numeric_limits<uint8_t>::max()),                                                                    \
          _offset(std::numeric_limits<uint8_t>::max() / 2),                                                                   \
          _phase(0.0 * driver::rad) {}                                                                                        \
    AUTD3_DEF_PROP(F, freq)                                                                                                   \
    AUTD3_DEF_PARAM(T, uint8_t, intensity)                                                                                    \
    AUTD3_DEF_PARAM(T, uint8_t, offset)                                                                                       \
    AUTD3_DEF_PARAM(T, driver::Angle, phase)                                                                                  \
    friend Fourier<T> operator+(T&& lhs, const T& rhs);                                                                       \
    [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry&) const override {            \
      return native_methods::AUTDModulation##T(_freq.hz(), _config, _intensity, _offset, _phase.to_radian(), _loop_behavior); \
    }                                                                                                                         \
  };

AUTD3_DEF_MOD_SINE(SineExact, driver::Freq<uint32_t>)
AUTD3_DEF_MOD_SINE(SineExactFloat, driver::Freq<double>)
AUTD3_DEF_MOD_SINE(SineNearest, driver::Freq<double>)

#undef AUTD3_DEF_MOD_SINE

struct Sine final {
  static SineExact create(const driver::Freq<uint32_t> freq) { return SineExact(freq); };
  static SineExactFloat create(const driver::Freq<double> freq) { return SineExactFloat(freq); };
  static SineNearest with_freq_nearest(const driver::Freq<double> freq) { return SineNearest(freq); };
};

}  // namespace autd3::modulation
