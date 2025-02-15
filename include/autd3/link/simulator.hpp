#pragma once

#include <string>

#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::link {

struct Simulator final {
  std::string ip;

  [[nodiscard]] native_methods::LinkPtr resolve() const { return validate(native_methods::AUTDLinkSimulator(ip.c_str())); }
};

}  // namespace autd3::link
