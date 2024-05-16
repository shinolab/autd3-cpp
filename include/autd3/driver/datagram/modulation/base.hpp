#pragma once

#include "autd3/driver/datagram/with_segment_transition.hpp"
#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

template <class M>
class ModulationBase : public DatagramST<native_methods::ModulationPtr>, public IntoDatagramWithSegmentTransition<native_methods::ModulationPtr, M> {
 public:
  ModulationBase() : _loop_behavior(LoopBehavior::infinite()) {}   // LCOV_EXCL_LINE
  ModulationBase(const ModulationBase& obj) = default;             // LCOV_EXCL_LINE
  ModulationBase& operator=(const ModulationBase& obj) = default;  // LCOV_EXCL_LINE
  ModulationBase(ModulationBase&& obj) = default;                  // LCOV_EXCL_LINE
  ModulationBase& operator=(ModulationBase&& obj) = default;       // LCOV_EXCL_LINE
  virtual ~ModulationBase() = default;                             // LCOV_EXCL_LINE

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const { return AUTDModulationIntoDatagram(modulation_ptr()); }

  // LCOV_EXCL_START
  [[nodiscard]] native_methods::ModulationPtr raw_ptr(const geometry::Geometry&) const override { return modulation_ptr(); }
  [[nodiscard]] native_methods::DatagramPtr into_segment(const native_methods::ModulationPtr p,
                                                         const native_methods::Segment segment) const override {
    return AUTDModulationIntoDatagramWithSegment(p, segment);
  }
  [[nodiscard]] native_methods::DatagramPtr into_segment_transition(const native_methods::ModulationPtr p, const native_methods::Segment segment,
                                                                    const native_methods::TransitionModeWrap transition_mode) const override {
    return native_methods::AUTDModulationIntoDatagramWithSegmentTransition(p, segment, transition_mode);
  }
  [[nodiscard]] virtual native_methods::ModulationPtr modulation_ptr() const = 0;
  // LCOV_EXCL_STOP

  void with_loop_behavior(const native_methods::LoopBehavior loop_behavior) & { _loop_behavior = loop_behavior; }  // LCOV_EXCL_LINE
  [[nodiscard]] M&& with_loop_behavior(const native_methods::LoopBehavior loop_behavior) && {
    _loop_behavior = loop_behavior;
    return std::move(*static_cast<M*>(this));
  }

 protected:
  native_methods::LoopBehavior _loop_behavior;
};

}  // namespace autd3::driver
