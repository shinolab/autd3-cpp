#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

enum class Status : uint8_t {
  Error = 0,
  StateChanged = 1,
  Lost = 2,
};

enum class TimerStrategy : uint8_t {
  Sleep = 0,
  BusyWait = 1,
};

struct EthernetAdaptersPtr {
  const void *_0;
};

struct LinkSOEMBuilderPtr {
  const void *_0;
};

struct LinkRemoteSOEMBuilderPtr {
  const void *_0;
};

struct ResultLinkRemoteSOEMBuilder {
  LinkRemoteSOEMBuilderPtr result;
  uint32_t err_len;
  const void* err;
};

extern "C" {

void AUTDAUTDLinkSOEMTracingInit();

[[nodiscard]] EthernetAdaptersPtr AUTDAdapterPointer();

[[nodiscard]] uint32_t AUTDAdapterGetSize(EthernetAdaptersPtr adapters);

void AUTDAdapterGetAdapter(EthernetAdaptersPtr adapters, uint32_t idx, char *desc, char *name);

void AUTDAdapterPointerDelete(EthernetAdaptersPtr adapters);

[[nodiscard]] LinkSOEMBuilderPtr AUTDLinkSOEM();

[[nodiscard]] LinkSOEMBuilderPtr AUTDLinkSOEMWithSendCycle(LinkSOEMBuilderPtr soem, uint64_t cycle);

[[nodiscard]]
LinkSOEMBuilderPtr AUTDLinkSOEMWithSync0Cycle(LinkSOEMBuilderPtr soem,
                                              uint64_t cycle);

[[nodiscard]]
LinkSOEMBuilderPtr AUTDLinkSOEMWithBufSize(LinkSOEMBuilderPtr soem,
                                           uint32_t buf_size);

[[nodiscard]]
LinkSOEMBuilderPtr AUTDLinkSOEMWithTimerStrategy(LinkSOEMBuilderPtr soem,
                                                 TimerStrategy timer_strategy);

[[nodiscard]] LinkSOEMBuilderPtr AUTDLinkSOEMWithSyncMode(LinkSOEMBuilderPtr soem, SyncMode mode);

[[nodiscard]]
LinkSOEMBuilderPtr AUTDLinkSOEMWithSyncTolerance(LinkSOEMBuilderPtr soem,
                                                 uint64_t tolerance_ns);

[[nodiscard]]
LinkSOEMBuilderPtr AUTDLinkSOEMWithSyncTimeout(LinkSOEMBuilderPtr soem,
                                               uint64_t timeout_ns);

[[nodiscard]]
LinkSOEMBuilderPtr AUTDLinkSOEMWithIfname(LinkSOEMBuilderPtr soem,
                                          const char *ifname);

[[nodiscard]]
LinkSOEMBuilderPtr AUTDLinkSOEMWithStateCheckInterval(LinkSOEMBuilderPtr soem,
                                                      uint32_t interval_ms);

uint32_t AUTDLinkSOEMStatusGetMsg(Status src, char *dst);

[[nodiscard]]
LinkSOEMBuilderPtr AUTDLinkSOEMWithErrHandler(LinkSOEMBuilderPtr soem,
                                              const void* handler,
                                              const void* context);

[[nodiscard]]
LinkSOEMBuilderPtr AUTDLinkSOEMWithTimeout(LinkSOEMBuilderPtr soem,
                                           uint64_t timeout_ns);

[[nodiscard]] LinkBuilderPtr AUTDLinkSOEMIntoBuilder(LinkSOEMBuilderPtr soem);

[[nodiscard]] ResultLinkRemoteSOEMBuilder AUTDLinkRemoteSOEM(const char *addr);

[[nodiscard]]
LinkRemoteSOEMBuilderPtr AUTDLinkRemoteSOEMWithTimeout(LinkRemoteSOEMBuilderPtr soem,
                                                       uint64_t timeout_ns);

[[nodiscard]] LinkBuilderPtr AUTDLinkRemoteSOEMIntoBuilder(LinkRemoteSOEMBuilderPtr soem);

} // extern "C"

} // namespace autd3::native_methods
