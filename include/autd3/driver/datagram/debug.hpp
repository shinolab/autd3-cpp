#pragma once

#include <concepts>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/ethercat/dc_sys_time.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct DebugType {
  AUTD3_API static const DebugType None;
  AUTD3_API static const DebugType BaseSignal;
  AUTD3_API static const DebugType Thermo;
  AUTD3_API static const DebugType ForceFan;
  AUTD3_API static const DebugType Sync;
  AUTD3_API static const DebugType ModSegment;
  AUTD3_API static DebugType ModIdx(const uint16_t idx) { return DebugType(native_methods::AUTDDebugTypeModIdx(idx)); }
  AUTD3_API static const DebugType StmSegment;
  AUTD3_API static DebugType StmIdx(const uint16_t idx) { return DebugType(native_methods::AUTDDebugTypeStmIdx(idx)); }
  AUTD3_API static const DebugType IsStmMode;
  AUTD3_API static DebugType SysTimeEq(const DcSysTime sys_time) { return DebugType(native_methods::AUTDDebugTypeSysTimeEq(sys_time)); }
  AUTD3_API static DebugType PwmOut(const geometry::Transducer* tr) { return DebugType(AUTDDebugTypePwmOut(tr->ptr())); }
  AUTD3_API static DebugType Direct(const bool v) { return DebugType(native_methods::AUTDDebugTypeDirect(v)); }

  operator native_methods::DebugTypeWrap() const { return _inner; }

 private:
  explicit DebugType(const native_methods::DebugTypeWrap inner) : _inner(inner) {}

  native_methods::DebugTypeWrap _inner;
};

const DebugType DebugType::None = DebugType(native_methods::AUTDDebugTypeNone());
const DebugType DebugType::BaseSignal = DebugType(native_methods::AUTDDebugTypeBaseSignal());
const DebugType DebugType::Thermo = DebugType(native_methods::AUTDDebugTypeThermo());
const DebugType DebugType::ForceFan = DebugType(native_methods::AUTDDebugTypeForceFan());
const DebugType DebugType::Sync = DebugType(native_methods::AUTDDebugTypeSync());
const DebugType DebugType::ModSegment = DebugType(native_methods::AUTDDebugTypeModSegment());
const DebugType DebugType::StmSegment = DebugType(native_methods::AUTDDebugTypeStmSegment());
const DebugType DebugType::IsStmMode = DebugType(native_methods::AUTDDebugTypeIsStmMode());

template <class F>
concept debug_settings_f = requires(F f, const geometry::Device& d, const native_methods::GPIOOut gpio) {
  { f(d, gpio) } -> std::same_as<DebugType>;
};

template <debug_settings_f F>
struct DebugSettings final : Datagram, IntoDatagramTuple<DebugSettings<F>> {
  AUTD3_API explicit DebugSettings(F f) : f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx, const native_methods::GPIOOut gpio,
                    native_methods::DebugTypeWrap* res) {
      const geometry::Device dev(dev_idx, geometry_ptr);
      *res = static_cast<const DebugSettings*>(context)->f(dev, gpio);
    };
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const override {
    return AUTDDatagramDebugSettings(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr());
  }

  F f;

 private:
  using native_f = void (*)(const void*, native_methods::GeometryPtr, uint16_t, native_methods::GPIOOut, native_methods::DebugTypeWrap*);

  native_f _f_native;
};

}  // namespace autd3::driver
