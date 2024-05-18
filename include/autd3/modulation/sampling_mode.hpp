#pragma once

#include <vector>

#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

struct SamplingModeExact {
  driver::Freq<uint32_t> freq;

  native_methods::ModulationPtr sine_ptr(native_methods::SamplingConfigWrap config, driver::EmitIntensity intensity, driver::EmitIntensity offset,
                                         driver::Angle phase, native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSineExact(freq.hz(), config, intensity.value(), offset.value(), phase.to_radian(), loop_behavior);
  }

  native_methods::ModulationPtr fourier_ptr(std::vector<native_methods::ModulationPtr> components, native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationFourierExact(components.data(), static_cast<uint32_t>(components.size()), loop_behavior);
  }

  native_methods::ModulationPtr square_ptr(native_methods::SamplingConfigWrap config, driver::EmitIntensity low, driver::EmitIntensity high,
                                           double duty, native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSquareExact(freq.hz(), config, low.value(), high.value(), duty, loop_behavior);
  }
};

struct SamplingModeExactFloat {
  driver::Freq<double> freq;
  native_methods::ModulationPtr sine_ptr(native_methods::SamplingConfigWrap config, driver::EmitIntensity intensity, driver::EmitIntensity offset,
                                         driver::Angle phase, native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSineExactFloat(freq.hz(), config, intensity.value(), offset.value(), phase.to_radian(), loop_behavior);
  }

  native_methods::ModulationPtr fourier_ptr(std::vector<native_methods::ModulationPtr> components, native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationFourierExactFloat(components.data(), static_cast<uint32_t>(components.size()), loop_behavior);
  }

  native_methods::ModulationPtr square_ptr(native_methods::SamplingConfigWrap config, driver::EmitIntensity low, driver::EmitIntensity high,
                                           double duty, native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSquareExactFloat(freq.hz(), config, low.value(), high.value(), duty, loop_behavior);
  }
};

struct SamplingModeNearest {
  driver::Freq<double> freq;
  native_methods::ModulationPtr sine_ptr(native_methods::SamplingConfigWrap config, driver::EmitIntensity intensity, driver::EmitIntensity offset,
                                         driver::Angle phase, native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSineNearest(freq.hz(), config, intensity.value(), offset.value(), phase.to_radian(), loop_behavior);
  }

  native_methods::ModulationPtr fourier_ptr(std::vector<native_methods::ModulationPtr> components, native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationFourierNearest(components.data(), static_cast<uint32_t>(components.size()), loop_behavior);
  }

  native_methods::ModulationPtr square_ptr(native_methods::SamplingConfigWrap config, driver::EmitIntensity low, driver::EmitIntensity high,
                                           double duty, native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationSquareNearest(freq.hz(), config, low.value(), high.value(), duty, loop_behavior);
  }
};

}  // namespace autd3::modulation
