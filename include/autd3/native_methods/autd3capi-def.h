#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

constexpr const uint32_t NUM_TRANS_IN_UNIT = 249;

constexpr const uint32_t NUM_TRANS_IN_X = 18;

constexpr const uint32_t NUM_TRANS_IN_Y = 14;

constexpr const uint32_t FPGA_CLK_FREQ = 20480000;

constexpr const int32_t AUTD3_ERR = -1;

constexpr const int32_t AUTD3_TRUE = 1;

constexpr const int32_t AUTD3_FALSE = 0;

enum class GainSTMMode : uint8_t {
  PhaseIntensityFull = 0,
  PhaseFull = 1,
  PhaseHalf = 2,
};

enum class Segment : uint8_t {
  S0 = 0,
  S1 = 1,
};

enum class TimerStrategy : uint8_t {
  Sleep = 0,
  BusyWait = 1,
  NativeTimer = 2,
};

struct LoopBehavior {
  uint32_t rep;
};

struct SamplingConfiguration {
  uint32_t div;
};

struct ResultSamplingConfig {
  SamplingConfiguration result;
  uint32_t err_len;
  void* err;
};

struct ControllerPtr {
  void* _0;
};

struct DevicePtr {
  void* _0;
};

struct TransducerPtr {
  void* _0;
};

struct GeometryPtr {
  void* _0;
};

struct ModulationPtr {
  void* _0;
};

struct GainPtr {
  void* _0;
};

struct LinkPtr {
  void* _0;
};

struct DatagramPtr {
  void* _0;
};

struct LinkBuilderPtr {
  void* _0;
};

struct ResultI32 {
  int32_t result;
  uint32_t err_len;
  void* err;
};

struct ResultModulation {
  ModulationPtr result;
  uint32_t err_len;
  void* err;
};

struct ResultController {
  ControllerPtr result;
  uint32_t err_len;
  void* err;
};

struct ResultDatagram {
  DatagramPtr result;
  uint32_t err_len;
  void* err;
};

struct Drive {
  uint8_t phase;
  uint8_t intensity;
};

struct GainSTMPtr {
  void* _0;
};

struct FocusSTMPtr {
  void* _0;
};

struct ResultGainSTM {
  GainSTMPtr result;
  uint32_t err_len;
  void* err;
};

struct ResultFocusSTM {
  FocusSTMPtr result;
  uint32_t err_len;
  void* err;
};

constexpr const double TRANS_SPACING_MM = 10.16;

constexpr const double DEVICE_HEIGHT_MM = 151.4;

constexpr const double DEVICE_WIDTH_MM = 192.0;

constexpr const double ULTRASOUND_FREQUENCY = 40000.0;

constexpr const double DEFAULT_CORRECTED_ALPHA = 0.803;

extern "C" {

[[nodiscard]] uint8_t AUTDEmitIntensityWithCorrectionAlpha(uint8_t value, double alpha);

[[nodiscard]] uint8_t AUTDPhaseFromRad(double value);

[[nodiscard]] double AUTDPhaseToRad(uint8_t value);

[[nodiscard]] LoopBehavior AUTDLoopBehaviorInfinite();

[[nodiscard]] LoopBehavior AUTDLoopBehaviorFinite(uint32_t v);

[[nodiscard]] LoopBehavior AUTDLoopBehaviorOnce();

void AUTDGetErr(void* src, char *dst);

[[nodiscard]] ResultSamplingConfig AUTDSamplingConfigFromFrequencyDivision(uint32_t div);

[[nodiscard]] ResultSamplingConfig AUTDSamplingConfigFromFrequency(double f);

[[nodiscard]] ResultSamplingConfig AUTDSamplingConfigFromPeriod(uint64_t p);

[[nodiscard]] uint32_t AUTDSamplingConfigFrequencyDivision(SamplingConfiguration config);

[[nodiscard]] double AUTDSamplingConfigFrequency(SamplingConfiguration config);

[[nodiscard]] uint64_t AUTDSamplingConfigPeriod(SamplingConfiguration config);

} // extern "C"

} // namespace autd3::native_methods
