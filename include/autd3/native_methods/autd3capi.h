#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

struct ResultController {
  ControllerPtr result;
  uint32_t err_len;
  const void* err;
};

struct FPGAStateListPtr {
  const void *_0;
};

struct ResultFPGAStateList {
  FPGAStateListPtr result;
  uint32_t err_len;
  const void* err;
};

struct FirmwareVersionListPtr {
  const void *_0;
};

struct ResultFirmwareVersionList {
  FirmwareVersionListPtr result;
  uint32_t err_len;
  const void* err;
};

struct GainCachePtr {
  const void *_0;
};

struct GroupGainMapPtr {
  const void *_0;
};

struct ModulationCachePtr {
  const void *_0;
};

extern "C" {

[[nodiscard]] RuntimePtr AUTDCreateRuntime();

[[nodiscard]] HandlePtr AUTDGetRuntimeHandle(RuntimePtr runtime);

void AUTDDeleteRuntime(RuntimePtr runtime);

[[nodiscard]] ResultStatus AUTDWaitResultStatus(HandlePtr handle, FfiFutureResultStatus future);

[[nodiscard]]
ResultStatus AUTDWaitLocalResultStatus(HandlePtr handle,
                                       LocalFfiFutureResultStatus future);

[[nodiscard]]
ResultController AUTDWaitResultController(HandlePtr handle,
                                          FfiFutureResultController future);

[[nodiscard]]
ResultFPGAStateList AUTDWaitResultFPGAStateList(HandlePtr handle,
                                                FfiFutureResultFPGAStateList future);

[[nodiscard]]
ResultFirmwareVersionList AUTDWaitResultFirmwareVersionList(HandlePtr handle,
                                                            FfiFutureResultFirmwareVersionList future);

void AUTDTracingInit();

ResultStatus AUTDTracingInitWithFile(const char *path);

[[nodiscard]] FfiFutureResultStatus AUTDControllerClose(ControllerPtr cnt);

[[nodiscard]] FfiFutureResultFPGAStateList AUTDControllerFPGAState(ControllerPtr cnt);

int16_t AUTDControllerFPGAStateGet(FPGAStateListPtr p, uint32_t idx);

void AUTDControllerFPGAStateDelete(FPGAStateListPtr p);

[[nodiscard]]
FfiFutureResultFirmwareVersionList AUTDControllerFirmwareVersionListPointer(ControllerPtr cnt);

void AUTDControllerFirmwareVersionGet(FirmwareVersionListPtr p_info_list, uint32_t idx, char *info);

void AUTDControllerFirmwareVersionListPointerDelete(FirmwareVersionListPtr p_info_list);

void AUTDFirmwareLatest(char *latest);

[[nodiscard]] FfiFutureResultStatus AUTDControllerSend(ControllerPtr cnt, DatagramPtr d);

[[nodiscard]]
ControllerBuilderPtr AUTDControllerBuilder(const Vector3 *pos,
                                           const Quaternion *rot,
                                           uint16_t len,
                                           uint16_t fallback_parallel_threshold,
                                           Duration fallback_timeout,
                                           Duration send_interval,
                                           Duration receive_interval,
                                           TimerStrategyWrap timer_strategy);

[[nodiscard]]
bool AUTDControllerBuilderIsDefault(uint16_t fallback_parallel_threshold,
                                    Duration fallback_timeout,
                                    Duration send_interval_ns,
                                    Duration receive_interval,
                                    TimerStrategyWrap timer_strategy);

[[nodiscard]]
FfiFutureResultController AUTDControllerOpen(ControllerBuilderPtr builder,
                                               LinkBuilderPtr link_builder,
                                               OptionDuration timeout);

[[nodiscard]]
LocalFfiFutureResultStatus AUTDControllerGroup(ControllerPtr cnt,
                                                 const void* f,
                                                 const void* context,
                                                 GeometryPtr geometry,
                                                 const int32_t *keys,
                                                 const DatagramPtr *d,
                                                 uint16_t n);

[[nodiscard]] TimerStrategyWrap AUTDTimerStrategyStd(uint32_t timer_resolution);

[[nodiscard]] uint32_t AUTDTimerStrategySpinDefaultAccuracy();

[[nodiscard]]
TimerStrategyWrap AUTDTimerStrategySpin(uint32_t native_accuracy_ns,
                                        SpinStrategyTag spin_strategy);

[[nodiscard]] TimerStrategyWrap AUTDTimerStrategyAsync(uint32_t timer_resolution);

[[nodiscard]] TimerStrategyWrap AUTDTimerStrategyWaitable();

[[nodiscard]] DatagramPtr AUTDDatagramTuple(DatagramPtr d1, DatagramPtr d2);

[[nodiscard]] DatagramPtr AUTDDatagramClear();

[[nodiscard]]
DatagramPtr AUTDDatagramDebugSettings(const void* f,
                                      const void* context,
                                      GeometryPtr geometry);

[[nodiscard]]
DatagramPtr AUTDDatagramForceFan(const void* f,
                                 const void* context,
                                 GeometryPtr geometry);

[[nodiscard]]
DatagramPtr AUTDDatagramPhaseCorr(const void* f,
                                  const void* context,
                                  GeometryPtr geometry);

[[nodiscard]]
DatagramPtr AUTDDatagramPulseWidthEncoder(const void* f,
                                          const void* context,
                                          GeometryPtr geometry);

[[nodiscard]] DatagramPtr AUTDDatagramPulseWidthEncoderDefault();

[[nodiscard]]
DatagramPtr AUTDDatagramReadsFPGAState(const void* f,
                                       const void* context,
                                       GeometryPtr geometry);

[[nodiscard]]
DatagramPtr AUTDDatagramSwapSegmentModulation(Segment segment,
                                              TransitionModeWrap transition_mode);

[[nodiscard]]
DatagramPtr AUTDDatagramSwapSegmentFociSTM(Segment segment,
                                           TransitionModeWrap transition_mode);

[[nodiscard]]
DatagramPtr AUTDDatagramSwapSegmentGainSTM(Segment segment,
                                           TransitionModeWrap transition_mode);

[[nodiscard]]
DatagramPtr AUTDDatagramSwapSegmentGain(Segment segment,
                                        TransitionModeWrap transition_mode);

[[nodiscard]]
DatagramPtr AUTDDatagramSilencerFromUpdateRate(uint16_t intensity,
                                               uint16_t phase,
                                               SilencerTarget target);

[[nodiscard]]
bool AUTDDatagramSilencerFixedUpdateRateIsValid(uint16_t intensity,
                                                uint16_t phase,
                                                SamplingConfig config_intensity,
                                                SamplingConfig config_phase);

[[nodiscard]]
DatagramPtr AUTDDatagramSilencerFromCompletionTime(Duration intensity,
                                                   Duration phase,
                                                   bool strict_mode,
                                                   SilencerTarget target);

[[nodiscard]]
bool AUTDDatagramSilencerFixedCompletionTimeIsValid(Duration intensity,
                                                    Duration phase,
                                                    bool strict_mode,
                                                    SamplingConfig config_intensity,
                                                    SamplingConfig config_phase);

[[nodiscard]]
bool AUTDDatagramSilencerFixedCompletionTimeIsDefault(Duration intensity,
                                                      Duration phase,
                                                      bool strict_mode,
                                                      SilencerTarget target);

[[nodiscard]] ResultSamplingConfig AUTDSTMConfigFromFreq(float f, uint16_t n);

[[nodiscard]] ResultSamplingConfig AUTDSTMConfigFromPeriod(Duration p, uint16_t n);

[[nodiscard]] ResultSamplingConfig AUTDSTMConfigFromFreqNearest(float f, uint16_t n);

[[nodiscard]] ResultSamplingConfig AUTDSTMConfigFromPeriodNearest(Duration p, uint16_t n);

[[nodiscard]] float AUTDSTMFreq(SamplingConfig c, uint16_t n);

[[nodiscard]] Duration AUTDSTMPeriod(SamplingConfig c, uint16_t n);

[[nodiscard]]
ResultFociSTM AUTDSTMFoci(SamplingConfig config,
                          const void* points,
                          uint16_t size,
                          uint8_t n,
                          LoopBehavior loop_behavior);

[[nodiscard]]
DatagramPtr AUTDSTMFociIntoDatagramWithSegment(FociSTMPtr stm,
                                               uint8_t n,
                                               Segment segment,
                                               TransitionModeWrap transition_mode);

[[nodiscard]] DatagramPtr AUTDSTMFociIntoDatagram(FociSTMPtr stm, uint8_t n);

[[nodiscard]]
ResultGainSTM AUTDSTMGain(SamplingConfig config,
                          const GainPtr *gains,
                          uint16_t size,
                          GainSTMMode mode,
                          LoopBehavior loop_behavior);

[[nodiscard]]
DatagramPtr AUTDSTMGainIntoDatagramWithSegment(GainSTMPtr stm,
                                               Segment segment,
                                               TransitionModeWrap transition_mode);

[[nodiscard]] DatagramPtr AUTDSTMGainIntoDatagram(GainSTMPtr stm);

[[nodiscard]] DatagramPtr AUTDDatagramSynchronize();

[[nodiscard]] DatagramPtr AUTDDatagramWithParallelThreshold(DatagramPtr d, int32_t threshold);

[[nodiscard]] DatagramPtr AUTDDatagramWithTimeout(DatagramPtr d, OptionDuration timeout);

[[nodiscard]] DcSysTime AUTDDcSysTimeNow();

[[nodiscard]] DebugTypeWrap AUTDDebugTypeNone();

[[nodiscard]] DebugTypeWrap AUTDDebugTypeBaseSignal();

[[nodiscard]] DebugTypeWrap AUTDDebugTypeThermo();

[[nodiscard]] DebugTypeWrap AUTDDebugTypeForceFan();

[[nodiscard]] DebugTypeWrap AUTDDebugTypeSync();

[[nodiscard]] DebugTypeWrap AUTDDebugTypeModSegment();

[[nodiscard]] DebugTypeWrap AUTDDebugTypeModIdx(uint16_t value);

[[nodiscard]] DebugTypeWrap AUTDDebugTypeStmSegment();

[[nodiscard]] DebugTypeWrap AUTDDebugTypeStmIdx(uint16_t value);

[[nodiscard]] DebugTypeWrap AUTDDebugTypeIsStmMode();

[[nodiscard]] DebugTypeWrap AUTDDebugTypePwmOut(TransducerPtr value);

[[nodiscard]] DebugTypeWrap AUTDDebugTypeDirect(bool value);

[[nodiscard]] DebugTypeWrap AUTDDebugTypeSysTimeEq(DcSysTime sys_time);

[[nodiscard]] LoopBehavior AUTDLoopBehaviorInfinite();

[[nodiscard]] LoopBehavior AUTDLoopBehaviorFinite(uint16_t v);

[[nodiscard]] LoopBehavior AUTDLoopBehaviorOnce();

[[nodiscard]] uint8_t AUTDPhaseFromRad(float value);

[[nodiscard]] float AUTDPhaseToRad(uint8_t value);

[[nodiscard]] ResultSamplingConfig AUTDSamplingConfigFromDivision(uint16_t div);

[[nodiscard]] ResultSamplingConfig AUTDSamplingConfigFromFreq(uint32_t f);

[[nodiscard]] ResultSamplingConfig AUTDSamplingConfigFromFreqF(float f);

[[nodiscard]] SamplingConfig AUTDSamplingConfigFromFreqNearest(float f);

[[nodiscard]] ResultSamplingConfig AUTDSamplingConfigFromPeriod(Duration p);

[[nodiscard]] SamplingConfig AUTDSamplingConfigFromPeriodNearest(Duration p);

[[nodiscard]] uint16_t AUTDSamplingConfigDivision(SamplingConfig c);

[[nodiscard]] float AUTDSamplingConfigFreq(SamplingConfig c);

[[nodiscard]] Duration AUTDSamplingConfigPeriod(SamplingConfig c);

[[nodiscard]] TransitionModeWrap AUTDTransitionModeSyncIdx();

[[nodiscard]] TransitionModeWrap AUTDTransitionModeSysTime(DcSysTime sys_time);

[[nodiscard]] TransitionModeWrap AUTDTransitionModeGPIO(GPIOIn gpio);

[[nodiscard]] TransitionModeWrap AUTDTransitionModeExt();

[[nodiscard]] TransitionModeWrap AUTDTransitionModeImmediate();

[[nodiscard]] TransitionModeWrap AUTDTransitionModeNone();

[[nodiscard]]
DatagramPtr AUTDGainIntoDatagramWithSegment(GainPtr gain,
                                            Segment segment,
                                            TransitionModeWrap transition_mode);

[[nodiscard]] DatagramPtr AUTDGainIntoDatagram(GainPtr gain);

[[nodiscard]]
GainPtr AUTDGainBessel(Vector3 p,
                       Vector3 n,
                       float theta_z,
                       uint8_t intensity,
                       uint8_t phase_offset);

[[nodiscard]] bool AUTDGainBesselIsDefault(uint8_t intensity, uint8_t phase_offset);

[[nodiscard]] GainCachePtr AUTDGainCache(GainPtr g);

[[nodiscard]] GainPtr AUTDGainCacheClone(GainCachePtr g);

void AUTDGainCacheFree(GainCachePtr g);

[[nodiscard]] GainPtr AUTDGainCustom(const void* f, const void* context, GeometryPtr geometry);

[[nodiscard]] GainPtr AUTDGainFocus(Vector3 p, uint8_t intensity, uint8_t phase_offset);

[[nodiscard]] bool AUTDGainFocusIsDefault(uint8_t intensity, uint8_t phase_offset);

[[nodiscard]]
GroupGainMapPtr AUTDGainGroupCreateMap(const uint16_t *device_indices_ptr,
                                       uint16_t num_devices);

[[nodiscard]]
GroupGainMapPtr AUTDGainGroupMapSet(GroupGainMapPtr map,
                                    uint16_t dev_idx,
                                    const int32_t *map_data);

[[nodiscard]]
GainPtr AUTDGainGroup(GroupGainMapPtr map,
                      const int32_t *keys_ptr,
                      const GainPtr *values_ptr,
                      uint32_t kv_len);

[[nodiscard]] GainPtr AUTDGainNull();

[[nodiscard]] GainPtr AUTDGainPlane(Vector3 n, uint8_t intensity, uint8_t phase_offset);

[[nodiscard]] bool AUTDGainPlanelIsDefault(uint8_t intensity, uint8_t phase_offset);

[[nodiscard]] GainPtr AUTDGainUniform(uint8_t intensity, uint8_t phase);

[[nodiscard]] GeometryPtr AUTDGeometry(ControllerPtr cnt);

[[nodiscard]] uint32_t AUTDGeometryNumDevices(GeometryPtr geo);

[[nodiscard]] uint32_t AUTDGeometryNumTransducers(GeometryPtr geo);

[[nodiscard]] Vector3 AUTDGeometrCenter(GeometryPtr geo);

[[nodiscard]] DevicePtr AUTDDevice(GeometryPtr geo, uint16_t dev_idx);

[[nodiscard]] uint32_t AUTDDeviceNumTransducers(DevicePtr dev);

[[nodiscard]] float AUTDDeviceGetSoundSpeed(DevicePtr dev);

void AUTDDeviceSetSoundSpeed(GeometryPtr geo, uint16_t dev_idx, float value);

void AUTDDeviceSetSoundSpeedFromTemp(GeometryPtr geo,
                                     uint16_t dev_idx,
                                     float temp,
                                     float k,
                                     float r,
                                     float m);

Vector3 AUTDDeviceCenter(DevicePtr dev);

void AUTDDeviceTranslate(GeometryPtr geo, uint16_t dev_idx, Vector3 t);

void AUTDDeviceRotate(GeometryPtr geo, uint16_t dev_idx, Quaternion r);

void AUTDDeviceAffine(GeometryPtr geo, uint16_t dev_idx, Vector3 t, Quaternion r);

void AUTDDeviceEnableSet(GeometryPtr geo, uint16_t dev_idx, bool value);

[[nodiscard]] bool AUTDDeviceEnableGet(DevicePtr dev);

[[nodiscard]] float AUTDDeviceWavelength(DevicePtr dev);

[[nodiscard]] float AUTDDeviceWavenumber(DevicePtr dev);

Quaternion AUTDDeviceRotation(DevicePtr dev);

Vector3 AUTDDeviceDirectionX(DevicePtr dev);

Vector3 AUTDDeviceDirectionY(DevicePtr dev);

Vector3 AUTDDeviceDirectionAxial(DevicePtr dev);

Quaternion AUTDRotationFromEulerXYZ(float x, float y, float z);

Quaternion AUTDRotationFromEulerZYZ(float z1, float y, float z2);

[[nodiscard]] TransducerPtr AUTDTransducer(DevicePtr dev, uint8_t idx);

Vector3 AUTDTransducerPosition(TransducerPtr tr);

[[nodiscard]] LinkPtr AUTDLinkGet(ControllerPtr cnt);

[[nodiscard]] LinkBuilderPtr AUTDLinkAudit();

[[nodiscard]] bool AUTDLinkAuditIsOpen(LinkPtr audit);

void AUTDLinkAuditDown(LinkPtr audit);

void AUTDLinkAuditUp(LinkPtr audit);

void AUTDLinkAuditBreakDown(LinkPtr audit);

void AUTDLinkAuditRepair(LinkPtr audit);

[[nodiscard]] OptionDuration AUTDLinkAuditLastTimeout(LinkPtr audit);

[[nodiscard]] int64_t AUTDLinkAuditLastParallelThreshold(LinkPtr audit);

[[nodiscard]] uint32_t AUTDLinkAuditCpuNumTransducers(LinkPtr audit, uint16_t idx);

void AUTDLinkAuditFpgaAssertThermalSensor(LinkPtr audit, uint16_t idx);

void AUTDLinkAuditFpgaDeassertThermalSensor(LinkPtr audit, uint16_t idx);

[[nodiscard]] bool AUTDLinkAuditFpgaIsForceFan(LinkPtr audit, uint16_t idx);

[[nodiscard]] Segment AUTDLinkAuditFpgaCurrentStmSegment(LinkPtr audit, uint16_t idx);

[[nodiscard]] Segment AUTDLinkAuditFpgaCurrentModSegment(LinkPtr audit, uint16_t idx);

[[nodiscard]] bool AUTDLinkAuditFpgaIsStmGainMode(LinkPtr audit, Segment segment, uint16_t idx);

[[nodiscard]] bool AUTDLinkAuditCpuSilencerStrictMode(LinkPtr audit, uint16_t idx);

[[nodiscard]] uint16_t AUTDLinkAuditFpgaSilencerUpdateRateIntensity(LinkPtr audit, uint16_t idx);

[[nodiscard]] uint16_t AUTDLinkAuditFpgaSilencerUpdateRatePhase(LinkPtr audit, uint16_t idx);

[[nodiscard]]
Duration AUTDLinkAuditFpgaSilencerCompletionStepsIntensity(LinkPtr audit,
                                                           uint16_t idx);

[[nodiscard]] Duration AUTDLinkAuditFpgaSilencerCompletionStepsPhase(LinkPtr audit, uint16_t idx);

[[nodiscard]] bool AUTDLinkAuditFpgaSilencerFixedCompletionStepsMode(LinkPtr audit, uint16_t idx);

[[nodiscard]] SilencerTarget AUTDLinkAuditFpgaSilencerTarget(LinkPtr audit, uint16_t idx);

void AUTDLinkAuditFpgaDebugTypes(LinkPtr audit, uint16_t idx, uint8_t *ty);

void AUTDLinkAuditFpgaDebugValues(LinkPtr audit, uint16_t idx, uint64_t *value);

[[nodiscard]]
uint16_t AUTDLinkAuditFpgaStmFreqDivision(LinkPtr audit,
                                          Segment segment,
                                          uint16_t idx);

[[nodiscard]] uint16_t AUTDLinkAuditFpgaStmCycle(LinkPtr audit, Segment segment, uint16_t idx);

[[nodiscard]] uint16_t AUTDLinkAuditFpgaSoundSpeed(LinkPtr audit, Segment segment, uint16_t idx);

[[nodiscard]]
LoopBehavior AUTDLinkAuditFpgaStmLoopBehavior(LinkPtr audit,
                                              Segment segment,
                                              uint16_t idx);

[[nodiscard]]
uint16_t AUTDLinkAuditFpgaModulationFreqDivision(LinkPtr audit,
                                                 Segment segment,
                                                 uint16_t idx);

[[nodiscard]]
uint16_t AUTDLinkAuditFpgaModulationCycle(LinkPtr audit,
                                          Segment segment,
                                          uint16_t idx);

void AUTDLinkAuditFpgaModulationBuffer(LinkPtr audit,
                                       Segment segment,
                                       uint16_t idx,
                                       uint8_t *data,
                                       uint32_t size);

[[nodiscard]]
LoopBehavior AUTDLinkAuditFpgaModulationLoopBehavior(LinkPtr audit,
                                                     Segment segment,
                                                     uint16_t idx);

void AUTDLinkAuditFpgaDrivesAt(LinkPtr audit,
                               Segment segment,
                               uint16_t idx,
                               uint16_t stm_idx,
                               Drive *drive);

void AUTDLinkAuditFpgaPulseWidthEncoderTable(LinkPtr audit, uint16_t idx, uint8_t *dst);

[[nodiscard]] LinkBuilderPtr AUTDLinkNop();

[[nodiscard]] SamplingConfig AUTDModulationSamplingConfig(ModulationPtr m);

[[nodiscard]]
DatagramPtr AUTDModulationIntoDatagramWithSegment(ModulationPtr m,
                                                  Segment segment,
                                                  TransitionModeWrap transition_mode);

[[nodiscard]] DatagramPtr AUTDModulationIntoDatagram(ModulationPtr m);

[[nodiscard]] ModulationCachePtr AUTDModulationCache(ModulationPtr m);

[[nodiscard]]
ModulationPtr AUTDModulationCacheClone(ModulationCachePtr m,
                                       LoopBehavior loop_behavior);

void AUTDModulationCacheFree(ModulationCachePtr m);

[[nodiscard]]
ModulationPtr AUTDModulationCustom(SamplingConfig config,
                                   LoopBehavior loop_behavior,
                                   const uint8_t *ptr,
                                   uint16_t len);

[[nodiscard]]
ModulationPtr AUTDModulationCustomWithResample(LoopBehavior loop_behavior,
                                               const uint8_t *ptr,
                                               uint16_t len,
                                               float src,
                                               SamplingConfig target,
                                               DynSincInterpolator resample);

[[nodiscard]]
ModulationPtr AUTDModulationWithFir(ModulationPtr m,
                                    LoopBehavior loop_behavior,
                                    const float *coef,
                                    uint32_t n_tap);

[[nodiscard]]
ResultModulation AUTDModulationFourierExact(const uint32_t *sine_freq,
                                            const SamplingConfig *sine_config,
                                            const uint8_t *sine_intensity,
                                            const uint8_t *sine_offset,
                                            const float *sine_phase,
                                            const bool *sine_clamp,
                                            uint32_t size,
                                            bool clamp,
                                            float scale_factor,
                                            uint8_t offset,
                                            LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationFourierExactFloat(const float *sine_freq,
                                                 const SamplingConfig *sine_config,
                                                 const uint8_t *sine_intensity,
                                                 const uint8_t *sine_offset,
                                                 const float *sine_phase,
                                                 const bool *sine_clamp,
                                                 uint32_t size,
                                                 bool clamp,
                                                 float scale_factor,
                                                 uint8_t offset,
                                                 LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationFourierNearest(const float *sine_freq,
                                              const SamplingConfig *sine_config,
                                              const uint8_t *sine_intensity,
                                              const uint8_t *sine_offset,
                                              const float *sine_phase,
                                              const bool *sine_clamp,
                                              uint32_t size,
                                              bool clamp,
                                              float scale_factor,
                                              uint8_t offset,
                                              LoopBehavior loop_behavior);

[[nodiscard]]
ModulationPtr AUTDModulationWithRadiationPressure(ModulationPtr m,
                                                  LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationSineExact(uint32_t freq,
                                         SamplingConfig config,
                                         uint8_t intensity,
                                         uint8_t offset,
                                         float phase,
                                         bool clamp,
                                         LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationSineExactFloat(float freq,
                                              SamplingConfig config,
                                              uint8_t intensity,
                                              uint8_t offset,
                                              float phase,
                                              bool clamp,
                                              LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationSineNearest(float freq,
                                           SamplingConfig config,
                                           uint8_t intensity,
                                           uint8_t offset,
                                           float phase,
                                           bool clamp,
                                           LoopBehavior loop_behavior);

[[nodiscard]] uint32_t AUTDModulationSineExactFreq(uint32_t freq);

[[nodiscard]] float AUTDModulationSineExactFloatFreq(float freq);

[[nodiscard]] float AUTDModulationSineNearestFreq(float freq);

[[nodiscard]]
bool AUTDModulationSineIsDefault(SamplingConfig config,
                                 uint8_t intensity,
                                 uint8_t offset,
                                 float phase,
                                 bool clamp,
                                 LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationSquareExact(uint32_t freq,
                                           SamplingConfig config,
                                           uint8_t low,
                                           uint8_t high,
                                           float duty,
                                           LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationSquareExactFloat(float freq,
                                                SamplingConfig config,
                                                uint8_t low,
                                                uint8_t high,
                                                float duty,
                                                LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationSquareNearest(float freq,
                                             SamplingConfig config,
                                             uint8_t low,
                                             uint8_t high,
                                             float duty,
                                             LoopBehavior loop_behavior);

[[nodiscard]] uint32_t AUTDModulationSquareExactFreq(uint32_t freq);

[[nodiscard]] float AUTDModulationSquareExactFloatFreq(float freq);

[[nodiscard]] float AUTDModulationSquareNearestFreq(float freq);

[[nodiscard]]
bool AUTDModulationSquareIsDefault(SamplingConfig config,
                                   uint8_t low,
                                   uint8_t high,
                                   float duty,
                                   LoopBehavior loop_behavior);

[[nodiscard]] ModulationPtr AUTDModulationStatic(uint8_t intensity, LoopBehavior loop_behavior);

[[nodiscard]] bool AUTDModulationStaticIsDefault(uint8_t intensity);

void AUTDGetErr(const void* src, char *dst);

} // extern "C"

} // namespace autd3::native_methods
