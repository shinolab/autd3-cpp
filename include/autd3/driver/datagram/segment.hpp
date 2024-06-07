#pragma once

#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct SwapSegment {
  class Gain final : public IntoDatagramWithTimeout<Gain>, public IntoDatagramWithParallelThreshold<Gain> {
   public:
    AUTD3_API explicit Gain(const native_methods::Segment segment) : _segment(segment) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const { return AUTDDatagramSwapSegmentGain(_segment); }

   private:
    native_methods::Segment _segment;
  };

  class GainSTM final : public IntoDatagramWithTimeout<GainSTM>, public IntoDatagramWithParallelThreshold<GainSTM> {
   public:
    AUTD3_API explicit GainSTM(const native_methods::Segment segment, const native_methods::TransitionModeWrap transition_mode)
        : _segment(segment), _transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return AUTDDatagramSwapSegmentGainSTM(_segment, _transition_mode);
    }

   private:
    native_methods::Segment _segment;
    native_methods::TransitionModeWrap _transition_mode;
  };

  class FociSTM final : public IntoDatagramWithTimeout<FociSTM>, public IntoDatagramWithParallelThreshold<FociSTM> {
   public:
    AUTD3_API explicit FociSTM(const native_methods::Segment segment, const native_methods::TransitionModeWrap transition_mode)
        : _segment(segment), _transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return AUTDDatagramSwapSegmentFociSTM(_segment, _transition_mode);
    }

   private:
    native_methods::Segment _segment;
    native_methods::TransitionModeWrap _transition_mode;
  };

  class Modulation final : public IntoDatagramWithTimeout<Modulation>, public IntoDatagramWithParallelThreshold<Modulation> {
   public:
    AUTD3_API explicit Modulation(const native_methods::Segment segment, const native_methods::TransitionModeWrap transition_mode)
        : _segment(segment), _transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return AUTDDatagramSwapSegmentModulation(_segment, _transition_mode);
    }

   private:
    native_methods::Segment _segment;
    native_methods::TransitionModeWrap _transition_mode;
  };
};

}  // namespace autd3::driver