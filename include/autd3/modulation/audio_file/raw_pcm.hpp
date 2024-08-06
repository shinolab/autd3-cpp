#pragma once

#include <filesystem>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation::audio_file {

class RawPCM final : public driver::ModulationBase<RawPCM>,
                     public driver::IntoModulationCache<RawPCM>,
                     public driver::IntoRadiationPressure<RawPCM>,
                     public driver::IntoModulationTransform<RawPCM> {
 public:
  template <typename T>
    requires std::convertible_to<T, driver::SamplingConfig>
  AUTD3_API explicit RawPCM(std::filesystem::path path, const T config) : _config(config), _path(std::move(path)) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return validate(AUTDModulationAudioFileRawPCM(_path.string().c_str(), _config, _loop_behavior));
  }

 private:
  driver::SamplingConfig _config;
  std::filesystem::path _path;
};

}  // namespace autd3::modulation::audio_file
