#pragma once

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/firmware/fpga/transition_mode.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct SwapSegment {
  struct Gain final : Datagram, IntoDatagramTuple<Gain> {
    AUTD3_API explicit Gain(const native_methods::Segment segment) : segment(segment) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override { return AUTDDatagramSwapSegmentGain(segment); }

    native_methods::Segment segment;
  };

  template <transition_mode_v T>
  struct GainSTM final : Datagram, IntoDatagramTuple<GainSTM<T>> {
    AUTD3_API explicit GainSTM(const native_methods::Segment segment, const T transition_mode) : segment(segment), transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
      return AUTDDatagramSwapSegmentGainSTM(segment, transition_mode.inner());
    }

    native_methods::Segment segment;
    T transition_mode;
  };

  template <transition_mode_v T>
  struct FociSTM final : Datagram, IntoDatagramTuple<FociSTM<T>> {
    AUTD3_API explicit FociSTM(const native_methods::Segment segment, const T transition_mode) : segment(segment), transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
      return AUTDDatagramSwapSegmentFociSTM(segment, transition_mode.inner());
    }

    native_methods::Segment segment;
    T transition_mode;
  };

  template <transition_mode_v T>
  struct Modulation final : Datagram, IntoDatagramTuple<Modulation<T>> {
    AUTD3_API explicit Modulation(const native_methods::Segment segment, const T transition_mode)
        : segment(segment), transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
      return AUTDDatagramSwapSegmentModulation(segment, transition_mode.inner());
    }

    native_methods::Segment segment;
    T transition_mode;
  };
};

}  // namespace autd3::driver