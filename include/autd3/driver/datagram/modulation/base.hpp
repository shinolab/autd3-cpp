#pragma once

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_segment_transition.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class M>
class ModulationBase : public DatagramST<native_methods::ModulationPtr>,
                       public IntoDatagramTuple<M>,
                       public IntoDatagramWithSegmentTransition<native_methods::ModulationPtr, M>,
                       public driver::IntoDatagramWithTimeout<M>,
                       public driver::IntoDatagramWithParallelThreshold<M> {
 public:
  AUTD3_API ModulationBase() : _loop_behavior(LoopBehavior::Infinite) {}
  ModulationBase(const ModulationBase& obj) = default;             // LCOV_EXCL_LINE
  ModulationBase& operator=(const ModulationBase& obj) = default;  // LCOV_EXCL_LINE
  ModulationBase(ModulationBase&& obj) = default;                  // LCOV_EXCL_LINE
  ModulationBase& operator=(ModulationBase&& obj) = default;       // LCOV_EXCL_LINE
  ~ModulationBase() override = default;                            // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDModulationIntoDatagram(modulation_ptr(geometry));
  }

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr raw_ptr(const geometry::Geometry& geometry) const override {
    return modulation_ptr(geometry);
  }
  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment(const native_methods::ModulationPtr p,
                                                                   const native_methods::Segment segment) const override {
    return AUTDModulationIntoDatagramWithSegment(p, segment);
  }
  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment_transition(
      const native_methods::ModulationPtr p, const native_methods::Segment segment,
      const native_methods::TransitionModeWrap transition_mode) const override {
    return AUTDModulationIntoDatagramWithSegmentTransition(p, segment, transition_mode);
  }
  AUTD3_API [[nodiscard]] virtual native_methods::ModulationPtr modulation_ptr(const geometry::Geometry&) const = 0;

  void with_loop_behavior(const native_methods::LoopBehavior loop_behavior) & { _loop_behavior = loop_behavior; }
  AUTD3_API [[nodiscard]] M&& with_loop_behavior(const native_methods::LoopBehavior loop_behavior) && {
    _loop_behavior = loop_behavior;
    return std::move(*static_cast<M*>(this));
  }

  AUTD3_API [[nodiscard]] native_methods::LoopBehavior loop_behavior() const noexcept { return _loop_behavior; }

 protected:
  native_methods::LoopBehavior _loop_behavior;
};

}  // namespace autd3::driver
