#pragma once

#include <filesystem>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation::audio_file {

class RawPCM final : public driver::Modulation<RawPCM> {
 public:
  AUTD3_API explicit RawPCM(std::filesystem::path path, const driver::Freq<uint32_t> sample_rate)
      : Modulation(driver::SamplingConfig::Division(5120)), _sample_rate(sample_rate), _path(std::move(path)) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry&) const override {
    return validate(AUTDModulationRawPCM(_path.string().c_str(), _sample_rate.hz(), _config, _loop_behavior));
  }

 private:
  driver::Freq<uint32_t> _sample_rate;
  std::filesystem::path _path;
};

}  // namespace autd3::modulation::audio_file
