#pragma once

#include <optional>

#include "autd3/driver/firmware/fpga/transition_mode.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "tuple.hpp"

namespace autd3::driver {

template <class T>
concept datagram_s =
    requires(T t, const geometry::Geometry& geometry, native_methods::Segment segment, native_methods::TransitionModeWrap transition_mode) {
      { t.with_segment_transition(geometry, segment, transition_mode) } -> std::same_as<native_methods::DatagramPtr>;
    };

template <datagram_s P>
struct WithSegment final : Datagram, IntoDatagramTuple<WithSegment<P>> {
  AUTD3_API explicit WithSegment(P inner, const native_methods::Segment segment, const std::optional<TransitionMode>& transition_mode)
      : inner(std::move(inner)), segment(segment), transition_mode(transition_mode) {}
  ~WithSegment() override = default;                         // LCOV_EXCL_LINE
  WithSegment(const WithSegment& v) noexcept = default;      // LCOV_EXCL_LINE
  WithSegment& operator=(const WithSegment& obj) = default;  // LCOV_EXCL_LINE
  WithSegment(WithSegment&& obj) = default;                  // LCOV_EXCL_LINE
  WithSegment& operator=(WithSegment&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    const auto transition = transition_mode.has_value() ? transition_mode.value() : native_methods::AUTDTransitionModeNone();
    return inner.with_segment_transition(geometry, segment, transition);
  }

  P inner;
  native_methods::Segment segment;
  std::optional<TransitionMode> transition_mode;
};

}  // namespace autd3::driver
