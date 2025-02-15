#pragma once

#include <string>

#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::link {

struct TwinCAT final {
  TwinCAT() = default;  // LCOV_EXCL_LINE

  [[nodiscard]] native_methods::LinkPtr resolve() const { return validate(native_methods::AUTDLinkTwinCAT()); }
};

struct RemoteTwinCATOption {
  std::string server_ip;
  std::string client_ams_net_id;
};

struct RemoteTwinCAT final {
  explicit RemoteTwinCAT(std::string server_ams_net_id, RemoteTwinCATOption option)
      : server_ams_net_id(std::move(server_ams_net_id)), option(std::move(option)) {}

  std::string server_ams_net_id;
  RemoteTwinCATOption option;

  [[nodiscard]] native_methods::LinkPtr resolve() const {
    return validate(native_methods::AUTDLinkRemoteTwinCAT(server_ams_net_id.c_str(), option.server_ip.c_str(), option.client_ams_net_id.c_str()));
  }
};

}  // namespace autd3::link
