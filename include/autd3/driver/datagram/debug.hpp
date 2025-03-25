#pragma once

#include <concepts>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/ethercat/dc_sys_time.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct GPIOOutputType {
  AUTD3_API static const GPIOOutputType None;
  AUTD3_API static const GPIOOutputType BaseSignal;
  AUTD3_API static const GPIOOutputType Thermo;
  AUTD3_API static const GPIOOutputType ForceFan;
  AUTD3_API static const GPIOOutputType Sync;
  AUTD3_API static const GPIOOutputType ModSegment;
  AUTD3_API static GPIOOutputType ModIdx(const uint16_t idx) { return GPIOOutputType(native_methods::AUTDGPIOOutputTypeModIdx(idx)); }
  AUTD3_API static const GPIOOutputType StmSegment;
  AUTD3_API static GPIOOutputType StmIdx(const uint16_t idx) { return GPIOOutputType(native_methods::AUTDGPIOOutputTypeStmIdx(idx)); }
  AUTD3_API static const GPIOOutputType IsStmMode;
  AUTD3_API static GPIOOutputType SysTimeEq(const DcSysTime sys_time) {
    return GPIOOutputType(native_methods::AUTDGPIOOutputTypeSysTimeEq(sys_time));
  }
  AUTD3_API static GPIOOutputType PwmOut(const geometry::Transducer* tr) { return GPIOOutputType(AUTDGPIOOutputTypePwmOut(tr->ptr())); }
  AUTD3_API static GPIOOutputType Direct(const bool v) { return GPIOOutputType(native_methods::AUTDGPIOOutputTypeDirect(v)); }

  operator native_methods::GPIOOutputTypeWrap() const { return _inner; }

 private:
  explicit GPIOOutputType(const native_methods::GPIOOutputTypeWrap inner) : _inner(inner) {}

  native_methods::GPIOOutputTypeWrap _inner;
};

const GPIOOutputType GPIOOutputType::None = GPIOOutputType(native_methods::AUTDGPIOOutputTypeNone());
const GPIOOutputType GPIOOutputType::BaseSignal = GPIOOutputType(native_methods::AUTDGPIOOutputTypeBaseSignal());
const GPIOOutputType GPIOOutputType::Thermo = GPIOOutputType(native_methods::AUTDGPIOOutputTypeThermo());
const GPIOOutputType GPIOOutputType::ForceFan = GPIOOutputType(native_methods::AUTDGPIOOutputTypeForceFan());
const GPIOOutputType GPIOOutputType::Sync = GPIOOutputType(native_methods::AUTDGPIOOutputTypeSync());
const GPIOOutputType GPIOOutputType::ModSegment = GPIOOutputType(native_methods::AUTDGPIOOutputTypeModSegment());
const GPIOOutputType GPIOOutputType::StmSegment = GPIOOutputType(native_methods::AUTDGPIOOutputTypeStmSegment());
const GPIOOutputType GPIOOutputType::IsStmMode = GPIOOutputType(native_methods::AUTDGPIOOutputTypeIsStmMode());

template <class F>
concept debug_settings_f = requires(F f, const geometry::Device& d, const native_methods::GPIOOut gpio) {
  { f(d, gpio) } -> std::same_as<GPIOOutputType>;
};

template <debug_settings_f F>
struct GPIOOutputs final : Datagram, IntoDatagramTuple<GPIOOutputs<F>> {
  AUTD3_API explicit GPIOOutputs(F f) : f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx, const native_methods::GPIOOut gpio,
                    native_methods::GPIOOutputTypeWrap* res) {
      const geometry::Device dev(dev_idx, geometry_ptr);
      *res = static_cast<const GPIOOutputs*>(context)->f(dev, gpio);
    };
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    return AUTDDatagramGPIOOutputs(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr());
  }

  F f;

 private:
  using native_f = void (*)(const void*, native_methods::GeometryPtr, uint16_t, native_methods::GPIOOut, native_methods::GPIOOutputTypeWrap*);

  native_f _f_native;
};

}  // namespace autd3::driver
