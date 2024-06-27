#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>
#include "autd3/native_methods/def.hpp"

namespace autd3::native_methods {

struct RuntimePtr {
  const void *_0;
};

struct ControllerPtr {
  const void *_0;
};

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

struct ControllerBuilderPtr {
  const void *_0;
};

struct GainCalcDrivesMapPtr {
  const void *_0;
};

struct ResultGainCalcDrivesMap {
  GainCalcDrivesMapPtr result;
  uint32_t err_len;
  const void* err;
};

struct GroupGainMapPtr {
  const void *_0;
};

struct LinkAuditBuilderPtr {
  const void *_0;
};

struct ModulationCalcPtr {
  const void *_0;
};

struct ResultModulationCalc {
  ModulationCalcPtr result;
  SamplingConfigWrap config;
  uint32_t err_len;
  const void* err;
};

extern "C" {

[[nodiscard]] RuntimePtr AUTDCreateRuntime();

void AUTDDeleteRuntime(RuntimePtr runtime);

[[nodiscard]] ResultI32 AUTDWaitResultI32(RuntimePtr runtime, FfiFutureResultI32 future);

[[nodiscard]]
ResultI32 AUTDWaitLocalResultI32(RuntimePtr runtime,
                                 LocalFfiFutureResultI32 future);

[[nodiscard]]
ResultController AUTDWaitResultController(RuntimePtr runtime,
                                          FfiFutureResultController future);

[[nodiscard]]
ResultFPGAStateList AUTDWaitResultFPGAStateList(RuntimePtr runtime,
                                                FfiFutureResultFPGAStateList future);

[[nodiscard]]
ResultFirmwareVersionList AUTDWaitResultFirmwareVersionList(RuntimePtr runtime,
                                                            FfiFutureResultFirmwareVersionList future);

void AUTDTracingInit(uint8_t level);

[[nodiscard]] FfiFutureResultI32 AUTDControllerClose(ControllerPtr cnt);

void AUTDControllerDelete(ControllerPtr cnt);

[[nodiscard]] uint16_t AUTDControllerLastParallelThreshold(ControllerPtr cnt);

[[nodiscard]] FfiFutureResultFPGAStateList AUTDControllerFPGAState(ControllerPtr cnt);

int16_t AUTDControllerFPGAStateGet(FPGAStateListPtr p, uint32_t idx);

void AUTDControllerFPGAStateDelete(FPGAStateListPtr p);

[[nodiscard]]
FfiFutureResultFirmwareVersionList AUTDControllerFirmwareVersionListPointer(ControllerPtr cnt);

void AUTDControllerFirmwareVersionGet(FirmwareVersionListPtr p_info_list, uint32_t idx, char *info);

void AUTDControllerFirmwareVersionListPointerDelete(FirmwareVersionListPtr p_info_list);

void AUTDFirmwareLatest(char *latest);

[[nodiscard]] FfiFutureResultI32 AUTDControllerSend(ControllerPtr cnt, DatagramPtr d);

[[nodiscard]]
ControllerBuilderPtr AUTDControllerBuilder(const Vector3 *pos,
                                           const Quaternion *rot,
                                           uint16_t len);

[[nodiscard]]
ControllerBuilderPtr AUTDControllerBuilderWithParallelThreshold(ControllerBuilderPtr builder,
                                                                uint16_t parallel_threshold);

[[nodiscard]]
ControllerBuilderPtr AUTDControllerBuilderWithSendInterval(ControllerBuilderPtr builder,
                                                           uint64_t interval_ns);

[[nodiscard]]
ControllerBuilderPtr AUTDControllerBuilderWithTimerResolution(ControllerBuilderPtr builder,
                                                              uint32_t resolution);

[[nodiscard]]
FfiFutureResultController AUTDControllerOpen(ControllerBuilderPtr builder,
                                               LinkBuilderPtr link_builder,
                                               int64_t timeout_ns);

[[nodiscard]]
LocalFfiFutureResultI32 AUTDControllerGroup(ControllerPtr cnt,
                                              const void* f,
                                              const void* context,
                                              GeometryPtr geometry,
                                              const int32_t *keys,
                                              const DatagramPtr *d,
                                              uint16_t n);

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

[[nodiscard]] DatagramPtr AUTDDatagramSwapSegmentGain(Segment segment);

[[nodiscard]]
DatagramPtr AUTDDatagramSilencerFromUpdateRate(uint16_t value_intensity,
                                               uint16_t value_phase);

[[nodiscard]]
DatagramPtr AUTDDatagramSilencerFromCompletionSteps(uint16_t value_intensity,
                                                    uint16_t value_phase,
                                                    bool strict_mode);

[[nodiscard]]
DatagramPtr AUTDDatagramSilencerFromCompletionTime(uint64_t value_intensity,
                                                   uint64_t value_phase,
                                                   bool strict_mode);

[[nodiscard]] bool AUTDDatagramSilencerFixedCompletionStepsIsDefault(DatagramPtr silencer);

[[nodiscard]] STMSamplingConfigWrap AUTDSTMSamplingConfigFromFreq(float f);

[[nodiscard]] STMSamplingConfigWrap AUTDSTMSamplingConfigFromFreqNearest(float f);

[[nodiscard]] STMSamplingConfigWrap AUTDSTMSamplingConfigFromPeriod(uint64_t p);

[[nodiscard]] STMSamplingConfigWrap AUTDSTMSamplingConfigFromPeriodNearest(uint64_t p);

[[nodiscard]] STMSamplingConfigWrap AUTDSTMSamplingConfigFromSamplingConfig(SamplingConfigWrap c);

[[nodiscard]] ResultF32 AUTDSTMFreq(STMSamplingConfigWrap c, uint32_t n);

[[nodiscard]] ResultU64 AUTDSTMPeriod(STMSamplingConfigWrap c, uint32_t n);

[[nodiscard]]
ResultSamplingConfigWrap AUTDSTMSamplingSamplingConfig(STMSamplingConfigWrap c,
                                                       uint32_t n);

[[nodiscard]]
ResultFociSTM AUTDSTMFoci(STMSamplingConfigWrap config,
                          const void* points,
                          uint16_t size,
                          uint8_t n);

[[nodiscard]]
FociSTMPtr AUTDSTMFociWithLoopBehavior(FociSTMPtr stm,
                                       uint8_t n,
                                       LoopBehavior loop_behavior);

[[nodiscard]]
DatagramPtr AUTDSTMFociIntoDatagramWithSegment(FociSTMPtr stm,
                                               uint8_t n,
                                               Segment segment);

[[nodiscard]]
DatagramPtr AUTDSTMFociIntoDatagramWithSegmentTransition(FociSTMPtr stm,
                                                         uint8_t n,
                                                         Segment segment,
                                                         TransitionModeWrap transition_mode);

[[nodiscard]] DatagramPtr AUTDSTMFociIntoDatagram(FociSTMPtr stm, uint8_t n);

[[nodiscard]]
ResultGainSTM AUTDSTMGain(STMSamplingConfigWrap config,
                          const GainPtr *gains,
                          uint16_t size);

[[nodiscard]] GainSTMPtr AUTDSTMGainWithMode(GainSTMPtr stm, GainSTMMode mode);

[[nodiscard]] GainSTMPtr AUTDSTMGainWithLoopBehavior(GainSTMPtr stm, LoopBehavior loop_behavior);

[[nodiscard]] DatagramPtr AUTDSTMGainIntoDatagramWithSegment(GainSTMPtr stm, Segment segment);

[[nodiscard]]
DatagramPtr AUTDSTMGainIntoDatagramWithSegmentTransition(GainSTMPtr stm,
                                                         Segment segment,
                                                         TransitionModeWrap transition_mode);

[[nodiscard]] DatagramPtr AUTDSTMGainIntoDatagram(GainSTMPtr stm);

[[nodiscard]] DatagramPtr AUTDDatagramSynchronize();

[[nodiscard]] DatagramPtr AUTDDatagramWithParallelThreshold(DatagramPtr d, uint16_t threshold);

[[nodiscard]] DatagramPtr AUTDDatagramWithTimeout(DatagramPtr d, uint64_t timeout_ns);

[[nodiscard]] uint64_t AUTDDcSysTimeNow();

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

[[nodiscard]] LoopBehavior AUTDLoopBehaviorInfinite();

[[nodiscard]] LoopBehavior AUTDLoopBehaviorFinite(uint32_t v);

[[nodiscard]] LoopBehavior AUTDLoopBehaviorOnce();

[[nodiscard]] uint8_t AUTDPhaseFromRad(float value);

[[nodiscard]] float AUTDPhaseToRad(uint8_t value);

[[nodiscard]] SamplingConfigWrap AUTDSamplingConfigFromDivision(uint32_t div);

[[nodiscard]] SamplingConfigWrap AUTDSamplingConfigFromDivisionRaw(uint32_t div);

[[nodiscard]] SamplingConfigWrap AUTDSamplingConfigFromFreq(uint32_t f);

[[nodiscard]] SamplingConfigWrap AUTDSamplingConfigFromFreqNearest(float f);

[[nodiscard]] SamplingConfigWrap AUTDSamplingConfigFromPeriod(uint64_t p);

[[nodiscard]] SamplingConfigWrap AUTDSamplingConfigFromPeriodNearest(uint64_t p);

[[nodiscard]] ResultU32 AUTDSamplingConfigDivision(SamplingConfigWrap c);

[[nodiscard]] ResultF32 AUTDSamplingConfigFreq(SamplingConfigWrap c);

[[nodiscard]] ResultU64 AUTDSamplingConfigPeriod(SamplingConfigWrap c);

[[nodiscard]] TransitionModeWrap AUTDTransitionModeSyncIdx();

[[nodiscard]] TransitionModeWrap AUTDTransitionModeSysTime(uint64_t sys_time);

[[nodiscard]] TransitionModeWrap AUTDTransitionModeGPIO(GPIOIn gpio);

[[nodiscard]] TransitionModeWrap AUTDTransitionModeExt();

[[nodiscard]] TransitionModeWrap AUTDTransitionModeImmediate();

[[nodiscard]]
DatagramPtr AUTDGainIntoDatagramWithSegment(GainPtr gain,
                                            Segment segment,
                                            bool update_segment);

[[nodiscard]] DatagramPtr AUTDGainIntoDatagram(GainPtr gain);

[[nodiscard]] ResultGainCalcDrivesMap AUTDGainCalc(GainPtr gain, GeometryPtr geometry);

void AUTDGainCalcGetResult(GainCalcDrivesMapPtr src, Drive *dst, DevicePtr device);

void AUTDGainCalcFreeResult(GainCalcDrivesMapPtr src);

[[nodiscard]]
GainPtr AUTDGainBessel(Vector3 p,
                       Vector3 n,
                       float theta_z,
                       uint8_t intensity,
                       uint8_t phase_offset);

[[nodiscard]] bool AUTDGainBesselIsDefault(GainPtr bessel);

[[nodiscard]] GainPtr AUTDGainCustom(const void* f, const void* context, GeometryPtr geometry);

[[nodiscard]] GainPtr AUTDGainFocus(Vector3 p, uint8_t intensity, uint8_t phase_offset);

[[nodiscard]] bool AUTDGainFocusIsDefault(GainPtr focus);

[[nodiscard]]
GroupGainMapPtr AUTDGainGroupCreateMap(const uint32_t *device_indices_ptr,
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

[[nodiscard]] bool AUTDGainPlanelIsDefault(GainPtr plane);

[[nodiscard]] GainPtr AUTDGainRaw();

[[nodiscard]]
GainPtr AUTDGainRawSet(GainPtr custom,
                       uint16_t dev_idx,
                       const Drive *ptr,
                       uint8_t len);

[[nodiscard]]
GainPtr AUTDGainWithTransform(GainPtr g,
                              const void* f,
                              const void* context,
                              GeometryPtr geometry);

[[nodiscard]] GainPtr AUTDGainUniform(uint8_t intensity, uint8_t phase);

[[nodiscard]] bool AUTDGainUniformIsDefault(GainPtr uniform);

[[nodiscard]] GeometryPtr AUTDGeometry(ControllerPtr cnt);

[[nodiscard]] uint32_t AUTDGeometryNumDevices(GeometryPtr geo);

[[nodiscard]] DevicePtr AUTDDevice(GeometryPtr geo, uint16_t dev_idx);

[[nodiscard]] uint32_t AUTDDeviceNumTransducers(DevicePtr dev);

[[nodiscard]] float AUTDDeviceGetSoundSpeed(DevicePtr dev);

void AUTDDeviceSetSoundSpeed(DevicePtr dev, float value);

void AUTDDeviceSetSoundSpeedFromTemp(DevicePtr dev, float temp, float k, float r, float m);

Vector3 AUTDDeviceCenter(DevicePtr dev);

void AUTDDeviceTranslate(DevicePtr dev, Vector3 t);

void AUTDDeviceRotate(DevicePtr dev, Quaternion r);

void AUTDDeviceAffine(DevicePtr dev, Vector3 t, Quaternion r);

void AUTDDeviceEnableSet(DevicePtr dev, bool value);

[[nodiscard]] bool AUTDDeviceEnableGet(DevicePtr dev);

[[nodiscard]] float AUTDDeviceWavelength(DevicePtr dev);

[[nodiscard]] float AUTDDeviceWavenumber(DevicePtr dev);

Quaternion AUTDDeviceRotation(DevicePtr dev);

Vector3 AUTDDeviceDirectionX(DevicePtr dev);

Vector3 AUTDDeviceDirectionY(DevicePtr dev);

Vector3 AUTDDeviceDirectionAxial(DevicePtr dev);

Quaternion AUTDRotationFromEulerZYZ(float x, float y, float z);

[[nodiscard]] TransducerPtr AUTDTransducer(DevicePtr dev, uint8_t idx);

Vector3 AUTDTransducerPosition(TransducerPtr tr);

[[nodiscard]] LinkPtr AUTDLinkGet(ControllerPtr cnt);

[[nodiscard]] LinkAuditBuilderPtr AUTDLinkAudit();

[[nodiscard]]
LinkAuditBuilderPtr AUTDLinkAuditWithTimeout(LinkAuditBuilderPtr audit,
                                             uint64_t timeout_ns);

[[nodiscard]] LinkBuilderPtr AUTDLinkAuditIntoBuilder(LinkAuditBuilderPtr audit);

[[nodiscard]] bool AUTDLinkAuditIsOpen(LinkPtr audit);

[[nodiscard]] uint64_t AUTDLinkAuditTimeoutNs(LinkPtr audit);

[[nodiscard]] int64_t AUTDLinkAuditLastTimeoutNs(LinkPtr audit);

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

void AUTDLinkAuditFpgaDebugTypes(LinkPtr audit, uint16_t idx, uint8_t *ty);

void AUTDLinkAuditFpgaDebugValues(LinkPtr audit, uint16_t idx, uint16_t *value);

[[nodiscard]]
uint32_t AUTDLinkAuditFpgaStmFreqDivision(LinkPtr audit,
                                          Segment segment,
                                          uint16_t idx);

[[nodiscard]] uint16_t AUTDLinkAuditFpgaStmCycle(LinkPtr audit, Segment segment, uint16_t idx);

[[nodiscard]] uint16_t AUTDLinkAuditFpgaSoundSpeed(LinkPtr audit, Segment segment, uint16_t idx);

[[nodiscard]]
LoopBehavior AUTDLinkAuditFpgaStmLoopBehavior(LinkPtr audit,
                                              Segment segment,
                                              uint16_t idx);

[[nodiscard]]
uint32_t AUTDLinkAuditFpgaModulationFreqDivision(LinkPtr audit,
                                                 Segment segment,
                                                 uint16_t idx);

[[nodiscard]]
uint16_t AUTDLinkAuditFpgaModulationCycle(LinkPtr audit,
                                          Segment segment,
                                          uint16_t idx);

void AUTDLinkAuditFpgaModulation(LinkPtr audit, Segment segment, uint16_t idx, uint8_t *data);

[[nodiscard]]
LoopBehavior AUTDLinkAuditFpgaModulationLoopBehavior(LinkPtr audit,
                                                     Segment segment,
                                                     uint16_t idx);

void AUTDLinkAuditFpgaDrives(LinkPtr audit,
                             Segment segment,
                             uint16_t idx,
                             uint16_t stm_idx,
                             uint8_t *intensities,
                             uint8_t *phases);

[[nodiscard]]
uint16_t AUTDLinkAuditFpgaPulseWidthEncoderTable(LinkPtr audit,
                                                 uint16_t idx,
                                                 uint8_t *dst);

[[nodiscard]] uint32_t AUTDLinkAuditFpgaUltrasoundFreq(LinkPtr audit, uint16_t idx);

[[nodiscard]] LinkBuilderPtr AUTDLinkNop();

[[nodiscard]] SamplingConfigWrap AUTDModulationSamplingConfig(ModulationPtr m);

[[nodiscard]] DatagramPtr AUTDModulationIntoDatagramWithSegment(ModulationPtr m, Segment segment);

[[nodiscard]]
DatagramPtr AUTDModulationIntoDatagramWithSegmentTransition(ModulationPtr m,
                                                            Segment segment,
                                                            TransitionModeWrap transition_mode);

[[nodiscard]] DatagramPtr AUTDModulationIntoDatagram(ModulationPtr m);

[[nodiscard]] ResultModulationCalc AUTDModulationCalc(ModulationPtr m);

void AUTDModulationCalcGetResult(ModulationCalcPtr src, uint8_t *dst);

uint16_t AUTDModulationCalcGetSize(ModulationCalcPtr src);

void AUTDModulationCalcFreeResult(ModulationCalcPtr src);

[[nodiscard]]
ResultModulation AUTDModulationFourierExact(const ModulationPtr *components,
                                            uint32_t size,
                                            LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationFourierExactFloat(const ModulationPtr *components,
                                                 uint32_t size,
                                                 LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationFourierNearest(const ModulationPtr *components,
                                              uint32_t size,
                                              LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationMixerExact(const ModulationPtr *components,
                                          uint32_t size,
                                          LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationMixerExactFloat(const ModulationPtr *components,
                                               uint32_t size,
                                               LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationMixerNearest(const ModulationPtr *components,
                                            uint32_t size,
                                            LoopBehavior loop_behavior);

[[nodiscard]]
ModulationPtr AUTDModulationWithRadiationPressure(ModulationPtr m,
                                                  LoopBehavior loop_behavior);

[[nodiscard]]
ModulationPtr AUTDModulationRaw(SamplingConfigWrap config,
                                LoopBehavior loop_behavior,
                                const uint8_t *ptr,
                                uint16_t len);

[[nodiscard]]
ModulationPtr AUTDModulationSineExact(uint32_t freq,
                                      SamplingConfigWrap config,
                                      uint8_t intensity,
                                      uint8_t offset,
                                      float phase,
                                      LoopBehavior loop_behavior);

[[nodiscard]]
ModulationPtr AUTDModulationSineExactFloat(float freq,
                                           SamplingConfigWrap config,
                                           uint8_t intensity,
                                           uint8_t offset,
                                           float phase,
                                           LoopBehavior loop_behavior);

[[nodiscard]]
ModulationPtr AUTDModulationSineNearest(float freq,
                                        SamplingConfigWrap config,
                                        uint8_t intensity,
                                        uint8_t offset,
                                        float phase,
                                        LoopBehavior loop_behavior);

[[nodiscard]] bool AUTDModulationSineIsDefault(ModulationPtr sine);

[[nodiscard]]
ModulationPtr AUTDModulationSquareExact(uint32_t freq,
                                        SamplingConfigWrap config,
                                        uint8_t low,
                                        uint8_t high,
                                        float duty,
                                        LoopBehavior loop_behavior);

[[nodiscard]]
ModulationPtr AUTDModulationSquareExactFloat(float freq,
                                             SamplingConfigWrap config,
                                             uint8_t low,
                                             uint8_t high,
                                             float duty,
                                             LoopBehavior loop_behavior);

[[nodiscard]]
ModulationPtr AUTDModulationSquareNearest(float freq,
                                          SamplingConfigWrap config,
                                          uint8_t low,
                                          uint8_t high,
                                          float duty,
                                          LoopBehavior loop_behavior);

[[nodiscard]] bool AUTDModulationSquareIsDefault(ModulationPtr square);

[[nodiscard]] ModulationPtr AUTDModulationStatic(uint8_t intensity, LoopBehavior loop_behavior);

[[nodiscard]] bool AUTDModulationStaticIsDefault(ModulationPtr s);

[[nodiscard]]
ModulationPtr AUTDModulationWithTransform(ModulationPtr m,
                                          const void* f,
                                          const void* context,
                                          LoopBehavior loop_behavior);

void AUTDGetErr(const void* src, char *dst);

} // extern "C"

} // namespace autd3::native_methods
