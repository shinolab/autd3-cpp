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
  ModulationBase() : _loop_behavior(LoopBehavior::infinite()) {}
  ModulationBase(const ModulationBase& obj) = default;
  ModulationBase& operator=(const ModulationBase& obj) = default;
  ModulationBase(ModulationBase&& obj) = default;
  ModulationBase& operator=(ModulationBase&& obj) = default;
  virtual ~ModulationBase() = default;

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDModulationIntoDatagram(modulation_ptr(geometry));
  }

  [[nodiscard]] native_methods::ModulationPtr raw_ptr(const geometry::Geometry& geometry) const override { return modulation_ptr(geometry); }
  [[nodiscard]] native_methods::DatagramPtr into_segment(const native_methods::ModulationPtr p,
                                                         const native_methods::Segment segment) const override {
    return AUTDModulationIntoDatagramWithSegment(p, segment);
  }
  [[nodiscard]] native_methods::DatagramPtr into_segment_transition(const native_methods::ModulationPtr p, const native_methods::Segment segment,
                                                                    const native_methods::TransitionModeWrap transition_mode) const override {
    return native_methods::AUTDModulationIntoDatagramWithSegmentTransition(p, segment, transition_mode);
  }
  [[nodiscard]] virtual native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry&) const = 0;

  void with_loop_behavior(const native_methods::LoopBehavior loop_behavior) & { _loop_behavior = loop_behavior; }
  [[nodiscard]] M&& with_loop_behavior(const native_methods::LoopBehavior loop_behavior) && {
    _loop_behavior = loop_behavior;
    return std::move(*static_cast<M*>(this));
  }

  [[nodiscard]] native_methods::LoopBehavior loop_behavior() const noexcept { return _loop_behavior; }

 protected:
  native_methods::LoopBehavior _loop_behavior;
};

}  // namespace autd3::driver
