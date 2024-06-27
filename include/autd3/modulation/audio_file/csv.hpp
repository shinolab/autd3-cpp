#pragma once

#include <filesystem>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation::audio_file {

class Csv final : public driver::ModulationBase<Csv>,
                  public driver::IntoModulationCache<Csv>,
                  public driver::IntoRadiationPressure<Csv>,
                  public driver::IntoModulationTransform<Csv> {
 public:
  AUTD3_API explicit Csv(std::filesystem::path path, const driver::Freq<uint32_t> sample_rate)
      : _deliminator(','), _sample_rate(sample_rate), _path(std::move(path)) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return validate(AUTDModulationAudioFileCsv(_path.string().c_str(), _sample_rate.hz(), _deliminator, _loop_behavior));
  }

  AUTD3_DEF_PARAM(Csv, uint8_t, deliminator)

 private:
  driver::Freq<uint32_t> _sample_rate;
  std::filesystem::path _path;
};

}  // namespace autd3::modulation::audio_file
