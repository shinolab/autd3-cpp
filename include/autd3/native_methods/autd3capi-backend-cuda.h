#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

extern "C" {

[[nodiscard]] ResultBackend AUTDCUDABackend();

void AUTDCUDABackendDelete(BackendPtr backend);

[[nodiscard]]
GainPtr AUTDGainHoloCUDASDP(BackendPtr backend,
                            const double *points,
                            const double *amps,
                            uint64_t size,
                            double alpha,
                            double lambda,
                            uint32_t repeat,
                            EmissionConstraintWrap constraint);

[[nodiscard]]
GainPtr AUTDGainHoloCUDAGS(BackendPtr backend,
                           const double *points,
                           const double *amps,
                           uint64_t size,
                           uint32_t repeat,
                           EmissionConstraintWrap constraint);

[[nodiscard]]
GainPtr AUTDGainHoloCUDAGSPAT(BackendPtr backend,
                              const double *points,
                              const double *amps,
                              uint64_t size,
                              uint32_t repeat,
                              EmissionConstraintWrap constraint);

[[nodiscard]]
GainPtr AUTDGainHoloCUDANaive(BackendPtr backend,
                              const double *points,
                              const double *amps,
                              uint64_t size,
                              EmissionConstraintWrap constraint);

[[nodiscard]]
GainPtr AUTDGainHoloCUDALM(BackendPtr backend,
                           const double *points,
                           const double *amps,
                           uint64_t size,
                           double eps_1,
                           double eps_2,
                           double tau,
                           uint32_t k_max,
                           EmissionConstraintWrap constraint,
                           const double *initial_ptr,
                           uint64_t initial_len);

} // extern "C"

} // namespace autd3::native_methods
