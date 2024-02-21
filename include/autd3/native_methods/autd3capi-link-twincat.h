#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

struct LinkTwinCATBuilderPtr {
  void* _0;
};

struct LinkRemoteTwinCATBuilderPtr {
  void* _0;
};

struct ResultLinkRemoteTwinCATBuilder {
  LinkRemoteTwinCATBuilderPtr result;
  uint32_t err_len;
  void* err;
};

extern "C" {

[[nodiscard]] LinkTwinCATBuilderPtr AUTDLinkTwinCAT();

[[nodiscard]]
LinkTwinCATBuilderPtr AUTDLinkTwinCATWithTimeout(LinkTwinCATBuilderPtr twincat,
                                                 uint64_t timeout_ns);

[[nodiscard]] LinkBuilderPtr AUTDLinkTwinCATIntoBuilder(LinkTwinCATBuilderPtr twincat);

[[nodiscard]] ResultLinkRemoteTwinCATBuilder AUTDLinkRemoteTwinCAT(const char *server_ams_net_id);

[[nodiscard]]
LinkRemoteTwinCATBuilderPtr AUTDLinkRemoteTwinCATWithServerIP(LinkRemoteTwinCATBuilderPtr twincat,
                                                              const char *addr);

[[nodiscard]]
LinkRemoteTwinCATBuilderPtr AUTDLinkRemoteTwinCATWithClientAmsNetId(LinkRemoteTwinCATBuilderPtr twincat,
                                                                    const char *id);

[[nodiscard]]
LinkRemoteTwinCATBuilderPtr AUTDLinkRemoteTwinCATWithTimeout(LinkRemoteTwinCATBuilderPtr twincat,
                                                             uint64_t timeout_ns);

[[nodiscard]] LinkBuilderPtr AUTDLinkRemoteTwinCATIntoBuilder(LinkRemoteTwinCATBuilderPtr twincat);

} // extern "C"

} // namespace autd3::native_methods