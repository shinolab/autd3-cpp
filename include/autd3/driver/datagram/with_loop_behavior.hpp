#pragma once

#include <optional>

#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/driver/firmware/fpga/transition_mode.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "tuple.hpp"

namespace autd3::driver {

template <class T>
concept datagram_l = requires(T t, const geometry::Geometry& geometry, native_methods::Segment segment,
                              native_methods::TransitionModeWrap transition_mode, native_methods::LoopBehavior loop_behavior) {
  { t.with_loop_behavior(geometry, segment, transition_mode, loop_behavior) } -> std::same_as<native_methods::DatagramPtr>;
};

template <datagram_l L>
struct WithLoopBehavior final : Datagram, IntoDatagramTuple<WithLoopBehavior<L>> {
  AUTD3_API explicit WithLoopBehavior(L inner, LoopBehavior loop_behavior, const native_methods::Segment segment,
                                      const std::optional<TransitionMode>& transition_mode)
      : inner(std::move(inner)), segment(segment), transition_mode(transition_mode), loop_behavior(loop_behavior) {}
  ~WithLoopBehavior() override = default;                              // LCOV_EXCL_LINE
  WithLoopBehavior(const WithLoopBehavior& v) noexcept = default;      // LCOV_EXCL_LINE
  WithLoopBehavior& operator=(const WithLoopBehavior& obj) = default;  // LCOV_EXCL_LINE
  WithLoopBehavior(WithLoopBehavior&& obj) = default;                  // LCOV_EXCL_LINE
  WithLoopBehavior& operator=(WithLoopBehavior&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    const auto transition = transition_mode.has_value() ? transition_mode.value() : native_methods::AUTDTransitionModeNone();
    return inner.with_loop_behavior(geometry, segment, transition, loop_behavior);
  }

  L inner;
  native_methods::Segment segment;
  std::optional<TransitionMode> transition_mode;
  LoopBehavior loop_behavior;
};

}  // namespace autd3::driver
