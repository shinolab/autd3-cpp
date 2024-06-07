#include "autd3/link/simulator.hpp"

#include "autd3.hpp"
#include "runner.hpp"
#include "util.hpp"

// Run AUTD Simulator before running this example

int main() try {
  auto autd =
      autd3::ControllerBuilder(std::vector{autd3::AUTD3(autd3::Vector3::Zero()), autd3::AUTD3(autd3::Vector3(autd3::AUTD3::DEVICE_WIDTH, 0.0, 0.0))})
          .open(autd3::link::Simulator::builder(8080).with_timeout(std::chrono::milliseconds(200)));
  run(autd);
  return 0;
} catch (std::exception& e) {
  print_err(e);
  return -1;
}
