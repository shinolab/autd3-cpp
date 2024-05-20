#pragma once

#include <memory>
#include <ranges>
#include <variant>

#include "autd3/driver/datagram/gain/base.hpp"
#include "autd3/driver/datagram/stm/stm.hpp"
#include "autd3/driver/datagram/with_segment_transition.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class R>
concept gain_range =
    std::ranges::viewable_range<R> && gain<std::ranges::range_value_t<R>>;

class GainSTM final : public DatagramST<native_methods::GainSTMPtr> {
 public:
  GainSTM() = delete;
  GainSTM(const GainSTM& obj) = default;             // LCOV_EXCL_LINE
  GainSTM& operator=(const GainSTM& obj) = default;  // LCOV_EXCL_LINE
  GainSTM(GainSTM&& obj) = default;                  // LCOV_EXCL_LINE
  GainSTM& operator=(GainSTM&& obj) = default;       // LCOV_EXCL_LINE
  ~GainSTM() override = default;                     // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] static GainSTM from_freq(const Freq<double> freq) {
    return GainSTM(STMSamplingModeFreq{freq});
  }
  AUTD3_API [[nodiscard]] static GainSTM from_freq_nearest(
      const Freq<double> freq) {
    return GainSTM(STMSamplingModeFreqNearest{freq});
  }
  AUTD3_API [[nodiscard]] static GainSTM from_sampling_config(
      const native_methods::SamplingConfigWrap config) {
    return GainSTM(STMSamplingModeSamplingConfig{config});
  }

  AUTD3_API [[nodiscard]] native_methods::GainSTMPtr raw_ptr(
      const geometry::Geometry& geometry) const override {
    native_methods::GainSTMPtr ptr =
        std::visit([](const auto s) { return s.gain_ptr(); }, _sampling);

    ptr = AUTDSTMGainWithMode(ptr, _mode);
    ptr = AUTDSTMGainWithLoopBehavior(ptr, _loop_behavior);

    std::vector<native_methods::GainPtr> gains;
    gains.reserve(_gains.size());
    std::ranges::transform(
        _gains, std::back_inserter(gains),
        [&](const auto& gain) { return gain->gain_ptr(geometry); });
    return AUTDSTMGainAddGains(ptr, gains.data(),
                               static_cast<uint32_t>(gains.size()));
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment(
      const native_methods::GainSTMPtr p,
      const native_methods::Segment segment) const override {
    return AUTDSTMGainIntoDatagramWithSegment(p, segment);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment_transition(
      const native_methods::GainSTMPtr p, const native_methods::Segment segment,
      const native_methods::TransitionModeWrap transition_mode) const override {
    return AUTDSTMGainIntoDatagramWithSegmentTransition(p, segment,
                                                        transition_mode);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(
      const geometry::Geometry& geometry) const {
    return AUTDSTMGainIntoDatagram(raw_ptr(geometry));
  }

  AUTD3_API
      [[nodiscard]] DatagramWithSegmentTransition<native_methods::GainSTMPtr>
      with_segment(
          const native_methods::Segment segment,
          std::optional<native_methods::TransitionModeWrap> transition_mode) {
    return DatagramWithSegmentTransition<native_methods::GainSTMPtr>(
        std::make_unique<GainSTM>(std::move(*this)), segment,
        std::move(transition_mode));
  }

  template <gain G>
  AUTD3_API void add_gain(G&& gain) & {
    _gains.emplace_back(
        std::make_shared<std::remove_reference_t<G>>(std::forward<G>(gain)));
  }

  template <gain G>
  AUTD3_API [[nodiscard]] GainSTM&& add_gain(G&& gain) && {
    _gains.emplace_back(
        std::make_shared<std::remove_reference_t<G>>(std::forward<G>(gain)));
    return std::move(*this);
  }

  template <gain_range R>
  AUTD3_API void add_gains_from_iter(R&& iter) & {
    for (auto e : iter)
      _gains.emplace_back(
          std::make_shared<
              std::remove_reference_t<std::ranges::range_value_t<R>>>(
              std::forward<std::ranges::range_value_t<R>>(e)));
  }

  template <gain_range R>
  AUTD3_API [[nodiscard]] GainSTM add_gains_from_iter(R&& iter) && {
    for (auto e : iter)
      _gains.emplace_back(
          std::make_shared<
              std::remove_reference_t<std::ranges::range_value_t<R>>>(
              std::forward<std::ranges::range_value_t<R>>(e)));
    return std::move(*this);
  }

  AUTD3_DEF_PARAM(GainSTM, native_methods::LoopBehavior, loop_behavior)
  AUTD3_DEF_PARAM(GainSTM, native_methods::GainSTMMode, mode)

 private:
  AUTD3_API explicit GainSTM(
      std::variant<STMSamplingModeFreq, STMSamplingModeFreqNearest,
                   STMSamplingModeSamplingConfig>
          sampling)
      : _loop_behavior(LoopBehavior::Infinite),
        _mode(native_methods::GainSTMMode::PhaseIntensityFull),
        _sampling(std::move(sampling)) {}

  std::vector<std::shared_ptr<GainBase>> _gains;
  std::variant<STMSamplingModeFreq, STMSamplingModeFreqNearest,
               STMSamplingModeSamplingConfig>
      _sampling;
};

}  // namespace autd3::driver
