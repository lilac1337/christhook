#pragma once

#include <numbers>

template <class T>
class Vector3 {
public:
    T x{}, y{}, z{};

    Vector3(T x, T y, T z) : x{x}, y{y}, z{z} {}
    Vector3(const Vector3& a) : x{a.x}, y{a.y}, z{a.z} {}
    Vector3() noexcept = default;

    inline constexpr Vector3& operator-=(const Vector3& a) {
        x -= a.x;
        y -= a.y;
        z -= a.z;

        return *this;
    };

    inline constexpr Vector3& operator/=(T a) {
        x /= a;
        y /= a;
        z /= a;

        return *this;
    };

    inline constexpr Vector3& operator/=(const Vector3& a) {
        x /= a.x;
        y /= a.y;
        z /= a.z;

        return *this;
    };

    friend inline constexpr Vector3 operator-(const Vector3& a, const Vector3& b) {
        return Vector3(a) -= b;
    };

    friend inline constexpr Vector3 operator/(const Vector3& a, T b) {
        return Vector3(a) /= b;
    };

    [[nodiscard]] constexpr float twoDVel() const noexcept {
        return std::sqrt(this->x * this->x + this->y * this->y);
    }
};

template <class T>
class Vector2 {
public:
    T x{}, y{};

    Vector2(const Vector3<T>& in) : x{in.x}, y{in.y}{}

    Vector2() = default;

    [[nodiscard]] constexpr float twoDVel() const noexcept {
        return std::sqrt(this->x * this->x + this->y * this->y);
    }
};

namespace math {
    template <std::floating_point T>
    constexpr T rad2deg(T x) noexcept {
        constexpr auto radian = T{180} / std::numbers::pi_v<T>;
        return x * radian;
    }
};