#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

extern "C" {

[[nodiscard]] ResultModulation AUTDModulationAudioFileWav(const char *path);

[[nodiscard]]
ResultModulation AUTDModulationAudioFileCsv(const char *path,
                                            SamplingConfigWrap sampling_config,
                                            uint8_t delimiter);

}  // extern "C"

}  // namespace autd3::native_methods
