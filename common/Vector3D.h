#pragma once

class Vector3D {
    double x_, y_, z_;

public:
    Vector3D(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    Vector3D(const Vector3D &other) = default;

    inline double x() const {
        return x_;
    }

    inline double y() const {
        return y_;
    }

    inline double z() const {
        return z_;
    }

    inline void operator+=(const Vector3D &other) {
        x_ += other.x_;
        y_ += other.y_;
        z_ += other.z_;
    }

    inline Vector3D operator+(const Vector3D &other) const {
        Vector3D result{*this};
        result += other;
        return result;
    }

    inline Vector3D operator-() const {
        Vector3D result{*this};
        result.x_ = -result.x_;
        result.y_ = -result.y_;
        result.z_ = -result.z_;
        return result;
    }

    inline void operator-=(const Vector3D &other) {
        *this += -other;
    }

    inline Vector3D operator-(const Vector3D &other) const {
        Vector3D result{*this};
        result -= other;
        return result;
    }

    inline double operator*(const Vector3D &other) const {
        return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
    }

    inline void operator/=(double other) {
        this->x_ /= other;
        this->y_ /= other;
        this->z_ /= other;
    }

    double length() const {
        return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
    }
};

inline Vector3D operator*(double scalar, const Vector3D& vector) {
    return {vector.x() * scalar, vector.y() * scalar, vector.z() * scalar};
}

inline Vector3D operator*(const Vector3D& vector, double scalar) {
    return scalar * vector;
}
