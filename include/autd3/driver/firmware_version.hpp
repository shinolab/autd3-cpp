#pragma once

#include <string>

#include "autd3/native_methods.hpp"

namespace autd3::controller {
class Controller;
}

namespace autd3::driver {

class FirmwareVersion {
 public:
  friend class controller::Controller;

  AUTD3_API [[nodiscard]] std::string info() const { return _info; }

  AUTD3_API [[nodiscard]] static std::string latest_version() {
    char info[256];
    native_methods::AUTDFirmwareLatest(info);
    return {info};
  }

 private:
  AUTD3_API explicit FirmwareVersion(std::string info) noexcept : _info(std::move(info)) {}

  std::string _info;
};

inline std::ostream& operator<<(std::ostream& os, const FirmwareVersion& obj) {
  os << obj.info();
  return os;
}

}  // namespace autd3::driver
