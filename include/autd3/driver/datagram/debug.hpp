#pragma once

#include <concepts>
#include <variant>

#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct DebugType {
  static inline const native_methods::DebugTypeWrap None = native_methods::AUTDDebugTypeNone();
  static inline const native_methods::DebugTypeWrap BaseSignal = native_methods::AUTDDebugTypeBaseSignal();
  static inline const native_methods::DebugTypeWrap Thermo = native_methods::AUTDDebugTypeThermo();
  static inline const native_methods::DebugTypeWrap ForceFan = native_methods::AUTDDebugTypeForceFan();
  static inline const native_methods::DebugTypeWrap Sync = native_methods::AUTDDebugTypeSync();
  static inline const native_methods::DebugTypeWrap ModSegment = native_methods::AUTDDebugTypeModSegment();
  static inline const native_methods::DebugTypeWrap ModIdx(uint16_t idx) { return native_methods::AUTDDebugTypeModIdx(idx); }
  static inline const native_methods::DebugTypeWrap StmSegment = native_methods::AUTDDebugTypeStmSegment();
  static inline const native_methods::DebugTypeWrap StmIdx(uint16_t idx) { return native_methods::AUTDDebugTypeStmIdx(idx); }
  static inline const native_methods::DebugTypeWrap IsStmMode = native_methods::AUTDDebugTypeIsStmMode();
  static inline const native_methods::DebugTypeWrap PwmOut(const geometry::Transducer* tr) { return native_methods::AUTDDebugTypePwmOut(tr->ptr()); }
  static inline const native_methods::DebugTypeWrap Direct(bool v) { return native_methods::AUTDDebugTypeDirect(v); }
};

template <class F>
concept _debug_settings_f = requires(F f, const geometry::Device& d, const native_methods::GPIOOut gpio) {
  { f(d, gpio) } -> std::same_as<native_methods::DebugTypeWrap>;
};

template <_debug_settings_f F>
class DebugSettings final {
  using native_f = void (*)(const void*, native_methods::GeometryPtr, uint32_t, native_methods::GPIOOut, native_methods::DebugTypeWrap*);

 public:
  explicit DebugSettings(F f) : _f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint32_t dev_idx, const native_methods::GPIOOut gpio,
                    native_methods::DebugTypeWrap* res) {
      const geometry::Device dev(dev_idx, AUTDDevice(geometry_ptr, dev_idx));
      *res = static_cast<const DebugSettings*>(context)->_f(dev, gpio);
    };
  }

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return native_methods::AUTDDatagramDebugSettings(const_cast<void*>(reinterpret_cast<const void*>(_f_native)),
                                                     const_cast<void*>(static_cast<const void*>(this)), geometry.ptr());
  }

 private:
  F _f;
  native_f _f_native;
};

}  // namespace autd3::driver
