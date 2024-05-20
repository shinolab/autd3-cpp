#pragma once

#include <concepts>

#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class F>
concept _phase_filter_f = requires(F f, const geometry::Device& d, const geometry::Transducer& tr) {
  { f(d)(tr) } -> std::same_as<Phase>;
};

class PhaseFilter final {
  using native_f = uint8_t (*)(const void*, native_methods::GeometryPtr, uint32_t, uint8_t);

  AUTD3_API explicit PhaseFilter(std::function<std::function<Phase(const geometry::Transducer&)>(const geometry::Device&)> f) noexcept
      : _f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint32_t dev_idx, const uint8_t tr_idx) -> uint8_t {
      const geometry::Device dev(dev_idx, AUTDDevice(geometry_ptr, dev_idx));
      const geometry::Transducer tr(tr_idx, dev.ptr());
      return static_cast<const PhaseFilter*>(context)->_f(dev)(tr).value();
    };
  }

 public:
  template <_phase_filter_f F>
  AUTD3_API [[nodiscard]] static PhaseFilter additive(F f) noexcept {
    return PhaseFilter(std::function<std::function<Phase(const geometry::Transducer&)>(const geometry::Device&)>(std::move(f)));
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDDatagramPhaseFilterAdditive(const_cast<void*>(reinterpret_cast<const void*>(_f_native)),
                                           const_cast<void*>(static_cast<const void*>(this)), geometry.ptr());
  }

 private:
  std::function<std::function<Phase(const geometry::Transducer&)>(const geometry::Device&)> _f;
  native_f _f_native;
};

}  // namespace autd3::driver
