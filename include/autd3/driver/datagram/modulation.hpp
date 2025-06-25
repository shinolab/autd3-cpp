#pragma once

#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class Modulation : Datagram {
 public:
  Modulation() = default;
  Modulation(const Modulation& obj) = default;             // LCOV_EXCL_LINE
  Modulation& operator=(const Modulation& obj) = default;  // LCOV_EXCL_LINE
  Modulation(Modulation&& obj) = default;                  // LCOV_EXCL_LINE
  Modulation& operator=(Modulation&& obj) = default;       // LCOV_EXCL_LINE
  ~Modulation() override = default;                        // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
    return AUTDModulationIntoDatagram(modulation_ptr());
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr with_segment_transition(const geometry::Geometry&, const native_methods::Segment segment,
                                                                              const native_methods::TransitionModeWrap transition_mode) const {
    return AUTDModulationIntoDatagramWithSegment(modulation_ptr(), segment, transition_mode);
  }
  AUTD3_API [[nodiscard]] native_methods::DatagramPtr with_loop_behavior(const geometry::Geometry&, const native_methods::Segment segment,
                                                                         const native_methods::TransitionModeWrap transition_mode,
                                                                         const native_methods::LoopBehavior loop_behavior) const {
    return AUTDModulationIntoDatagramWithLoopBehavior(modulation_ptr(), segment, transition_mode, loop_behavior);
  }

  AUTD3_API [[nodiscard]] virtual native_methods::ModulationPtr modulation_ptr() const = 0;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const { return SamplingConfig{AUTDModulationSamplingConfig(modulation_ptr())}; }
};

}  // namespace autd3::driver
