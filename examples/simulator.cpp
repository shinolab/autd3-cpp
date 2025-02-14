#include "autd3/link/simulator.hpp"

#include "autd3.hpp"
#include "runner.hpp"
#include "util.hpp"

// Run AUTD Simulator before running this example

int main() try {
  auto autd = autd3::Controller::open(
      std::vector{autd3::AUTD3{.pos = autd3::Point3::origin(), .rot = autd3::Quaternion::Identity()},
                  autd3::AUTD3{.pos = autd3::Point3(autd3::AUTD3::DEVICE_WIDTH, 0.0, 0.0), .rot = autd3::Quaternion::Identity()}},
      autd3::link::Simulator{"127.0.0.1:8080"});
  run(autd);
  return 0;
} catch (std::exception& e) {
  print_err(e);
  return -1;
}
