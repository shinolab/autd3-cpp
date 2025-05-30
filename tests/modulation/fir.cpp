
#include <gtest/gtest.h>

#include <autd3/modulation/fir.hpp>
#include <autd3/modulation/fourier.hpp>
#include <autd3/modulation/sine.hpp>
#include <ranges>

#include "utils.hpp"

TEST(DriverDatagramModulation, Fir) {
  using autd3::driver::Hz;
  auto autd = create_controller();

  const std::vector components{autd3::modulation::Sine(50 * Hz, autd3::modulation::SineOption()),
                               autd3::modulation::Sine(1000 * Hz, autd3::modulation::SineOption())};
  const auto m = autd3::modulation::Fourier(components, autd3::modulation::FourierOption());

  autd.send(autd3::modulation::Fir(m, std::vector{
                                          0.0f,
                                          2.3367325e-6f,
                                          8.982681e-6f,
                                          1.8887062e-5f,
                                          3.030097e-5f,
                                          4.075849e-5f,
                                          4.708182e-5f,
                                          4.542212e-5f,
                                          3.1348824e-5f,
                                          0.0f,
                                          -5.3695723e-5f,
                                          -0.00013471874f,
                                          -0.00024757805f,
                                          -0.00039585598f,
                                          -0.0005816907f,
                                          -0.0008052172f,
                                          -0.001063996f,
                                          -0.0013524637f,
                                          -0.0016614473f,
                                          -0.0019777846f,
                                          -0.0022840954f,
                                          -0.002558745f,
                                          -0.002776031f,
                                          -0.0029066242f,
                                          -0.0029182725f,
                                          -0.0027767674f,
                                          -0.0024471567f,
                                          -0.0018951697f,
                                          -0.0010888024f,
                                          0.0f,
                                          0.0013936388f,
                                          0.0031072246f,
                                          0.0051470925f,
                                          0.007509561f,
                                          0.010180013f,
                                          0.013132379f,
                                          0.016329063f,
                                          0.01972136f,
                                          0.023250382f,
                                          0.026848452f,
                                          0.030440966f,
                                          0.033948626f,
                                          0.037290003f,
                                          0.040384263f,
                                          0.043154005f,
                                          0.04552806f,
                                          0.04744411f,
                                          0.048851013f,
                                          0.049710777f,
                                          0.05f,
                                          0.049710777f,
                                          0.048851013f,
                                          0.04744411f,
                                          0.04552806f,
                                          0.043154005f,
                                          0.040384263f,
                                          0.037290003f,
                                          0.033948626f,
                                          0.030440966f,
                                          0.026848452f,
                                          0.023250382f,
                                          0.01972136f,
                                          0.016329063f,
                                          0.013132379f,
                                          0.010180013f,
                                          0.007509561f,
                                          0.0051470925f,
                                          0.0031072246f,
                                          0.0013936388f,
                                          0.0f,
                                          -0.0010888024f,
                                          -0.0018951697f,
                                          -0.0024471567f,
                                          -0.0027767674f,
                                          -0.0029182725f,
                                          -0.0029066242f,
                                          -0.002776031f,
                                          -0.002558745f,
                                          -0.0022840954f,
                                          -0.0019777846f,
                                          -0.0016614473f,
                                          -0.0013524637f,
                                          -0.001063996f,
                                          -0.0008052172f,
                                          -0.0005816907f,
                                          -0.00039585598f,
                                          -0.00024757805f,
                                          -0.00013471874f,
                                          -5.3695723e-5f,
                                          0.0f,
                                          3.1348824e-5f,
                                          4.542212e-5f,
                                          4.708182e-5f,
                                          4.075849e-5f,
                                          3.030097e-5f,
                                          1.8887062e-5f,
                                          8.982681e-6f,
                                          2.3367325e-6f,
                                          0.0f,
                                      }));

  for (auto& dev : autd) {
    auto mod = autd.link<autd3::link::Audit>().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{
        127, 131, 136, 140, 145, 149, 153, 157, 161, 164, 168, 171, 173, 176, 178, 180, 182, 183, 184, 184, 184, 184, 184, 183, 182, 180, 178,
        176, 173, 171, 168, 164, 161, 157, 153, 149, 145, 140, 136, 131, 127, 122, 118, 113, 109, 105, 100, 96,  93,  89,  86,  83,  80,  77,
        75,  73,  72,  70,  70,  69,  69,  69,  70,  70,  72,  73,  75,  77,  80,  83,  86,  89,  93,  96,  100, 105, 109, 113, 118, 122,
    };
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(10, autd.link<autd3::link::Audit>().modulation_freq_divide(dev.idx(), autd3::native_methods::Segment::S0));
  }
}
