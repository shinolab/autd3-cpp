#pragma once

#include <memory>
#include <optional>

#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <typename P>
class DatagramST {
 public:
  virtual ~DatagramST() = default;  // LCOV_EXCL_LINE
  virtual P raw_ptr(const geometry::Geometry&) const = 0;
  virtual native_methods::DatagramPtr into_segment(const P p, const native_methods::Segment segment) const = 0;
  virtual native_methods::DatagramPtr into_segment_transition(const P p, const native_methods::Segment segment,
                                                              const native_methods::TransitionModeWrap transition_mode) const = 0;
};

template <typename P>
class DatagramWithSegmentTransition {
 public:
  explicit DatagramWithSegmentTransition(std::unique_ptr<DatagramST<P>> datagram, const native_methods::Segment segment,
                                         const std::optional<native_methods::TransitionModeWrap> transition_mode)
      : _datagram(std::move(datagram)), _segment(segment), _transition_mode(transition_mode) {}
  ~DatagramWithSegmentTransition() = default;                                                    // LCOV_EXCL_LINE
  DatagramWithSegmentTransition(const DatagramWithSegmentTransition& v) noexcept = default;      // LCOV_EXCL_LINE
  DatagramWithSegmentTransition& operator=(const DatagramWithSegmentTransition& obj) = default;  // LCOV_EXCL_LINE
  DatagramWithSegmentTransition(DatagramWithSegmentTransition&& obj) = default;                  // LCOV_EXCL_LINE
  DatagramWithSegmentTransition& operator=(DatagramWithSegmentTransition&& obj) = default;       // LCOV_EXCL_LINE

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& g) {
    auto raw_ptr = _datagram->raw_ptr(g);
    return _transition_mode.has_value() ? _datagram->into_segment_transition(raw_ptr, _segment, _transition_mode.value())
                                        : _datagram->into_segment(raw_ptr, _segment);
  }

 private:
  std::unique_ptr<DatagramST<P>> _datagram;
  native_methods::Segment _segment;
  std::optional<native_methods::TransitionModeWrap> _transition_mode;
};

template <typename P, class D>
class IntoDatagramWithSegmentTransition {
 public:
  virtual ~IntoDatagramWithSegmentTransition() = default;  // LCOV_EXCL_LINE

  [[nodiscard]] DatagramWithSegmentTransition<P> with_segment(const native_methods::Segment segment,
                                                              const std::optional<native_methods::TransitionModeWrap> transition_mode) & {
    return DatagramWithSegmentTransition<P>(std::make_unique<D>(*static_cast<D*>(this)), segment, transition_mode);
  }
  [[nodiscard]] DatagramWithSegmentTransition<P> with_segment(const native_methods::Segment segment,
                                                              const std::optional<native_methods::TransitionModeWrap> transition_mode) && {
    return DatagramWithSegmentTransition<P>(std::make_unique<D>((std::move(*static_cast<D*>(this)))), segment, transition_mode);
  }
};

}  // namespace autd3::driver