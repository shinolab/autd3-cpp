#pragma once

#include <concepts>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class F>
concept reads_fpga_info_f = requires(F f, const geometry::Device& d) {
  { f(d) } -> std::same_as<bool>;
};

template <reads_fpga_info_f F>
struct ReadsFPGAState final : Datagram, IntoDatagramTuple<ReadsFPGAState<F>> {
  AUTD3_API explicit ReadsFPGAState(F f) : f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx) -> bool {
      const geometry::Device dev(dev_idx, geometry_ptr);
      return static_cast<const ReadsFPGAState*>(context)->f(dev);
    };
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    return AUTDDatagramReadsFPGAState(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr());
  }

  F f;

 private:
  using native_f = bool (*)(const void*, native_methods::GeometryPtr, uint16_t);

  native_f _f_native;
};

}  // namespace autd3::driver
