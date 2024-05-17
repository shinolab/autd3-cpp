#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct SwapSegment {
  class Gain final {
   public:
    AUTD3_API explicit Gain(const native_methods::Segment segment) : _segment(segment) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const { return AUTDDatagramSwapSegmentGain(_segment); }

   private:
    native_methods::Segment _segment;
  };

  class GainSTM final {
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

  class FocusSTM final {
   public:
    AUTD3_API explicit FocusSTM(const native_methods::Segment segment, const native_methods::TransitionModeWrap transition_mode)
        : _segment(segment), _transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return AUTDDatagramSwapSegmentFocusSTM(_segment, _transition_mode);
    }

   private:
    native_methods::Segment _segment;
    native_methods::TransitionModeWrap _transition_mode;
  };

  class Modulation final {
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

  AUTD3_API [[nodiscard]] static inline Gain gain(const native_methods::Segment segment) { return Gain(segment); }

  AUTD3_API [[nodiscard]] static inline GainSTM gain_stm(const native_methods::Segment segment,
                                                         const native_methods::TransitionModeWrap transition_mode) {
    return GainSTM(segment, transition_mode);
  }

  AUTD3_API [[nodiscard]] static inline FocusSTM focus_stm(const native_methods::Segment segment,
                                                           const native_methods::TransitionModeWrap transition_mode) {
    return FocusSTM(segment, transition_mode);
  }

  AUTD3_API [[nodiscard]] static inline Modulation modulation(const native_methods::Segment segment,
                                                              const native_methods::TransitionModeWrap transition_mode) {
    return Modulation(segment, transition_mode);
  }
};

}  // namespace autd3::driver