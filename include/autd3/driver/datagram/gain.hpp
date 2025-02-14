#pragma once

#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class Gain : public Datagram {
 public:
  Gain() = default;                            // LCOV_EXCL_LINE
  Gain(const Gain& obj) = default;             // LCOV_EXCL_LINE
  Gain& operator=(const Gain& obj) = default;  // LCOV_EXCL_LINE
  Gain(Gain&& obj) = default;                  // LCOV_EXCL_LINE
  Gain& operator=(Gain&& obj) = default;       // LCOV_EXCL_LINE
  ~Gain() override = default;                  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    return AUTDGainIntoDatagram(gain_ptr(geometry));
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr with_segment_transition(const geometry::Geometry& geometry,
                                                                              const native_methods::Segment segment,
                                                                              const native_methods::TransitionModeWrap transition) const {
    return AUTDGainIntoDatagramWithSegment(gain_ptr(geometry), segment, transition);
  }

  AUTD3_API [[nodiscard]] virtual native_methods::GainPtr gain_ptr(const geometry::Geometry&) const = 0;
};

template <class G>
concept gain = std::derived_from<std::remove_reference_t<G>, Gain>;

}  // namespace autd3::driver
