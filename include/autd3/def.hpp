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
    Point3(const Vector3& coords) : coords(coords) {}
    Point3(const float x, const float y, const float z) : coords(x, y, z) {}

    inline static Point3 origin() { return {Vector3::Zero()}; }

    inline float x() const { return coords.x(); }
    inline float y() const { return coords.y(); }
    inline float z() const { return coords.z(); }

    inline Point3 operator+() const { return *this; }
    inline Point3 operator-() const { return {-coords}; }
    inline Point3 operator+(const Vector3& rhs) const { return {coords + rhs}; }
    inline Point3 operator-(const Vector3& rhs) const { return {coords - rhs}; }
    inline Vector3 operator-(const Point3& rhs) const { return coords - rhs.coords; }
    inline Point3 operator*(const float& rhs) const { return {coords * rhs}; }
    friend inline Point3 operator*(const float& lhs, const Point3& rhs) { return rhs * lhs; }
    inline Point3 operator/(const float& rhs) const { return {coords / rhs}; }
    inline Point3 operator+=(const Vector3& rhs) {
        coords += rhs;
        return *this;
    }
    inline Point3 operator-=(const Vector3& rhs) {
        coords -= rhs;
        return *this;
    }
    inline Point3 operator*=(const float& rhs) {
        coords *= rhs;
        return *this;
    }
    inline Point3 operator/=(const float& rhs) {
        coords /= rhs;
        return *this;
    }
    inline bool operator==(const Point3& rhs) const { return coords == rhs.coords; }
    inline bool operator!=(const Point3& rhs) const { return coords != rhs.coords; }

    inline float operator[](const size_t i) const { return coords[i]; }
    inline float& operator[](const size_t i) { return coords[i]; }

};


}  // namespace autd3::driver
