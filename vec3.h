//
// Created by Anchit Mishra on 2023-05-12.
//

#ifndef LUMINA_VECTOR_H
#define LUMINA_VECTOR_H

#include <lumina.h>

using std::sqrt;

class vec3 {
public:
    double x, y, z;
    vec3(): x(0), y(0), z(0) {}
    vec3(double xx, double yy, double zz): x(xx), y(yy), z(zz) {}
    bool near_zero()    {
        const auto epsilon = 1e-8;
        return (fabs(x) < epsilon) && (fabs(y) < epsilon) && (fabs(z) < epsilon);
    }
    vec3 operator-() const { return vec3(-x, -y, -z); }
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
    vec3& operator+=(const vec3 &v)   {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    vec3& operator-=(const vec3 &v)   {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    vec3& operator*=(const double t)   {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }
    vec3& operator/=(const double t)   {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }
    double length_squared() const  {
        return x*x + y*y + z*z;
    }
    double length() const {
        return sqrt(length_squared());
    }
    inline static vec3 random() {
        return {random_double(), random_double(), random_double()};
    }
    inline static vec3 random(double min, double max)   {
        return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }
};

// Define vec3 aliases for points and colors
using point3 = vec3; // XYZ
using color3 = vec3; // RGB

// Define utility functions for vectors
inline std::ostream& operator<<(std::ostream& os, const vec3 &v) {
    return os << v.x << " " << v.y << " " << v.z ;
}

inline vec3 operator+(const vec3 &u, const vec3& v)    {
    return vec3(u.x+v.x, u.y+v.y, u.z+v.z);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)    {
    return vec3(u.x-v.x, u.y-v.y, u.z-v.z);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)    {
    return vec3(u.x*v.x, u.y*v.y, u.z*v.z);
}

inline vec3 operator*(const double t, const vec3& v)  {
    return vec3(t*v.x, t*v.y, t*v.z);
}

inline vec3 operator*(const vec3 &v, const double t)    {
    return t * v;
}

inline vec3 operator/(const vec3 &v, const double t)  {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v)   {
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

inline vec3 cross(const vec3 &u, const vec3 &v)    {
    return vec3(
            u.y*v.z - u.z*v.y,
            u.z*v.x - u.x*v.z,
            u.x*v.y - u.y*v.x
            );
}

inline vec3 unit(const vec3 &v)  {
    return v / v.length();
}

inline vec3 random_in_unit_sphere()    {
    while (true)    {
        vec3 guess = vec3::random(-1, 1);
        if (guess.length_squared() <= 1)  {
            return guess;
        }
    }
}

vec3 random_unit_vector()   {
    return unit(random_in_unit_sphere());
}

vec3 reflect(const vec3& v, const vec3& n)  {
    return v - 2 * dot(v, n) * n;
}


#endif //LUMINA_VECTOR_H
