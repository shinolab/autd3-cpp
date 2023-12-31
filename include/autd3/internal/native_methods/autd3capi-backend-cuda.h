#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::internal::native_methods {

extern "C" {

[[nodiscard]] ResultBackend AUTDCUDABackend();

void AUTDCUDABackendDelete(BackendPtr backend);

[[nodiscard]]
GainPtr AUTDGainHoloCUDASDP(BackendPtr backend,
                            const double *points,
                            const double *amps,
                            uint64_t size);

[[nodiscard]]
GainPtr AUTDGainHoloCUDASDPWithConstraint(GainPtr holo,
                                          EmissionConstraintPtr constraint);

[[nodiscard]] GainPtr AUTDGainHoloCUDASDPWithAlpha(GainPtr holo, double alpha);

[[nodiscard]] GainPtr AUTDGainHoloCUDASDPWithLambda(GainPtr holo, double lambda);

[[nodiscard]] GainPtr AUTDGainHoloCUDASDPWithRepeat(GainPtr holo, uint32_t repeat);

[[nodiscard]]
GainPtr AUTDGainHoloCUDAGS(BackendPtr backend,
                           const double *points,
                           const double *amps,
                           uint64_t size);

[[nodiscard]]
GainPtr AUTDGainHoloCUDAGSWithConstraint(GainPtr holo,
                                         EmissionConstraintPtr constraint);

[[nodiscard]] GainPtr AUTDGainHoloCUDAGSWithRepeat(GainPtr holo, uint32_t repeat);

[[nodiscard]]
GainPtr AUTDGainHoloCUDAGSPAT(BackendPtr backend,
                              const double *points,
                              const double *amps,
                              uint64_t size);

[[nodiscard]]
GainPtr AUTDGainHoloCUDAGSPATWithConstraint(GainPtr holo,
                                            EmissionConstraintPtr constraint);

[[nodiscard]] GainPtr AUTDGainHoloCUDAGSPATWithRepeat(GainPtr holo, uint32_t repeat);

[[nodiscard]]
GainPtr AUTDGainHoloCUDANaive(BackendPtr backend,
                              const double *points,
                              const double *amps,
                              uint64_t size);

[[nodiscard]]
GainPtr AUTDGainHoloCUDANaiveWithConstraint(GainPtr holo,
                                            EmissionConstraintPtr constraint);

[[nodiscard]]
GainPtr AUTDGainHoloCUDAGreedy(const double *points,
                               const double *amps,
                               uint64_t size);

[[nodiscard]]
GainPtr AUTDGainHoloCUDAGreedyWithConstraint(GainPtr holo,
                                             EmissionConstraintPtr constraint);

[[nodiscard]] GainPtr AUTDGainHoloCUDAGreedyWithPhaseDiv(GainPtr holo, uint32_t div);

[[nodiscard]]
GainPtr AUTDGainHoloCUDALM(BackendPtr backend,
                           const double *points,
                           const double *amps,
                           uint64_t size);

[[nodiscard]]
GainPtr AUTDGainHoloCUDALMWithConstraint(GainPtr holo,
                                         EmissionConstraintPtr constraint);

[[nodiscard]] GainPtr AUTDGainHoloCUDALMWithEps1(GainPtr holo, double eps);

[[nodiscard]] GainPtr AUTDGainHoloCUDALMWithEps2(GainPtr holo, double eps);

[[nodiscard]] GainPtr AUTDGainHoloCUDALMWithTau(GainPtr holo, double tau);

[[nodiscard]] GainPtr AUTDGainHoloCUDALMWithKMax(GainPtr holo, uint32_t k_max);

[[nodiscard]]
GainPtr AUTDGainHoloCUDALMWithInitial(GainPtr holo,
                                      const double *initial_ptr,
                                      uint64_t len);

} // extern "C"

} // namespace autd3::internal::native_methods
