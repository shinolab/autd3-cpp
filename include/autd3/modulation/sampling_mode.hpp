#pragma once

#include <limits>
#include <vector>

#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

struct SamplingModeExact {
  driver::Freq<uint32_t> freq;

  [[nodiscard]] native_methods::ModulationPtr sine_ptr(const native_methods::SamplingConfig config, const uint8_t intensity, const uint8_t offset,
                                                       const driver::Angle phase, const bool clamp,
                                                       const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSineExact(freq.hz(), config, intensity, offset, phase.radian(), clamp, loop_behavior));
  }

  [[nodiscard]] static native_methods::ModulationPtr fourier_ptr(const std::vector<native_methods::ModulationPtr>& components, const bool clamp,
                                                                 const std::optional<float> scale_factor,
                                                                 const native_methods::LoopBehavior loop_behavior) {
    return validate(AUTDModulationFourierExact(components.data(), static_cast<uint32_t>(components.size()), clamp,
                                               scale_factor.value_or(std::numeric_limits<float>::quiet_NaN()), loop_behavior));
  }

  [[nodiscard]] native_methods::ModulationPtr square_ptr(const native_methods::SamplingConfig config, const uint8_t low, const uint8_t high,
                                                         const float duty, const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSquareExact(freq.hz(), config, low, high, duty, loop_behavior));
  }

  [[nodiscard]] float sine_freq(native_methods::ModulationPtr ptr) const {
    return static_cast<float>(native_methods::AUTDModulationSineExactFreq(ptr));
  }

  [[nodiscard]] float square_freq(native_methods::ModulationPtr ptr) const {
    return static_cast<float>(native_methods::AUTDModulationSquareExactFreq(ptr));
  }
};

struct SamplingModeExactFloat {
  driver::Freq<float> freq;

  [[nodiscard]] native_methods::ModulationPtr sine_ptr(const native_methods::SamplingConfig config, const uint8_t intensity, const uint8_t offset,
                                                       const driver::Angle phase, const bool clamp,
                                                       const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSineExactFloat(freq.hz(), config, intensity, offset, phase.radian(), clamp, loop_behavior));
  }

  [[nodiscard]] static native_methods::ModulationPtr fourier_ptr(const std::vector<native_methods::ModulationPtr>& components, const bool clamp,
                                                                 const std::optional<float> scale_factor,
                                                                 const native_methods::LoopBehavior loop_behavior) {
    return validate(AUTDModulationFourierExactFloat(components.data(), static_cast<uint32_t>(components.size()), clamp,
                                                    scale_factor.value_or(std::numeric_limits<float>::quiet_NaN()), loop_behavior));
  }

  [[nodiscard]] native_methods::ModulationPtr square_ptr(const native_methods::SamplingConfig config, const uint8_t low, const uint8_t high,
                                                         const float duty, const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSquareExactFloat(freq.hz(), config, low, high, duty, loop_behavior));
  }

  [[nodiscard]] float sine_freq(native_methods::ModulationPtr ptr) const { return native_methods::AUTDModulationSineExactFloatFreq(ptr); }

  [[nodiscard]] float square_freq(native_methods::ModulationPtr ptr) const { return native_methods::AUTDModulationSquareExactFloatFreq(ptr); }
};

struct SamplingModeNearest {
  driver::Freq<float> freq;

  [[nodiscard]] native_methods::ModulationPtr sine_ptr(const native_methods::SamplingConfig config, const uint8_t intensity, const uint8_t offset,
                                                       const driver::Angle phase, const bool clamp,
                                                       const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSineNearest(freq.hz(), config, intensity, offset, phase.radian(), clamp, loop_behavior));
  }

  [[nodiscard]] static native_methods::ModulationPtr fourier_ptr(const std::vector<native_methods::ModulationPtr>& components, const bool clamp,
                                                                 const std::optional<float> scale_factor,
                                                                 const native_methods::LoopBehavior loop_behavior) {
    return validate(AUTDModulationFourierNearest(components.data(), static_cast<uint32_t>(components.size()), clamp,
                                                 scale_factor.value_or(std::numeric_limits<float>::quiet_NaN()), loop_behavior));
  }

  [[nodiscard]] native_methods::ModulationPtr square_ptr(const native_methods::SamplingConfig config, const uint8_t low, const uint8_t high,
                                                         const float duty, const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSquareNearest(freq.hz(), config, low, high, duty, loop_behavior));
  }

  [[nodiscard]] float sine_freq(native_methods::ModulationPtr ptr) const { return native_methods::AUTDModulationSineNearestFreq(ptr); }

  [[nodiscard]] float square_freq(native_methods::ModulationPtr ptr) const { return native_methods::AUTDModulationSquareNearestFreq(ptr); }
};

}  // namespace autd3::modulation
