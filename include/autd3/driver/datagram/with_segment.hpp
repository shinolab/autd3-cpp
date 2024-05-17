#pragma once

#include <memory>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <typename P>
class DatagramS {
 public:
  DatagramS() = default;
  AUTD3_API virtual ~DatagramS() = default;
  DatagramS(const DatagramS& v) noexcept = default;
  DatagramS& operator=(const DatagramS& obj) = default;
  DatagramS(DatagramS&& obj) = default;
  DatagramS& operator=(DatagramS&& obj) = default;
  AUTD3_API [[nodiscard]] virtual P raw_ptr(const geometry::Geometry&) const = 0;
  AUTD3_API [[nodiscard]] virtual native_methods::DatagramPtr into_segment(P p, native_methods::Segment segment, bool transition) const = 0;
};

template <typename P>
class DatagramWithSegment {
 public:
  AUTD3_API explicit DatagramWithSegment(std::unique_ptr<DatagramS<P>> datagram, const native_methods::Segment segment, const bool transition)
      : _datagram(std::move(datagram)), _segment(segment), _transition(transition) {}
  ~DatagramWithSegment() = default;
  DatagramWithSegment(const DatagramWithSegment& v) noexcept = default;
  DatagramWithSegment& operator=(const DatagramWithSegment& obj) = default;
  DatagramWithSegment(DatagramWithSegment&& obj) = default;
  DatagramWithSegment& operator=(DatagramWithSegment&& obj) = default;

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& g) {
    auto raw_ptr = _datagram->raw_ptr(g);
    return _datagram->into_segment(raw_ptr, _segment, _transition);
  }

 private:
  std::unique_ptr<DatagramS<P>> _datagram;
  native_methods::Segment _segment;
  bool _transition;
};

template <typename P, class D>
class IntoDatagramWithSegment {
 public:
  IntoDatagramWithSegment() = default;
  virtual ~IntoDatagramWithSegment() = default;
  IntoDatagramWithSegment(const IntoDatagramWithSegment& v) noexcept = default;
  IntoDatagramWithSegment& operator=(const IntoDatagramWithSegment& obj) = default;
  IntoDatagramWithSegment(IntoDatagramWithSegment&& obj) = default;
  IntoDatagramWithSegment& operator=(IntoDatagramWithSegment&& obj) = default;

  AUTD3_API [[nodiscard]] DatagramWithSegment<P> with_segment(const native_methods::Segment segment, const bool transition) & {
    return DatagramWithSegment<P>(std::make_unique<D>(*static_cast<D*>(this)), segment, transition);
  }
  AUTD3_API [[nodiscard]] DatagramWithSegment<P> with_segment(const native_methods::Segment segment, const bool transition) && {
    return DatagramWithSegment<P>(std::make_unique<D>((std::move(*static_cast<D*>(this)))), segment, transition);
  }
};

}  // namespace autd3::driver
