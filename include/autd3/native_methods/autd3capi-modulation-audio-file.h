#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

extern "C" {

[[nodiscard]] ResultModulation AUTDModulationWav(const char *path, SamplingConfiguration config);

[[nodiscard]] SamplingConfiguration AUTDModulationWavDefaultSamplingConfig();

[[nodiscard]]
ResultModulation AUTDModulationRawPCM(const char *path,
                                      uint32_t sample_rate,
                                      SamplingConfiguration config);

[[nodiscard]] SamplingConfiguration AUTDModulationRawPCMDefaultSamplingConfig();

} // extern "C"

} // namespace autd3::native_methods
