#pragma once

#include <type_traits>

#include "autd3/driver/datagram/with_segment.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class GainBase : public DatagramS<native_methods::GainPtr> {
 public:
  GainBase() = default;
  GainBase(const GainBase& obj) = default;
  GainBase& operator=(const GainBase& obj) = default;
  GainBase(GainBase&& obj) = default;
  GainBase& operator=(GainBase&& obj) = default;
  virtual ~GainBase() = default;

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const { return AUTDGainIntoDatagram(gain_ptr(geometry)); }

  [[nodiscard]] native_methods::GainPtr raw_ptr(const geometry::Geometry& geometry) const override { return gain_ptr(geometry); }

  [[nodiscard]] native_methods::DatagramPtr into_segment(const native_methods::GainPtr p, const native_methods::Segment segment,
                                                         const bool transition) const override {
    return AUTDGainIntoDatagramWithSegment(p, segment, transition);
  }

  [[nodiscard]] virtual native_methods::GainPtr gain_ptr(const geometry::Geometry&) const = 0;
};

template <class G>
concept gain = std::derived_from<std::remove_reference_t<G>, GainBase>;

}  // namespace autd3::driver
