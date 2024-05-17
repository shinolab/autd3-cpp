#pragma once

#include <optional>
#include <vector>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

class PulseWidthEncoder final {
 public:
  AUTD3_API PulseWidthEncoder() : _buf(std::nullopt) {}
  AUTD3_API explicit PulseWidthEncoder(std::vector<uint16_t> buf) : _buf(std::move(buf)) {}

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
    return _buf.has_value() ? validate(native_methods::AUTDDatagramPulseWidthEncoder(_buf.value().data(), static_cast<uint32_t>(_buf.value().size())))
                            : native_methods::AUTDDatagramPulseWidthEncoderDefault();
  }

 private:
  std::optional<std::vector<uint16_t>> _buf;
};

}  // namespace autd3::driver
