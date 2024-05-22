

#pragma once

#include <algorithm>
#include <numeric>
#include <vector>

#include "autd3/driver/datagram/gain/base.hpp"
#include "autd3/driver/datagram/gain/cache.hpp"
#include "autd3/driver/firmware/fpga/drive.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain {

template <class F>
concept gain_transform_f = requires(F f, const driver::geometry::Device& dev, const driver::geometry::Transducer& tr, const driver::Drive d) {
  { f(dev)(tr, d) } -> std::same_as<driver::Drive>;
};

template <class G, gain_transform_f F>
class Transform final : public driver::GainBase,
                        public driver::IntoDatagramWithSegment<native_methods::GainPtr, Transform<G, F>>,
                        public driver::IntoGainCache<Transform<G, F>> {
  using transform_f = uint8_t (*)(const void*, native_methods::GeometryPtr, uint32_t, uint8_t, driver::Drive, driver::Drive*);

 public:
  AUTD3_API Transform(G g, F f) : _g(std::move(g)), _f(std::move(f)) {
    _f_native = +[](const void* context, native_methods::GeometryPtr geometry_ptr, const uint32_t dev_idx, const uint8_t tr_idx, driver::Drive drive,
                    driver::Drive* dst) -> uint8_t {
      const driver::geometry::Device dev(dev_idx, AUTDDevice(geometry_ptr, dev_idx));
      const driver::geometry::Transducer tr(tr_idx, dev.ptr());
      *dst = static_cast<const Transform*>(context)->_f(dev)(tr, drive);
    };
  }
  Transform() = delete;
  Transform(const Transform& obj) = default;             // LCOV_EXCL_LINE
  Transform& operator=(const Transform& obj) = default;  // LCOV_EXCL_LINE
  Transform(Transform&& obj) = default;                  // LCOV_EXCL_LINE
  Transform& operator=(Transform&& obj) = default;       // LCOV_EXCL_LINE
  ~Transform() override = default;                       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry& geometry) const override {
    return AUTDGainWithTransform(_g.gain_ptr(geometry), const_cast<void*>(reinterpret_cast<const void*>(_f_native)),
                                 const_cast<void*>(static_cast<const void*>(this)), geometry.ptr());
  }

 private:
  G _g;
  F _f;
  transform_f _f_native;
};
}  // namespace autd3::gain

namespace autd3::driver {
template <class G>
class IntoGainTransform {
 public:
  IntoGainTransform() = default;                                         // LCOV_EXCL_LINE
  IntoGainTransform(const IntoGainTransform& obj) = default;             // LCOV_EXCL_LINE
  IntoGainTransform& operator=(const IntoGainTransform& obj) = default;  // LCOV_EXCL_LINE
  IntoGainTransform(IntoGainTransform&& obj) = default;                  // LCOV_EXCL_LINE
  IntoGainTransform& operator=(IntoGainTransform&& obj) = default;       // LCOV_EXCL_LINE
  virtual ~IntoGainTransform() = default;                                // LCOV_EXCL_LINE

  template <gain::gain_transform_f F>
  AUTD3_API [[nodiscard]] gain::Transform<G, F> with_transform(F f) & {
    return gain::Transform(*static_cast<G*>(this), std::move(f));
  }
  template <gain::gain_transform_f F>
  AUTD3_API [[nodiscard]] gain::Transform<G, F> with_transform(F f) && {
    return gain::Transform(std::move(*static_cast<G*>(this)), std::move(f));
  }
};

}  // namespace autd3::driver
