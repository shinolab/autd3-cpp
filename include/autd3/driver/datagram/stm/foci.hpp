#pragma once

#include "autd3/driver/datagram/stm/control_point.hpp"
#include "autd3/driver/datagram/stm/stm.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/stm_sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <uint8_t N>
class FociSTMBase : Datagram {
 public:
  FociSTMBase(const FociSTMBase& obj) = default;             // LCOV_EXCL_LINE
  FociSTMBase& operator=(const FociSTMBase& obj) = default;  // LCOV_EXCL_LINE
  FociSTMBase(FociSTMBase&& obj) = default;                  // LCOV_EXCL_LINE
  FociSTMBase& operator=(FociSTMBase&& obj) = default;       // LCOV_EXCL_LINE
  ~FociSTMBase() override = default;                         // LCOV_EXCL_LINE

  AUTD3_API explicit FociSTMBase(std::vector<ControlPoints<N>> foci) : foci(std::move(foci)) {}

  AUTD3_API [[nodiscard]] native_methods::FociSTMPtr raw_ptr() const {
    return native_methods::AUTDSTMFoci(sampling_config(), reinterpret_cast<const void*>(foci.data()), static_cast<uint16_t>(foci.size()), N);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr with_segment_transition(const geometry::Geometry&, const native_methods::Segment segment,
                                                                              const native_methods::TransitionModeWrap transition_mode) const {
    return AUTDSTMFociIntoDatagramWithSegment(raw_ptr(), N, segment, transition_mode);
  }
  AUTD3_API [[nodiscard]] native_methods::DatagramPtr with_loop_behavior(const geometry::Geometry&, const native_methods::Segment segment,
                                                                         const native_methods::TransitionModeWrap transition_mode,
                                                                         const native_methods::LoopBehavior loop_behavior) const {
    return AUTDSTMFociIntoDatagramWithLoopBehavior(raw_ptr(), N, segment, transition_mode, loop_behavior);
  }
  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override { return AUTDSTMFociIntoDatagram(raw_ptr(), N); }

  std::vector<ControlPoints<N>> foci;

  AUTD3_API [[nodiscard]] virtual SamplingConfig sampling_config() const = 0;

 protected:
  FociSTMBase() = default;
};

template <uint8_t, typename>
class FociSTM {};

template <>
class FociSTM<1, SamplingConfig> final : public FociSTMBase<1>, public IntoDatagramTuple<FociSTM<1, SamplingConfig>> {
 public:
  AUTD3_API explicit FociSTM(std::vector<ControlPoints<1>> foci_, const SamplingConfig config) : FociSTMBase(std::move(foci_)), config(config) {}
  AUTD3_API explicit FociSTM(const std::vector<ControlPoint>& foci_, const SamplingConfig config_) : FociSTMBase(), config(config_) {
    this->foci.reserve(foci_.size());
    for (const auto& f : foci_)
      this->foci.emplace_back(ControlPoints<1>{.points = std::array{f}, .intensity = std::numeric_limits<EmitIntensity>::max()});
  }
  AUTD3_API explicit FociSTM(const std::vector<Point3>& foci_, const SamplingConfig config_) : FociSTMBase(), config(config_) {
    this->foci.reserve(foci_.size());
    for (const auto& f : foci_)
      this->foci.emplace_back(ControlPoints<1>{.points = std::array{ControlPoint{.point = f, .phase_offset = Phase::zero()}},
                                               .intensity = std::numeric_limits<EmitIntensity>::max()});
  }

  SamplingConfig config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return config; }
};

template <uint8_t N>
class FociSTM<N, SamplingConfig> final : public FociSTMBase<N>, public IntoDatagramTuple<FociSTM<N, SamplingConfig>> {
 public:
  AUTD3_API explicit FociSTM(std::vector<ControlPoints<N>> foci, const SamplingConfig config) : FociSTMBase<N>(std::move(foci)), config(config) {}

  SamplingConfig config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return config; }
};

template <uint8_t N>
class FociSTM<N, NearestFreq> final : public FociSTMBase<N>, public IntoDatagramTuple<FociSTM<N, NearestFreq>> {
 public:
  friend class FociSTM<N, Freq<float>>;

  Freq<float> config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return STMSamplingConfig::from_freq_nearest(config, this->foci.size()); }

 private:
  AUTD3_API explicit FociSTM(std::vector<ControlPoints<N>> foci_, const Freq<float> config_) : FociSTMBase<N>(std::move(foci_)), config(config_) {}
};

template <>
class FociSTM<1, Freq<float>> final : public FociSTMBase<1>, public IntoDatagramTuple<FociSTM<1, Freq<float>>> {
 public:
  AUTD3_API explicit FociSTM(std::vector<ControlPoints<1>> foci_, const Freq<float> config_) : FociSTMBase<1>(std::move(foci_)), config(config_) {}
  AUTD3_API explicit FociSTM(const std::vector<ControlPoint>& foci_, const Freq<float> config_) : FociSTMBase<1>(), config(config_) {
    this->foci.reserve(foci_.size());
    for (const auto& f : foci_)
      this->foci.emplace_back(ControlPoints<1>{.points = std::array{f}, .intensity = std::numeric_limits<EmitIntensity>::max()});
  }
  AUTD3_API explicit FociSTM(const std::vector<Point3>& foci_, const Freq<float> config_) : FociSTMBase<1>(), config(config_) {
    this->foci.reserve(foci_.size());
    for (const auto& f : foci_)
      this->foci.emplace_back(ControlPoints<1>{.points = std::array{ControlPoint{.point = f, .phase_offset = Phase::zero()}},
                                               .intensity = std::numeric_limits<EmitIntensity>::max()});
  }

  AUTD3_API FociSTM<1, NearestFreq> into_nearest() && { return FociSTM<1, NearestFreq>(std::move(this->foci), config); }

  Freq<float> config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return STMSamplingConfig::from_freq(config, this->foci.size()); }
};

template <uint8_t N>
class FociSTM<N, Freq<float>> final : public FociSTMBase<N>, public IntoDatagramTuple<FociSTM<N, Freq<float>>> {
 public:
  AUTD3_API explicit FociSTM(std::vector<ControlPoints<N>> foci, const Freq<float> config) : FociSTMBase<N>(std::move(foci)), config(config) {}

  AUTD3_API FociSTM<N, NearestFreq> into_nearest() && { return FociSTM<N, NearestFreq>(std::move(this->foci), config); }

  Freq<float> config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return STMSamplingConfig::from_freq(config, this->foci.size()); }
};

template <uint8_t N>
class FociSTM<N, NearestPeriod> final : public FociSTMBase<N>, public IntoDatagramTuple<FociSTM<N, NearestPeriod>> {
 public:
  friend class FociSTM<N, std::chrono::nanoseconds>;

  std::chrono::nanoseconds config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override {
    return STMSamplingConfig::from_period_nearest(config, this->foci.size());
  }

 private:
  AUTD3_API explicit FociSTM(std::vector<ControlPoints<N>> foci, const std::chrono::nanoseconds config)
      : FociSTMBase<N>(std::move(foci)), config(config) {}
};

template <>
class FociSTM<1, std::chrono::nanoseconds> final : public FociSTMBase<1>, public IntoDatagramTuple<FociSTM<1, std::chrono::nanoseconds>> {
 public:
  template <typename Rep, typename P>
  AUTD3_API explicit FociSTM(std::vector<ControlPoints<1>> foci, const std::chrono::duration<Rep, P> config)
      : FociSTMBase<1>(std::move(foci)), config(config) {}
  template <typename Rep, typename P>
  AUTD3_API explicit FociSTM(const std::vector<ControlPoint>& foci, const std::chrono::duration<Rep, P> config) : FociSTMBase<1>(), config(config) {
    this->foci.reserve(foci.size());
    for (const auto& f : foci)
      this->foci.emplace_back(ControlPoints<1>{.points = std::array{f}, .intensity = std::numeric_limits<EmitIntensity>::max()});
  }
  template <typename Rep, typename P>
  AUTD3_API explicit FociSTM(const std::vector<Point3>& foci, const std::chrono::duration<Rep, P> config) : FociSTMBase<1>(), config(config) {
    this->foci.reserve(foci.size());
    for (const auto& f : foci)
      this->foci.emplace_back(ControlPoints<1>{.points = std::array{ControlPoint{.point = f, .phase_offset = Phase::zero()}},
                                               .intensity = std::numeric_limits<EmitIntensity>::max()});
  }

  AUTD3_API FociSTM<1, NearestPeriod> into_nearest() && { return FociSTM<1, NearestPeriod>(std::move(this->foci), config); }

  std::chrono::nanoseconds config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return STMSamplingConfig::from_period(config, this->foci.size()); }
};

template <uint8_t N>
class FociSTM<N, std::chrono::nanoseconds> final : public FociSTMBase<N>, public IntoDatagramTuple<FociSTM<N, std::chrono::nanoseconds>> {
 public:
  template <typename Rep, typename P>
  AUTD3_API explicit FociSTM(std::vector<ControlPoints<N>> foci, const std::chrono::duration<Rep, P> config)
      : FociSTMBase<N>(std::move(foci)), config(config) {}

  AUTD3_API FociSTM<N, NearestPeriod> into_nearest() && { return FociSTM<N, NearestPeriod>(std::move(this->foci), config); }

  std::chrono::nanoseconds config;

  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const override { return STMSamplingConfig::from_period(config, this->foci.size()); }
};

FociSTM(std::vector<ControlPoints<1>> foci, SamplingConfig config) -> FociSTM<1, SamplingConfig>;
FociSTM(std::vector<ControlPoint> foci, SamplingConfig config) -> FociSTM<1, SamplingConfig>;
FociSTM(std::vector<Point3> foci, SamplingConfig config) -> FociSTM<1, SamplingConfig>;
FociSTM(std::vector<ControlPoints<1>> foci, Freq<float> config) -> FociSTM<1, Freq<float>>;
FociSTM(std::vector<ControlPoint> foci, Freq<float> config) -> FociSTM<1, Freq<float>>;
FociSTM(std::vector<Point3> foci, Freq<float> config) -> FociSTM<1, Freq<float>>;
template <typename Rep, typename P>
FociSTM(std::vector<ControlPoints<1>> foci, std::chrono::duration<Rep, P> config) -> FociSTM<1, std::chrono::nanoseconds>;
template <typename Rep, typename P>
FociSTM(std::vector<ControlPoint> foci, std::chrono::duration<Rep, P> config) -> FociSTM<1, std::chrono::nanoseconds>;
template <typename Rep, typename P>
FociSTM(std::vector<Point3> foci, std::chrono::duration<Rep, P> config) -> FociSTM<1, std::chrono::nanoseconds>;
template <uint8_t N>
FociSTM(std::vector<ControlPoints<N>> foci, SamplingConfig config) -> FociSTM<N, SamplingConfig>;
template <uint8_t N>
FociSTM(std::vector<ControlPoints<N>> foci, Freq<float> config) -> FociSTM<N, Freq<float>>;
template <typename Rep, typename P, uint8_t N>
FociSTM(std::vector<ControlPoints<N>> foci, std::chrono::duration<Rep, P> config) -> FociSTM<N, std::chrono::nanoseconds>;

}  // namespace autd3::driver
