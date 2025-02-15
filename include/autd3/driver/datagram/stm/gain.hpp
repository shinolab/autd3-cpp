#pragma once

#include <memory>

#include "autd3/driver/datagram/gain.hpp"
#include "autd3/driver/datagram/stm/stm.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/driver/firmware/fpga/stm_sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct GainSTMOption {
  native_methods::GainSTMMode mode = native_methods::GainSTMMode::PhaseIntensityFull;

  operator native_methods::GainSTMOption() const { return native_methods::GainSTMOption{.mode = mode}; }
};

class GainSTMBase : public Datagram {
 public:
  GainSTMBase() = delete;
  GainSTMBase(const GainSTMBase& obj) = default;             // LCOV_EXCL_LINE
  GainSTMBase& operator=(const GainSTMBase& obj) = default;  // LCOV_EXCL_LINE
  GainSTMBase(GainSTMBase&& obj) = default;                  // LCOV_EXCL_LINE
  GainSTMBase& operator=(GainSTMBase&& obj) = default;       // LCOV_EXCL_LINE
  ~GainSTMBase() override = default;                         // LCOV_EXCL_LINE

  template <gain G>
  AUTD3_API explicit GainSTMBase(std::vector<G> gains, const GainSTMOption option) : option(option) {
    for (auto& g : gains) this->gains.emplace_back(std::make_shared<std::remove_reference_t<G>>(std::forward<std::remove_reference_t<G>>(g)));
  }

  AUTD3_API explicit GainSTMBase(std::vector<std::shared_ptr<Gain>> gains, const GainSTMOption option) : gains(std::move(gains)), option(option) {}

  AUTD3_API [[nodiscard]] native_methods::GainSTMPtr raw_ptr(const geometry::Geometry& geometry) const {
    std::vector<native_methods::GainPtr> gains_;
    gains_.reserve(gains.size());
    std::ranges::transform(gains, std::back_inserter(gains_), [&](const auto& gain) { return gain->gain_ptr(geometry); });
    return AUTDSTMGain(sampling_config(), gains_.data(), static_cast<uint16_t>(gains.size()), option);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr with_segment_transition(const geometry::Geometry& geometry,
                                                                              const native_methods::Segment segment,
                                                                              const native_methods::TransitionModeWrap transition_mode) const {
    return AUTDSTMGainIntoDatagramWithSegment(raw_ptr(geometry), segment, transition_mode);
  }
  AUTD3_API [[nodiscard]] native_methods::DatagramPtr with_loop_behavior(const geometry::Geometry& geometry, const native_methods::Segment segment,
                                                                         const native_methods::TransitionModeWrap transition_mode,
                                                                         const native_methods::LoopBehavior loop_behavior) const {
    return AUTDSTMGainIntoDatagramWithLoopBehavior(raw_ptr(geometry), segment, transition_mode, loop_behavior);
  }
  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    return AUTDSTMGainIntoDatagram(raw_ptr(geometry));
  }

  std::vector<std::shared_ptr<Gain>> gains;
  GainSTMOption option;

  AUTD3_API [[nodiscard]] virtual SamplingConfig sampling_config() const = 0;
};

template <typename>
class GainSTM {};

template <>
class GainSTM<SamplingConfig> final : public GainSTMBase, public IntoDatagramTuple<GainSTM<SamplingConfig>> {
 public:
  template <gain G>
  AUTD3_API explicit GainSTM(std::vector<G> gains, const SamplingConfig config, const GainSTMOption option)
      : GainSTMBase(std::move(gains), option), config(config) {}
  AUTD3_API explicit GainSTM(std::vector<std::shared_ptr<Gain>> gains_, const SamplingConfig config, const GainSTMOption option_)
      : GainSTMBase(std::move(gains_), option_), config(config) {}

  SamplingConfig config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return config; }
};

template <>
class GainSTM<NearestFreq> final : public GainSTMBase, public IntoDatagramTuple<GainSTM<NearestFreq>> {
 public:
  friend class GainSTM<Freq<float>>;

  Freq<float> config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return STMSamplingConfig::from_freq_nearest(config, gains.size()); }

 private:
  AUTD3_API explicit GainSTM(std::vector<std::shared_ptr<Gain>> gains_, const Freq<float> config, const GainSTMOption option_)
      : GainSTMBase(std::move(gains_), option_), config(config) {}
};

template <>
class GainSTM<Freq<float>> final : public GainSTMBase, public IntoDatagramTuple<GainSTM<Freq<float>>> {
 public:
  template <gain G>
  AUTD3_API explicit GainSTM(std::vector<G> gains, const Freq<float> config, const GainSTMOption option)
      : GainSTMBase(std::move(gains), option), config(config) {}
  AUTD3_API explicit GainSTM(std::vector<std::shared_ptr<Gain>> gains_, const Freq<float> config, const GainSTMOption option_)
      : GainSTMBase(std::move(gains_), option_), config(config) {}

  AUTD3_API GainSTM<NearestFreq> into_nearest() && { return GainSTM<NearestFreq>(std::move(gains), config, option); }

  Freq<float> config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return STMSamplingConfig::from_freq(config, gains.size()); }
};

template <>
class GainSTM<NearestPeriod> final : public GainSTMBase, public IntoDatagramTuple<GainSTM<NearestPeriod>> {
 public:
  friend class GainSTM<std::chrono::nanoseconds>;

  std::chrono::nanoseconds config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return STMSamplingConfig::from_period_nearest(config, gains.size()); }

 private:
  AUTD3_API explicit GainSTM(std::vector<std::shared_ptr<Gain>> gains, const std::chrono::nanoseconds config, const GainSTMOption option)
      : GainSTMBase(std::move(gains), option), config(config) {}
};

template <>
class GainSTM<std::chrono::nanoseconds> final : public GainSTMBase, public IntoDatagramTuple<GainSTM<std::chrono::nanoseconds>> {
 public:
  template <typename Rep, typename P, gain G>
  AUTD3_API explicit GainSTM(std::vector<G> gains, const std::chrono::duration<Rep, P> config, const GainSTMOption option)
      : GainSTMBase(std::move(gains), option), config(config) {}

  template <typename Rep, typename P>
  AUTD3_API explicit GainSTM(std::vector<std::shared_ptr<Gain>> gains, const std::chrono::duration<Rep, P> config, const GainSTMOption option)
      : GainSTMBase(std::move(gains), option), config(config) {}

  AUTD3_API GainSTM<NearestPeriod> into_nearest() && { return GainSTM<NearestPeriod>(std::move(gains), config, option); }

  std::chrono::nanoseconds config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return STMSamplingConfig::from_period(config, gains.size()); }
};

template <gain G>
GainSTM(std::vector<G> gains, SamplingConfig config, GainSTMOption option) -> GainSTM<SamplingConfig>;
GainSTM(std::vector<std::shared_ptr<Gain>> gains, SamplingConfig config, GainSTMOption option) -> GainSTM<SamplingConfig>;
template <gain G>
GainSTM(std::vector<G> gains, Freq<float> config, GainSTMOption option) -> GainSTM<Freq<float>>;
GainSTM(std::vector<std::shared_ptr<Gain>> gains, Freq<float> config, GainSTMOption option) -> GainSTM<Freq<float>>;
template <typename Rep, typename P, gain G>
GainSTM(std::vector<G> gains, std::chrono::duration<Rep, P> config, GainSTMOption option) -> GainSTM<std::chrono::nanoseconds>;
template <typename Rep, typename P>
GainSTM(std::vector<std::shared_ptr<Gain>> gains, std::chrono::duration<Rep, P> config, GainSTMOption option) -> GainSTM<std::chrono::nanoseconds>;
}  // namespace autd3::driver
