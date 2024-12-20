#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

extern "C" {

void AUTDModulationAudioFileTracingInit();

ResultStatus AUTDModulationAudioFileTracingInitWithFile(const char *path);

[[nodiscard]]
ResultModulation AUTDModulationAudioFileWav(const char *path,
                                            LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationAudioFileWavWithResample(const char *path,
                                                        LoopBehavior loop_behavior,
                                                        SamplingConfig target,
                                                        DynSincInterpolator resample);

[[nodiscard]]
ResultModulation AUTDModulationAudioFileRawPCM(const char *path,
                                               SamplingConfig config,
                                               LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationAudioFileRawPCMWithResample(const char *path,
                                                           LoopBehavior loop_behavior,
                                                           float src,
                                                           SamplingConfig target,
                                                           DynSincInterpolator resample);

[[nodiscard]]
ResultModulation AUTDModulationAudioFileCsv(const char *path,
                                            SamplingConfig config,
                                            uint8_t deliminator,
                                            LoopBehavior loop_behavior);

[[nodiscard]]
ResultModulation AUTDModulationAudioFileCsvWithResample(const char *path,
                                                        uint8_t deliminator,
                                                        LoopBehavior loop_behavior,
                                                        float src,
                                                        SamplingConfig target,
                                                        DynSincInterpolator resample);

} // extern "C"

} // namespace autd3::native_methods
