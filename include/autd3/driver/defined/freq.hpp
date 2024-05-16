#pragma once

namespace autd3::driver {

template <class T>
class Freq {
 public:
  [[nodiscard]] T hz() const { return _value; }

  explicit Freq(const T value) : _value(value) {}

  auto operator<=>(const Freq<T>&) const = default;

 private:
  T _value;
};

template <>
class Freq<int> {
 public:
  operator Freq<uint32_t>() const { return Freq<uint32_t>(static_cast<uint32_t>(_value)); }

  explicit Freq(const int value) : _value(value) {}

  auto operator<=>(const Freq<int>&) const = default;

 private:
  int _value;
};

class UnitHz {
  template <class T>
  friend Freq<T> operator*(T l, const UnitHz&) {
    return Freq<T>(l);
  }
};

class UnitkHz {
  template <class T>
  Freq<T> friend operator*(T l, const UnitkHz&) {
    return Freq<T>(l * 1000);
  }
};

constexpr UnitHz Hz = UnitHz{};
constexpr UnitkHz kHz = UnitkHz{};

}  // namespace autd3::driver
