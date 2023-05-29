//
// Created by Anchit Mishra on 2023-05-12.
//

#ifndef LUMINA_VECTOR_H
#define LUMINA_VECTOR_H

#include <Lumina.h>

using std::sqrt;

class Vector3 {
public:
    double x, y, z;
    Vector3(): x(0), y(0), z(0) {}
    Vector3(double xx, double yy, double zz): x(xx), y(yy), z(zz) {}
    Vector3 operator-() const { return Vector3(-x, -y, -z); }
    double operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
    }
    double& operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
    }
    Vector3& operator+=(const Vector3 &v)   {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vector3& operator-=(const Vector3 &v)   {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    Vector3& operator*=(const double t)   {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }
    Vector3& operator/=(const double t)   {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }
    double squaredLength() const  {
        return x*x + y*y + z*z;
    }
    double length() const {
        return sqrt(squaredLength());
    }
};

// Define Vector3 aliases for points and colors
using Point3 = Vector3; // XYZ
using Color3 = Vector3; // RGB

// Define utility functions for vectors
inline std::ostream& operator<<(std::ostream& os, const Vector3 &v) {
    return os << v.x << " " << v.y << " " << v.z ;
}

inline Vector3 operator+(const Vector3 &u, const Vector3& v)    {
    return Vector3(u.x+v.x, u.y+v.y, u.z+v.z);
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v)    {
    return Vector3(u.x-v.x, u.y-v.y, u.z-v.z);
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v)    {
    return Vector3(u.x*v.x, u.y*v.y, u.z*v.z);
}

inline Vector3 operator*(const double t, const Vector3& v)  {
    return Vector3(t*v.x, t*v.y, t*v.z);
}

inline Vector3 operator*(const Vector3 &v, const double t)    {
    return t * v;
}

inline Vector3 operator/(const Vector3 &v, const double t)  {
    return (1/t) * v;
}

inline double dot(const Vector3 &u, const Vector3 &v)   {
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

inline Vector3 cross(const Vector3 &u, const Vector3 &v)    {
    return Vector3(
            u.y*v.z - u.z*v.y,
            u.z*v.x - u.x*v.z,
            u.x*v.y - u.y*v.x
            );
}

inline Vector3 unit(const Vector3 &v)  {
    return v / v.length();
}

#endif //LUMINA_VECTOR_H
