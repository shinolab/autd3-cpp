#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

enum class EmissionConstraintTag : uint8_t {
  Normalize = 1,
  Uniform = 2,
  Multiply = 3,
  Clamp = 4,
};

union EmissionConstraintValue {
  uint8_t null;
  uint8_t uniform;
  float multiply;
  uint8_t clamp[2];
};

struct EmissionConstraintWrap {
  EmissionConstraintTag tag;
  EmissionConstraintValue value;
};

struct BackendPtr {
  const void *_0;
};

extern "C" {

[[nodiscard]] float AUTDGainHoloSPLToPascal(float value);

[[nodiscard]] float AUTDGainHoloPascalToSPL(float value);

[[nodiscard]] EmissionConstraintWrap AUTDGainHoloConstraintNormalize();

[[nodiscard]] EmissionConstraintWrap AUTDGainHoloConstraintUniform(uint8_t intensity);

[[nodiscard]] EmissionConstraintWrap AUTDGainHoloConstraintMultiply(float v);

[[nodiscard]] EmissionConstraintWrap AUTDGainHoloConstraintClamp(uint8_t min_v, uint8_t max_v);

[[nodiscard]]
GainPtr AUTDGainHoloGreedySphere(const Point3 *points,
                                 const float *amps,
                                 uint32_t size,
                                 uint8_t div,
                                 EmissionConstraintWrap constraint);

[[nodiscard]]
GainPtr AUTDGainHoloGreedyT4010A1(const Point3 *points,
                                  const float *amps,
                                  uint32_t size,
                                  uint8_t div,
                                  EmissionConstraintWrap constraint);

[[nodiscard]] bool AUTDGainGreedyIsDefault(EmissionConstraintWrap constraint, uint8_t phase_div);

[[nodiscard]]
GainPtr AUTDGainHoloGSSphere(BackendPtr backend,
                             const Point3 *points,
                             const float *amps,
                             uint32_t size,
                             uint32_t repeat_nonzero,
                             EmissionConstraintWrap constraint);

[[nodiscard]]
GainPtr AUTDGainHoloGST4010A1(BackendPtr backend,
                              const Point3 *points,
                              const float *amps,
                              uint32_t size,
                              uint32_t repeat_nonzero,
                              EmissionConstraintWrap constraint);

[[nodiscard]] bool AUTDGainGSIsDefault(EmissionConstraintWrap constraint, uint32_t repeat);

[[nodiscard]]
GainPtr AUTDGainHoloGSPATSphere(BackendPtr backend,
                                const Point3 *points,
                                const float *amps,
                                uint32_t size,
                                uint32_t repeat_nonzero,
                                EmissionConstraintWrap constraint);

[[nodiscard]]
GainPtr AUTDGainHoloGSPATT4010A1(BackendPtr backend,
                                 const Point3 *points,
                                 const float *amps,
                                 uint32_t size,
                                 uint32_t repeat_nonzero,
                                 EmissionConstraintWrap constraint);

[[nodiscard]] bool AUTDGainGSPATIsDefault(EmissionConstraintWrap constraint, uint32_t repeat);

[[nodiscard]]
GainPtr AUTDGainHoloLMSphere(BackendPtr backend,
                             const Point3 *points,
                             const float *amps,
                             uint32_t size,
                             float eps_1,
                             float eps_2,
                             float tau,
                             uint32_t k_max_nonzero,
                             const float *initial_ptr,
                             uint32_t initial_len,
                             EmissionConstraintWrap constraint);

[[nodiscard]]
GainPtr AUTDGainHoloLMT4010A1(BackendPtr backend,
                              const Point3 *points,
                              const float *amps,
                              uint32_t size,
                              float eps_1,
                              float eps_2,
                              float tau,
                              uint32_t k_max_nonzero,
                              const float *initial_ptr,
                              uint32_t initial_len,
                              EmissionConstraintWrap constraint);

[[nodiscard]]
bool AUTDGainLMIsDefault(EmissionConstraintWrap constraint,
                         float eps_1,
                         float eps_2,
                         float tau,
                         uint32_t k_max,
                         const float *initial_ptr,
                         uint32_t initial_len);

[[nodiscard]]
GainPtr AUTDGainHoloNaiveSphere(BackendPtr backend,
                                const Point3 *points,
                                const float *amps,
                                uint32_t size,
                                EmissionConstraintWrap constraint);

[[nodiscard]]
GainPtr AUTDGainHoloNaiveT4010A1(BackendPtr backend,
                                 const Point3 *points,
                                 const float *amps,
                                 uint32_t size,
                                 EmissionConstraintWrap constraint);

[[nodiscard]] bool AUTDGainNaiveIsDefault(EmissionConstraintWrap constraint);

[[nodiscard]] BackendPtr AUTDNalgebraBackendSphere();

[[nodiscard]] BackendPtr AUTDNalgebraBackendT4010A1();

void AUTDDeleteNalgebraBackendSphere(BackendPtr backend);

void AUTDDeleteNalgebraBackendT4010A1(BackendPtr backend);

} // extern "C"

} // namespace autd3::native_methods
