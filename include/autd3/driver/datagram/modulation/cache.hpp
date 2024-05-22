#pragma once

#include <memory>

#include "autd3/driver/datagram/modulation/base.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation {

template <class M>
class Cache final : public driver::ModulationBase<Cache<M>> {
 public:
  AUTD3_API explicit Cache(M m) : _m(std::move(m)), _cache(std::make_shared<std::vector<driver::EmitIntensity>>()) {
    this->_loop_behavior = _m.loop_behavior();
  }
  Cache(const Cache& v) = default;  // LCOV_EXCL_LINE
  Cache& operator=(const Cache& obj) = delete;
  Cache(Cache&& obj) noexcept = default;  // LCOV_EXCL_LINE
  Cache& operator=(Cache&& obj) noexcept = delete;
  ~Cache() noexcept override = default;  // LCOV_EXCL_LINE

  AUTD3_API const std::vector<driver::EmitIntensity>& calc(const driver::geometry::Geometry& geometry) const { return init(geometry); }

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry& geometry) const override {
    const auto& buf = calc(geometry);
    return native_methods::AUTDModulationRaw(_sampling_config.value(), this->_loop_behavior, reinterpret_cast<const uint8_t*>(buf.data()),
                                             static_cast<uint32_t>(buf.size()));
  }

  AUTD3_API [[nodiscard]] const std::vector<driver::EmitIntensity>& buffer() const { return *_cache; }

  AUTD3_API [[nodiscard]] std::vector<driver::EmitIntensity>::const_iterator cbegin() const noexcept { return _cache->cbegin(); }
  AUTD3_API [[nodiscard]] std::vector<driver::EmitIntensity>::const_iterator cend() const noexcept { return _cache->cend(); }
  AUTD3_API [[nodiscard]] std::vector<driver::EmitIntensity>::const_iterator begin() const noexcept { return _cache->begin(); }
  AUTD3_API [[nodiscard]] std::vector<driver::EmitIntensity>::const_iterator end() const noexcept { return _cache->end(); }
  AUTD3_API [[nodiscard]] const driver::EmitIntensity& operator[](const size_t i) const { return _cache->at(i); }

 private:
  AUTD3_API const std::vector<driver::EmitIntensity>& init(const driver::geometry::Geometry& geometry) const {
    if (_cache->size() == 0) {
      const auto res = native_methods::AUTDModulationCalc(_m.modulation_ptr(geometry), geometry.ptr());
      const auto ptr = validate(res);
      _cache->resize(native_methods::AUTDModulationCalcGetSize(ptr), driver::EmitIntensity(0));
      _sampling_config = res.config;
      native_methods::AUTDModulationCalcGetResult(ptr, reinterpret_cast<uint8_t*>(_cache->data()));
      native_methods::AUTDModulationCalcFreeResult(ptr);
    }
    return *_cache;
  }

  M _m;
  mutable std::shared_ptr<std::vector<driver::EmitIntensity>> _cache;
  mutable std::optional<native_methods::SamplingConfigWrap> _sampling_config;
};

}  // namespace autd3::modulation

namespace autd3::driver {

template <class M>
class IntoModulationCache {
 public:
  AUTD3_API [[nodiscard]] modulation::Cache<M> with_cache() & { return modulation::Cache(*static_cast<M*>(this)); }
  AUTD3_API [[nodiscard]] modulation::Cache<M> with_cache() && { return modulation::Cache(std::move(*static_cast<M*>(this))); }
};

}  // namespace autd3::driver
