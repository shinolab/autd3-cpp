#pragma once

#include "autd3/gain/holo/backend.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/autd3capi-backend-cuda.h"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain::holo {

class CUDABackend final : public Backend {
 public:
  CUDABackend() { _ptr = validate(native_methods::AUTDCUDABackend()); }
  AUTD3_API ~CUDABackend() override {
    if (_ptr._0 != nullptr) {
      AUTDCUDABackendDelete(_ptr);
      _ptr._0 = nullptr;
    }
  }
  CUDABackend(const CUDABackend& v) noexcept = delete;
  CUDABackend& operator=(const CUDABackend& obj) = delete;
  CUDABackend(CUDABackend&& obj) = default;             // LCOV_EXCL_LINE
  CUDABackend& operator=(CUDABackend&& obj) = default;  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::GainPtr gs(const float* foci, const float* amps, const uint32_t size, const uint32_t repeat,
                                                     const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloCUDAGS(_ptr, reinterpret_cast<const native_methods::Vector3*>(foci), amps, size, repeat, constraint);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr gspat(const float* foci, const float* amps, const uint32_t size, const uint32_t repeat,
                                                        const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloCUDAGSPAT(_ptr, reinterpret_cast<const native_methods::Vector3*>(foci), amps, size, repeat, constraint);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr naive(const float* foci, const float* amps, const uint32_t size,
                                                        const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloCUDANaive(_ptr, reinterpret_cast<const native_methods::Vector3*>(foci), amps, size, constraint);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr lm(const float* foci, const float* amps, const uint32_t size, const float eps1, const float eps2,
                                                     const float tau, const uint32_t k_max, const float* initial, const uint32_t initial_size,
                                                     const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloCUDALM(_ptr, reinterpret_cast<const native_methods::Vector3*>(foci), amps, size, eps1, eps2, tau, k_max, constraint, initial,
                              initial_size);
  }
};

}  // namespace autd3::gain::holo
