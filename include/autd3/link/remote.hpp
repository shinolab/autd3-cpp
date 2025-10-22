#pragma once

#include <optional>
#include <string>

#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::link {

struct RemoteOption {
  std::optional<std::chrono::nanoseconds> timeout = std::nullopt;

  operator native_methods::RemoteOption() const {
    return native_methods::RemoteOption{
        .timeout = native_methods::to_option_duration(timeout),
    };
  }
};

struct Remote final {
  std::string ip;
  RemoteOption option;

  [[nodiscard]] native_methods::LinkPtr resolve() const {
    return validate(native_methods::AUTDLinkRemote(ip.c_str(), static_cast<native_methods::RemoteOption>(option)));
  }
};

}  // namespace autd3::link
