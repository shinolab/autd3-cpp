#pragma once

#include "autd3/gain/holo/backend.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

class NalgebraBackend final : public Backend {
 public:
  NalgebraBackend() : Backend(native_methods::AUTDNalgebraBackendSphere()) {}
  AUTD3_API ~NalgebraBackend() override {
    if (_ptr._0 != nullptr) {
      AUTDDeleteNalgebraBackendSphere(_ptr);
      _ptr._0 = nullptr;
    }
  }
  NalgebraBackend(const NalgebraBackend& v) noexcept = delete;
  NalgebraBackend& operator=(const NalgebraBackend& obj) = delete;
  NalgebraBackend(NalgebraBackend&& obj) = default;             // LCOV_EXCL_LINE
  NalgebraBackend& operator=(NalgebraBackend&& obj) = default;  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::GainPtr sdp(const double* foci, const double* amps, const uint64_t size, const double alpha,
                                                      const double lambda, const uint32_t repeat,
                                                      const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloSDPSphere(_ptr, foci, amps, size, alpha, lambda, repeat, constraint);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr gs(const double* foci, const double* amps, const uint64_t size, const uint32_t repeat,
                                                     const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloGSSphere(_ptr, foci, amps, size, repeat, constraint);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr gspat(const double* foci, const double* amps, const uint64_t size, const uint32_t repeat,
                                                        const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloGSPATSphere(_ptr, foci, amps, size, repeat, constraint);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr naive(const double* foci, const double* amps, const uint64_t size,
                                                        const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloNaiveSphere(_ptr, foci, amps, size, constraint);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr lm(const double* foci, const double* amps, const uint64_t size, const double eps1,
                                                     const double eps2, const double tau, const uint32_t k_max, const double* initial,
                                                     const uint64_t initial_size,
                                                     const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloLMSphere(_ptr, foci, amps, size, eps1, eps2, tau, k_max, initial, initial_size, constraint);
  }
};
}  // namespace autd3::gain::holo
