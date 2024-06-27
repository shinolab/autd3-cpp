#pragma once

#include "autd3/driver/datagram/gain/gain.hpp"
#include "autd3/driver/firmware/fpga/drive.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain {

template <class F>
concept gain_transform = requires(F f, const driver::geometry::Device& dev, const driver::geometry::Transducer& tr) {
  { f(dev)(tr) } -> std::same_as<driver::Drive>;
};

using GainCalcResult = std::function<std::function<driver::Drive(const driver::geometry::Transducer&)>(const driver::geometry::Device&)>;

template <class G>
class Gain : public driver::Gain<G> {
  using native_f = void (*)(const void*, native_methods::GeometryPtr, uint16_t, uint8_t, driver::Drive*);

 public:
  Gain() = default;  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] virtual GainCalcResult calc(const driver::geometry::Geometry& geometry) const = 0;

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry& geometry) const override {
    _f = calc(geometry);
    _f_native =
        +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx, const uint8_t tr_idx, driver::Drive* dst) {
          const auto dev_ptr = AUTDDevice(geometry_ptr, dev_idx);
          const driver::geometry::Device dev(dev_idx, dev_ptr);
          const driver::geometry::Transducer tr(tr_idx, dev_ptr);
          *dst = static_cast<const Gain*>(context)->_f(dev)(tr);
        };
    return AUTDGainCustom(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr());
  }

  template <gain_transform Fn>
  AUTD3_API [[nodiscard]] static std::function<std::function<driver::Drive(const driver::geometry::Transducer&)>(const driver::geometry::Device&)>
  transform(Fn func) {
    return [func](const driver::geometry::Device& dev) {
      const auto f = func(dev);
      return [f, dev](const driver::geometry::Transducer& tr) { return f(tr); };
    };
  }

 private:
  mutable GainCalcResult _f = nullptr;
  mutable native_f _f_native = nullptr;
};

}  // namespace autd3::gain
