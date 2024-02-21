#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

struct EmissionConstraintPtr {
  void* _0;
};

struct BackendPtr {
  void* _0;
};

struct ResultBackend {
  BackendPtr result;
  uint32_t err_len;
  void* err;
};

extern "C" {

[[nodiscard]] double AUTDGainHoloSPLToPascal(double value);

[[nodiscard]] double AUTDGainHoloPascalToSPL(double value);

[[nodiscard]] EmissionConstraintPtr AUTDGainHoloConstraintDotCare();

[[nodiscard]] EmissionConstraintPtr AUTDGainHoloConstraintNormalize();

[[nodiscard]] EmissionConstraintPtr AUTDGainHoloConstraintUniform(uint8_t intensity);

[[nodiscard]] EmissionConstraintPtr AUTDGainHoloConstraintClamp(uint8_t min_v, uint8_t max_v);

[[nodiscard]] bool AUTDGainHoloConstraintEq(EmissionConstraintPtr a, EmissionConstraintPtr b);

[[nodiscard]]
GainPtr AUTDGainHoloGreedy(const double *points,
                           const double *amps,
                           uint64_t size,
                           uint8_t div,
                           EmissionConstraintPtr constraint);

[[nodiscard]] bool AUTDGainGreedyIsDefault(GainPtr greedy);

[[nodiscard]]
GainPtr AUTDGainHoloGS(BackendPtr backend,
                       const double *points,
                       const double *amps,
                       uint64_t size,
                       uint32_t repeat,
                       EmissionConstraintPtr constraint);

[[nodiscard]] bool AUTDGainGSIsDefault(GainPtr gs);

[[nodiscard]]
GainPtr AUTDGainHoloGSPAT(BackendPtr backend,
                          const double *points,
                          const double *amps,
                          uint64_t size,
                          uint32_t repeat,
                          EmissionConstraintPtr constraint);

[[nodiscard]] bool AUTDGainGSPATIsDefault(GainPtr gs);

[[nodiscard]]
GainPtr AUTDGainHoloLM(BackendPtr backend,
                       const double *points,
                       const double *amps,
                       uint64_t size,
                       double eps_1,
                       double eps_2,
                       double tau,
                       uint32_t k_max,
                       const double *initial_ptr,
                       uint64_t initial_len,
                       EmissionConstraintPtr constraint);

[[nodiscard]] bool AUTDGainLMIsDefault(GainPtr gs);

[[nodiscard]]
GainPtr AUTDGainHoloNaive(BackendPtr backend,
                          const double *points,
                          const double *amps,
                          uint64_t size,
                          EmissionConstraintPtr constraint);

[[nodiscard]] bool AUTDGainNaiveIsDefault(GainPtr gs);

[[nodiscard]] BackendPtr AUTDNalgebraBackend();

void AUTDDeleteNalgebraBackend(BackendPtr backend);

[[nodiscard]]
GainPtr AUTDGainHoloSDP(BackendPtr backend,
                        const double *points,
                        const double *amps,
                        uint64_t size,
                        double alpha,
                        double lambda,
                        uint32_t repeat,
                        EmissionConstraintPtr constraint);

[[nodiscard]] bool AUTDGainSDPIsDefault(GainPtr gs);

} // extern "C"

} // namespace autd3::native_methods
