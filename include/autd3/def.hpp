#pragma once

#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(disable : 5054)
#pragma warning(disable : 26495)
#endif
#include <Eigen/Geometry>
#if _MSC_VER
#pragma warning(pop)
#endif

#include <numbers>

namespace autd3::driver {

constexpr float pi = std::numbers::pi_v<float>;

using Vector3 = Eigen::Matrix<float, 3, 1>;
using Vector4 = Eigen::Matrix<float, 4, 1>;
using Quaternion = Eigen::Quaternion<float>;
using Affine3 = Eigen::Transform<float, 3, Eigen::Affine>;

struct Point3 {
  Vector3 coords;

  Point3() : coords(Vector3::Zero()) {}
  explicit Point3(Vector3 coords) : coords(std::move(coords)) {}
  Point3(const float x, const float y, const float z) : coords(x, y, z) {}

  static Point3 origin() { return Point3{Vector3::Zero()}; }

  [[nodiscard]] float x() const { return coords.x(); }
  [[nodiscard]] float y() const { return coords.y(); }
  [[nodiscard]] float z() const { return coords.z(); }

  Point3 operator+() const { return *this; }
  Point3 operator-() const { return Point3{-coords}; }
  Point3 operator+(const Vector3& rhs) const { return Point3{coords + rhs}; }
  Point3 operator-(const Vector3& rhs) const { return Point3{coords - rhs}; }
  Vector3 operator-(const Point3& rhs) const { return coords - rhs.coords; }
  Point3 operator*(const float& rhs) const { return Point3{coords * rhs}; }
  friend Point3 operator*(const float& lhs, const Point3& rhs) { return rhs * lhs; }
  Point3 operator/(const float& rhs) const { return Point3{coords / rhs}; }

  Point3 operator+=(const Vector3& rhs) {
    coords += rhs;
    return *this;
  }

  Point3 operator-=(const Vector3& rhs) {
    coords -= rhs;
    return *this;
  }

  Point3 operator*=(const float& rhs) {
    coords *= rhs;
    return *this;
  }

  Point3 operator/=(const float& rhs) {
    coords /= rhs;
    return *this;
  }

  bool operator==(const Point3& rhs) const { return coords == rhs.coords; }
  bool operator!=(const Point3& rhs) const { return coords != rhs.coords; }

  float operator[](const Eigen::Index i) const { return coords[i]; }
  float& operator[](const Eigen::Index i) { return coords[i]; }
};

}  // namespace autd3::driver
