#pragma once

#include <vector>

#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

struct SamplingModeExact {
  driver::Freq<uint32_t> freq;

  [[nodiscard]] native_methods::ModulationPtr sine_ptr(const native_methods::SamplingConfigWrap config, const uint8_t intensity, const uint8_t offset,
                                                       const driver::Angle phase, const native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSineExact(freq.hz(), config, intensity, offset, phase.radian(), loop_behavior);
  }

  [[nodiscard]] static native_methods::ModulationPtr fourier_ptr(const std::vector<native_methods::ModulationPtr>& components,
                                                                 const native_methods::LoopBehavior loop_behavior) {
    return validate(AUTDModulationFourierExact(components.data(), static_cast<uint32_t>(components.size()), loop_behavior));
  }

  [[nodiscard]] static native_methods::ModulationPtr mixer_ptr(const std::vector<native_methods::ModulationPtr>& components,
                                                               const native_methods::LoopBehavior loop_behavior) {
    return validate(AUTDModulationMixerExact(components.data(), static_cast<uint32_t>(components.size()), loop_behavior));
  }

  [[nodiscard]] native_methods::ModulationPtr square_ptr(const native_methods::SamplingConfigWrap config, const uint8_t low, const uint8_t high,
                                                         const float duty, const native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSquareExact(freq.hz(), config, low, high, duty, loop_behavior);
  }
};

struct SamplingModeExactFloat {
  driver::Freq<float> freq;

  [[nodiscard]] native_methods::ModulationPtr sine_ptr(const native_methods::SamplingConfigWrap config, const uint8_t intensity, const uint8_t offset,
                                                       const driver::Angle phase, const native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSineExactFloat(freq.hz(), config, intensity, offset, phase.radian(), loop_behavior);
  }

  [[nodiscard]] static native_methods::ModulationPtr fourier_ptr(const std::vector<native_methods::ModulationPtr>& components,
                                                                 const native_methods::LoopBehavior loop_behavior) {
    return validate(AUTDModulationFourierExactFloat(components.data(), static_cast<uint32_t>(components.size()), loop_behavior));
  }

  [[nodiscard]] static native_methods::ModulationPtr mixer_ptr(const std::vector<native_methods::ModulationPtr>& components,
                                                               const native_methods::LoopBehavior loop_behavior) {
    return validate(AUTDModulationMixerExactFloat(components.data(), static_cast<uint32_t>(components.size()), loop_behavior));
  }

  [[nodiscard]] native_methods::ModulationPtr square_ptr(const native_methods::SamplingConfigWrap config, const uint8_t low, const uint8_t high,
                                                         const float duty, const native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSquareExactFloat(freq.hz(), config, low, high, duty, loop_behavior);
  }
};

struct SamplingModeNearest {
  driver::Freq<float> freq;

  [[nodiscard]] native_methods::ModulationPtr sine_ptr(const native_methods::SamplingConfigWrap config, const uint8_t intensity, const uint8_t offset,
                                                       const driver::Angle phase, const native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSineNearest(freq.hz(), config, intensity, offset, phase.radian(), loop_behavior);
  }

  [[nodiscard]] static native_methods::ModulationPtr fourier_ptr(const std::vector<native_methods::ModulationPtr>& components,
                                                                 const native_methods::LoopBehavior loop_behavior) {
    return validate(AUTDModulationFourierNearest(components.data(), static_cast<uint32_t>(components.size()), loop_behavior));
  }

  [[nodiscard]] static native_methods::ModulationPtr mixer_ptr(const std::vector<native_methods::ModulationPtr>& components,
                                                               const native_methods::LoopBehavior loop_behavior) {
    return validate(AUTDModulationMixerNearest(components.data(), static_cast<uint32_t>(components.size()), loop_behavior));
  }

  [[nodiscard]] native_methods::ModulationPtr square_ptr(const native_methods::SamplingConfigWrap config, const uint8_t low, const uint8_t high,
                                                         const float duty, const native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSquareNearest(freq.hz(), config, low, high, duty, loop_behavior);
  }
};

}  // namespace autd3::modulation
