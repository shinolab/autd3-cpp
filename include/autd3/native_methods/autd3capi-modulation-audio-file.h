#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

extern "C" {

[[nodiscard]]
ResultModulation AUTDModulationWav(const char *path,
                                   SamplingConfigWrap config,
                                   LoopBehavior loop_behavior);

[[nodiscard]] bool AUTDModulationWavIsDefault(ModulationPtr wav);

[[nodiscard]]
ResultModulation AUTDModulationRawPCM(const char *path,
                                      uint32_t sample_rate,
                                      LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationCsv(const char *path,
                                   uint32_t sample_rate,
                                   uint8_t deliminator,
                                   LoopBehavior loop_behavior);

} // extern "C"

} // namespace autd3::native_methods
