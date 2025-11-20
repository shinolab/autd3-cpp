#include "autd3/link/nop.hpp"

#include "autd3.hpp"
#include "runner.hpp"
#include "util.hpp"

int main() try {
  auto autd = autd3::Controller::open({autd3::AUTD3{}, autd3::AUTD3{}}, autd3::link::Nop());
  run(autd);
  return 0;
} catch (std::exception& e) {
  print_err(e);
  return -1;
}
