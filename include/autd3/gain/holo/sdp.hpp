#pragma once

#include <memory>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/backend.hpp"
#include "autd3/gain/holo/holo.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain::holo {

template <backend B>
class SDP final : public Holo<SDP<B>> {
 public:
  template <holo_foci_range R>
  AUTD3_API explicit SDP(std::shared_ptr<B> holo_backend, R&& iter)
      : Holo<SDP>(EmissionConstraint::DontCare, std::forward<R>(iter)),
        _alpha(1e-3f),
        _repeat(100),
        _lambda(0.9f),
        _backend(std::move(holo_backend)) {}

  AUTD3_DEF_PARAM(SDP, float, alpha)
  AUTD3_DEF_PARAM(SDP, uint32_t, repeat)
  AUTD3_DEF_PARAM(SDP, float, lambda)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return this->_backend->sdp(reinterpret_cast<const float*>(this->_foci.data()), reinterpret_cast<const float*>(this->_amps.data()),
                               static_cast<uint32_t>(this->_amps.size()), _alpha, _lambda, _repeat, this->_constraint);
  }

 private:
  std::shared_ptr<B> _backend;
};

}  // namespace autd3::gain::holo
