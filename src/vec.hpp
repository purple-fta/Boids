#ifndef VEC_H
#define VEC_H
#include <cmath>

struct Vec2 {
    float x, y;
    
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}

    inline Vec2 operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
    inline Vec2 operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
    inline Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
    inline Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
    inline bool operator==(const Vec2& v) { return (x==v.x && y==v.y); }

    inline bool operator!=(const Vec2& v) { return (x!=v.x && y!=v.y); }

    inline float magnitude() const { return std::sqrt(x*x + y*y); }
    inline Vec2 normalize() const { float mag = magnitude(); return Vec2(x / mag, y / mag); }
};


#endif