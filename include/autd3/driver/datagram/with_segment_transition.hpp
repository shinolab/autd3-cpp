#pragma once

#include <memory>
#include <optional>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <typename P>
class DatagramST {
 public:
  DatagramST() = default;
  AUTD3_API virtual ~DatagramST() = default;
  DatagramST(const DatagramST& v) noexcept = default;
  DatagramST& operator=(const DatagramST& obj) = default;
  DatagramST(DatagramST&& obj) = default;
  DatagramST& operator=(DatagramST&& obj) = default;
  AUTD3_API [[nodiscard]] virtual P raw_ptr(const geometry::Geometry&) const = 0;
  AUTD3_API [[nodiscard]] virtual native_methods::DatagramPtr into_segment(P p, native_methods::Segment segment) const = 0;
  AUTD3_API [[nodiscard]] virtual native_methods::DatagramPtr into_segment_transition(P p, native_methods::Segment segment,
                                                                                      native_methods::TransitionModeWrap transition_mode) const = 0;
};

template <typename P>
class DatagramWithSegmentTransition {
 public:
  AUTD3_API explicit DatagramWithSegmentTransition(std::unique_ptr<DatagramST<P>> datagram, const native_methods::Segment segment,
                                                   std::optional<native_methods::TransitionModeWrap> transition_mode)
      : _datagram(std::move(datagram)), _segment(segment), _transition_mode(std::move(transition_mode)) {}
  ~DatagramWithSegmentTransition() = default;
  DatagramWithSegmentTransition(const DatagramWithSegmentTransition& v) noexcept = default;
  DatagramWithSegmentTransition& operator=(const DatagramWithSegmentTransition& obj) = default;
  DatagramWithSegmentTransition(DatagramWithSegmentTransition&& obj) = default;
  DatagramWithSegmentTransition& operator=(DatagramWithSegmentTransition&& obj) = default;

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& g) {
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
  IntoDatagramWithSegmentTransition() = default;
  virtual ~IntoDatagramWithSegmentTransition() = default;
  IntoDatagramWithSegmentTransition(const IntoDatagramWithSegmentTransition& v) noexcept = default;
  IntoDatagramWithSegmentTransition& operator=(const IntoDatagramWithSegmentTransition& obj) = default;
  IntoDatagramWithSegmentTransition(IntoDatagramWithSegmentTransition&& obj) = default;
  IntoDatagramWithSegmentTransition& operator=(IntoDatagramWithSegmentTransition&& obj) = default;

  AUTD3_API [[nodiscard]] DatagramWithSegmentTransition<P> with_segment(const native_methods::Segment segment,
                                                                        std::optional<native_methods::TransitionModeWrap> transition_mode) & {
    return DatagramWithSegmentTransition<P>(std::make_unique<D>(*static_cast<D*>(this)), segment, std::move(transition_mode));
  }
  AUTD3_API [[nodiscard]] DatagramWithSegmentTransition<P> with_segment(const native_methods::Segment segment,
                                                                        std::optional<native_methods::TransitionModeWrap> transition_mode) && {
    return DatagramWithSegmentTransition<P>(std::make_unique<D>((std::move(*static_cast<D*>(this)))), segment, std::move(transition_mode));
  }
};

}  // namespace autd3::driver
