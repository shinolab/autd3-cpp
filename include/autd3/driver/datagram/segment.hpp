#pragma once

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/firmware/fpga/transition_mode.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct SwapSegmentGain final : Datagram, IntoDatagramTuple<SwapSegmentGain> {
  AUTD3_API explicit SwapSegmentGain(const native_methods::Segment segment) : segment(segment) {}

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override { return AUTDDatagramSwapSegmentGain(segment); }

  native_methods::Segment segment;
};

template <transition_mode_v T>
struct SwapSegmentGainSTM final : Datagram, IntoDatagramTuple<SwapSegmentGainSTM<T>> {
  AUTD3_API explicit SwapSegmentGainSTM(const native_methods::Segment segment, const T transition_mode)
      : segment(segment), transition_mode(transition_mode) {}

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
    return AUTDDatagramSwapSegmentGainSTM(segment, transition_mode.inner());
  }

  native_methods::Segment segment;
  T transition_mode;
};

template <transition_mode_v T>
struct SwapSegmentFociSTM final : Datagram, IntoDatagramTuple<SwapSegmentFociSTM<T>> {
  AUTD3_API explicit SwapSegmentFociSTM(const native_methods::Segment segment, const T transition_mode)
      : segment(segment), transition_mode(transition_mode) {}

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
    return AUTDDatagramSwapSegmentFociSTM(segment, transition_mode.inner());
  }

  native_methods::Segment segment;
  T transition_mode;
};

template <transition_mode_v T>
struct SwapSegmentModulation final : Datagram, IntoDatagramTuple<SwapSegmentModulation<T>> {
  AUTD3_API explicit SwapSegmentModulation(const native_methods::Segment segment, const T transition_mode)
      : segment(segment), transition_mode(transition_mode) {}

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
    return AUTDDatagramSwapSegmentModulation(segment, transition_mode.inner());
  }

  native_methods::Segment segment;
  T transition_mode;
};

}  // namespace autd3::driver