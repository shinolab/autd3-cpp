#pragma once

#include <optional>

#include "autd3/driver/firmware/fpga/transition_mode.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "tuple.hpp"

namespace autd3::driver {

template <class T>
concept datagram_l = requires(T t, const geometry::Geometry& geometry, native_methods::Segment segment,
                              native_methods::TransitionModeWrap transition_mode, const uint16_t loop_count) {
  { t.with_finite_loop(geometry, segment, transition_mode, loop_count) } -> std::same_as<native_methods::DatagramPtr>;
};

template <datagram_l L, finite_transition_mode_v T>
struct WithFiniteLoop final : Datagram, IntoDatagramTuple<WithFiniteLoop<L, T>> {
  AUTD3_API explicit WithFiniteLoop(L inner, const uint16_t loop_count, const native_methods::Segment segment, const T transition_mode)
      : inner(std::move(inner)), segment(segment), transition_mode(transition_mode), loop_count(loop_count) {}
  ~WithFiniteLoop() override = default;                            // LCOV_EXCL_LINE
  WithFiniteLoop(const WithFiniteLoop& v) noexcept = default;      // LCOV_EXCL_LINE
  WithFiniteLoop& operator=(const WithFiniteLoop& obj) = default;  // LCOV_EXCL_LINE
  WithFiniteLoop(WithFiniteLoop&& obj) = default;                  // LCOV_EXCL_LINE
  WithFiniteLoop& operator=(WithFiniteLoop&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    return inner.with_finite_loop(geometry, segment, transition_mode.inner(), loop_count);
  }

  L inner;
  native_methods::Segment segment;
  T transition_mode;
  uint16_t loop_count;
};

}  // namespace autd3::driver
