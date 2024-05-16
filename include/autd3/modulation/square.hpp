#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

#define AUTD3_DEF_MOD_SQUARE(T, F)                                                                                 \
  class T final : public driver::Modulation<T> {                                                                   \
   public:                                                                                                         \
    explicit T(const F freq)                                                                                       \
        : Modulation(driver::SamplingConfig::Division(5120)),                                                      \
          _freq(freq),                                                                                             \
          _low(std::numeric_limits<driver::EmitIntensity>::min()),                                                 \
          _high(std::numeric_limits<driver::EmitIntensity>::max()),                                                \
          _duty(0.5) {}                                                                                            \
    AUTD3_DEF_PROP(F, freq)                                                                                        \
    AUTD3_DEF_PARAM(T, driver::EmitIntensity, low)                                                                 \
    AUTD3_DEF_PARAM(T, driver::EmitIntensity, high)                                                                \
    AUTD3_DEF_PARAM(T, double, duty)                                                                               \
    [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry&) const override { \
      return AUTDModulation##T(_freq.hz(), _config, _low.value(), _high.value(), _duty, _loop_behavior);           \
    }                                                                                                              \
  };

AUTD3_DEF_MOD_SQUARE(SquareExact, driver::Freq<uint32_t>)
AUTD3_DEF_MOD_SQUARE(SquareExactFloat, driver::Freq<double>)
AUTD3_DEF_MOD_SQUARE(SquareNearest, driver::Freq<double>)

#undef AUTD3_DEF_MOD_SQUARE

struct Square final {
  static SquareExact create(const driver::Freq<uint32_t> freq) { return SquareExact(freq); };
  static SquareExactFloat create(const driver::Freq<double> freq) { return SquareExactFloat(freq); };
  static SquareNearest with_freq_nearest(const driver::Freq<double> freq) { return SquareNearest(freq); };
};

}  // namespace autd3::modulation
