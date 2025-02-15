#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

struct ResultController {
  ControllerPtr result;
  uint32_t err_len;
  const void* err;
};

struct SenderOption {
  Duration send_interval;
  Duration receive_interval;
  OptionDuration timeout;
  ParallelMode parallel;
  SleeperWrap sleeper;
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

struct FixedCompletionTime {
  Duration intensity;
  Duration phase;
  bool strict_mode;
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

struct FourierOption {
  bool has_scale_factor;
  float scale_factor;
  bool clamp;
  uint8_t offset;
};

extern "C" {

void AUTDTracingInit();

ResultStatus AUTDTracingInitWithFile(const char *path);

[[nodiscard]]
ResultController AUTDControllerOpen(const Point3 *pos,
                                    const Quaternion *rot,
                                    uint16_t len,
                                    LinkPtr link,
                                    SenderOption option);

[[nodiscard]] ResultStatus AUTDControllerClose(ControllerPtr cnt);

[[nodiscard]] ResultFPGAStateList AUTDControllerFPGAState(ControllerPtr cnt);

int16_t AUTDControllerFPGAStateGet(FPGAStateListPtr p, uint32_t idx);

void AUTDControllerFPGAStateDelete(FPGAStateListPtr p);

[[nodiscard]] ResultFirmwareVersionList AUTDControllerFirmwareVersionListPointer(ControllerPtr cnt);

void AUTDControllerFirmwareVersionGet(FirmwareVersionListPtr p_info_list, uint32_t idx, char *info);

void AUTDControllerFirmwareVersionListPointerDelete(FirmwareVersionListPtr p_info_list);

void AUTDFirmwareLatest(char *latest);

[[nodiscard]]
ResultStatus AUTDControllerGroup(SenderPtr sender,
                                 const void* f,
                                 const void* context,
                                 GeometryPtr geometry,
                                 const int32_t *keys,
                                 const DatagramPtr *d,
                                 uint16_t n);

[[nodiscard]] SenderPtr AUTDSender(ControllerPtr cnt, SenderOption option);

[[nodiscard]] ResultStatus AUTDSenderSend(SenderPtr sender, DatagramPtr d);

[[nodiscard]] uint32_t AUTDSpinSleepDefaultAccuracy();

[[nodiscard]] bool AUTDSenderOptionIsDefault(SenderOption option);

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
DatagramPtr AUTDDatagramSilencerFromUpdateRate(FixedUpdateRate config,
                                               SilencerTarget target);

[[nodiscard]]
DatagramPtr AUTDDatagramSilencerFromCompletionSteps(FixedCompletionSteps config,
                                                    SilencerTarget target);

[[nodiscard]]
DatagramPtr AUTDDatagramSilencerFromCompletionTime(FixedCompletionTime config,
                                                   SilencerTarget target);

[[nodiscard]]
bool AUTDDatagramSilencerFixedCompletionStepsIsDefault(FixedCompletionSteps config,
                                                       SilencerTarget target);

[[nodiscard]] ResultSamplingConfig AUTDSTMConfigFromFreq(float f, uint16_t n);

[[nodiscard]] ResultSamplingConfig AUTDSTMConfigFromPeriod(Duration p, uint16_t n);

[[nodiscard]] SamplingConfig AUTDSTMConfigFromFreqNearest(float f, uint16_t n);

[[nodiscard]] SamplingConfig AUTDSTMConfigFromPeriodNearest(Duration p, uint16_t n);

[[nodiscard]]
FociSTMPtr AUTDSTMFoci(SamplingConfig config,
                       const void* points,
                       uint16_t size,
                       uint8_t n);

[[nodiscard]]
DatagramPtr AUTDSTMFociIntoDatagramWithSegment(FociSTMPtr stm,
                                               uint8_t n,
                                               Segment segment,
                                               TransitionModeWrap transition_mode);

[[nodiscard]]
DatagramPtr AUTDSTMFociIntoDatagramWithLoopBehavior(FociSTMPtr stm,
                                                    uint8_t n,
                                                    Segment segment,
                                                    TransitionModeWrap transition_mode,
                                                    LoopBehavior loop_behavior);

[[nodiscard]] DatagramPtr AUTDSTMFociIntoDatagram(FociSTMPtr stm, uint8_t n);

[[nodiscard]]
GainSTMPtr AUTDSTMGain(SamplingConfig config,
                       const GainPtr *gains,
                       uint16_t size,
                       GainSTMOption option);

[[nodiscard]]
DatagramPtr AUTDSTMGainIntoDatagramWithSegment(GainSTMPtr stm,
                                               Segment segment,
                                               TransitionModeWrap transition_mode);

[[nodiscard]]
DatagramPtr AUTDSTMGainIntoDatagramWithLoopBehavior(GainSTMPtr stm,
                                                    Segment segment,
                                                    TransitionModeWrap transition_mode,
                                                    LoopBehavior loop_behavior);

[[nodiscard]] DatagramPtr AUTDSTMGainIntoDatagram(GainSTMPtr stm);

[[nodiscard]] DatagramPtr AUTDDatagramSynchronize();

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

[[nodiscard]] uint8_t AUTDPhaseFromRad(float value);

[[nodiscard]] float AUTDPhaseToRad(Phase value);

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

[[nodiscard]] GainPtr AUTDGainBessel(Point3 pos, Vector3 dir, Angle theta, BesselOption option);

[[nodiscard]] bool AUTDGainBesselIsDefault(BesselOption option);

[[nodiscard]] GainCachePtr AUTDGainCache(GainPtr g);

[[nodiscard]] GainPtr AUTDGainCacheClone(GainCachePtr g);

void AUTDGainCacheFree(GainCachePtr g);

[[nodiscard]] GainPtr AUTDGainCustom(const void* f, const void* context, GeometryPtr geometry);

[[nodiscard]] GainPtr AUTDGainFocus(Point3 pos, FocusOption option);

[[nodiscard]] bool AUTDGainFocusIsDefault(FocusOption option);

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

[[nodiscard]] GainPtr AUTDGainPlane(Vector3 n, PlaneOption option);

[[nodiscard]] bool AUTDGainPlanelIsDefault(PlaneOption option);

[[nodiscard]] GainPtr AUTDGainUniform(EmitIntensity intensity, Phase phase);

[[nodiscard]] GeometryPtr AUTDGeometry(ControllerPtr cnt);

[[nodiscard]] uint32_t AUTDGeometryNumDevices(GeometryPtr geo);

[[nodiscard]] uint32_t AUTDGeometryNumTransducers(GeometryPtr geo);

[[nodiscard]] Point3 AUTDGeometrCenter(GeometryPtr geo);

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

Point3 AUTDDeviceCenter(DevicePtr dev);

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

Point3 AUTDTransducerPosition(TransducerPtr tr);

[[nodiscard]] LinkPtr AUTDLinkGet(ControllerPtr cnt);

[[nodiscard]] LinkPtr AUTDLinkAudit();

[[nodiscard]] bool AUTDLinkAuditIsOpen(LinkPtr audit);

void AUTDLinkAuditDown(LinkPtr audit);

void AUTDLinkAuditUp(LinkPtr audit);

void AUTDLinkAuditBreakDown(LinkPtr audit);

void AUTDLinkAuditRepair(LinkPtr audit);

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
uint16_t AUTDLinkAuditFpgaSilencerCompletionStepsIntensity(LinkPtr audit,
                                                           uint16_t idx);

[[nodiscard]] uint16_t AUTDLinkAuditFpgaSilencerCompletionStepsPhase(LinkPtr audit, uint16_t idx);

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

[[nodiscard]] LinkPtr AUTDLinkNop();

[[nodiscard]] ResultSamplingConfig AUTDModulationSamplingConfig(ModulationPtr m);

[[nodiscard]]
DatagramPtr AUTDModulationIntoDatagramWithSegment(ModulationPtr m,
                                                  Segment segment,
                                                  TransitionModeWrap transition_mode);

[[nodiscard]]
DatagramPtr AUTDModulationIntoDatagramWithLoopBehavior(ModulationPtr m,
                                                       Segment segment,
                                                       TransitionModeWrap transition_mode,
                                                       LoopBehavior loop_behavior);

[[nodiscard]] DatagramPtr AUTDModulationIntoDatagram(ModulationPtr m);

[[nodiscard]] ModulationCachePtr AUTDModulationCache(ModulationPtr m);

[[nodiscard]] ModulationPtr AUTDModulationCacheClone(ModulationCachePtr m);

void AUTDModulationCacheFree(ModulationCachePtr m);

[[nodiscard]]
ModulationPtr AUTDModulationCustom(const uint8_t *ptr,
                                   uint16_t len,
                                   SamplingConfig sampling_config);

[[nodiscard]]
ModulationPtr AUTDModulationWithFir(ModulationPtr m,
                                    const float *coef,
                                    uint32_t n_tap);

[[nodiscard]]
ModulationPtr AUTDModulationFourierExact(const uint32_t *sine_freq,
                                         const SineOption *sine_clamp,
                                         uint32_t size,
                                         FourierOption option);

[[nodiscard]]
ModulationPtr AUTDModulationFourierExactFloat(const float *sine_freq,
                                              const SineOption *sine_clamp,
                                              uint32_t size,
                                              FourierOption option);

[[nodiscard]]
ModulationPtr AUTDModulationFourierNearest(const float *sine_freq,
                                           const SineOption *sine_clamp,
                                           uint32_t size,
                                           FourierOption option);

[[nodiscard]] ModulationPtr AUTDModulationWithRadiationPressure(ModulationPtr m);

[[nodiscard]] ModulationPtr AUTDModulationSineExact(uint32_t freq, SineOption option);

[[nodiscard]] ModulationPtr AUTDModulationSineExactFloat(float freq, SineOption option);

[[nodiscard]] ModulationPtr AUTDModulationSineNearest(float freq, SineOption option);

[[nodiscard]] bool AUTDModulationSineIsDefault(SineOption option);

[[nodiscard]] ModulationPtr AUTDModulationSquareExact(uint32_t freq, SquareOption option);

[[nodiscard]] ModulationPtr AUTDModulationSquareExactFloat(float freq, SquareOption option);

[[nodiscard]] ModulationPtr AUTDModulationSquareNearest(float freq, SquareOption option);

[[nodiscard]] bool AUTDModulationSquareIsDefault(SquareOption option);

[[nodiscard]] ModulationPtr AUTDModulationStatic(uint8_t intensity);

[[nodiscard]] bool AUTDModulationStaticIsDefault(uint8_t intensity);

void AUTDGetErr(const void* src, char *dst);

} // extern "C"

} // namespace autd3::native_methods
