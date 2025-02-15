#pragma once

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/firmware/fpga/transition_mode.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct SwapSegment {
  struct Gain final : Datagram, IntoDatagramTuple<Gain> {
    AUTD3_API explicit Gain(const native_methods::Segment segment, const TransitionMode transition_mode)
        : segment(segment), transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
      return AUTDDatagramSwapSegmentGain(segment, transition_mode);
    }

    native_methods::Segment segment;
    TransitionMode transition_mode;
  };

  struct GainSTM final : Datagram, IntoDatagramTuple<GainSTM> {
    AUTD3_API explicit GainSTM(const native_methods::Segment segment, const TransitionMode transition_mode)
        : segment(segment), transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
      return AUTDDatagramSwapSegmentGainSTM(segment, transition_mode);
    }

    native_methods::Segment segment;
    TransitionMode transition_mode;
  };

  struct FociSTM final : Datagram, IntoDatagramTuple<FociSTM> {
    AUTD3_API explicit FociSTM(const native_methods::Segment segment, const TransitionMode transition_mode)
        : segment(segment), transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
      return AUTDDatagramSwapSegmentFociSTM(segment, transition_mode);
    }

    native_methods::Segment segment;
    TransitionMode transition_mode;
  };

  struct Modulation final : Datagram, IntoDatagramTuple<Modulation> {
    AUTD3_API explicit Modulation(const native_methods::Segment segment, const TransitionMode transition_mode)
        : segment(segment), transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override {
      return AUTDDatagramSwapSegmentModulation(segment, transition_mode);
    }

    native_methods::Segment segment;
    TransitionMode transition_mode;
  };
};

}  // namespace autd3::driver