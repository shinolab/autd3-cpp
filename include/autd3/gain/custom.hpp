#pragma once

#include "autd3/driver/datagram/gain.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/firmware/fpga/drive.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain {

template <class F>
concept custom_test_f = requires(F f, const driver::geometry::Device& d, const driver::geometry::Transducer& tr) {
  { f(d)(tr) } -> std::same_as<driver::Drive>;
};

template <custom_test_f F>
struct Custom final : driver::Gain, driver::IntoDatagramTuple<Custom<F>> {
  AUTD3_API explicit Custom(F f) : f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx, const uint8_t tr_idx,
                    native_methods::Drive* raw) {
      const driver::geometry::Device dev(dev_idx, geometry_ptr);
      const driver::geometry::Transducer tr(tr_idx, dev_idx, dev.ptr());
      const auto d = static_cast<const Custom*>(context)->f(dev)(tr);
      raw->phase._0 = d.phase.value();
      raw->intensity._0 = d.intensity.value();
    };
  }

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry& geometry) const override {
    return AUTDGainCustom(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr());
  }

  F f;

 private:
  using native_f = void (*)(const void*, native_methods::GeometryPtr, uint16_t, uint8_t, native_methods::Drive*);

  native_f _f_native;
};

}  // namespace autd3::gain
