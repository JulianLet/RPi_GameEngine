#pragma once
#include <cmath>
#include <cstddef>   // Defines std::size_t
#include <cstdint>   // defines uint8_t, uint16_t, uint32_t, uint64_t, etc.
#include <cstdlib>   // for rand() and srand()

struct Vector2
{
    float x;
    float y;

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    Vector2 operator +(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator -(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator *(float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 operator /(float scalar) const { return Vector2(x / scalar, y / scalar); }
    bool operator ==(const Vector2&other) const {return ( x==other.x && y == other.y); }

    Vector2& operator+=(const Vector2& other) 
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other) 
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2& operator*=(float scalar) 
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2& operator/=(float scalar) 
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }


    Vector2 Normalize()
    {
        float length = std::sqrt(x * x + y * y);

        float xN = 0;
        float yN = 0;

        if (length != 0.0f) 
        {
            xN = x / length;
            yN = y / length;
        }

        return Vector2(xN, yN);
    }

    float Magnitude()
    {
        return std::sqrt(x * x + y * y);
    }
};

// non-member operator to allow float * Vector2
inline Vector2 operator*(float scalar, const Vector2& vec)
{
    return Vector2(vec.x * scalar, vec.y * scalar);
}

// non-member operator to allow float * Vector2
inline Vector2 operator/(float scalar, const Vector2& vec)
{
    return Vector2(vec.x / scalar, vec.y / scalar);
}

// Dot product function
inline float Dot(const Vector2& a, const Vector2& b)
{
    return a.x * b.x + a.y * b.y;
}

//component ID
using ComponentID = std::size_t;

inline ComponentID GetUniqueComponentID()
{
    static ComponentID lastID = 0;
    return lastID++; //return +1 from last ID
}

template<typename T>
inline ComponentID GetComponentID() noexcept
{
    static ComponentID typeID = GetUniqueComponentID(); //because its static it will always be the same for every component type
    return typeID;
}

namespace Color
{
    // Your display = RBG (5/6/5 bits)
    inline uint16_t Color(uint8_t r, uint8_t g, uint8_t b)
    {
        uint16_t red   = (r >> 3) << 11;  // 5 bits
        uint16_t blue  = (b >> 2) << 5;   // 6 bits, but stored in middle!
        uint16_t green = (g >> 3);        // 5 bits at bottom
        return red | blue | green;
    }

    const uint16_t WHITE    = Color(255, 255, 255);
    const uint16_t LIGHTGRAY= Color( 50,  50,  50);
    const uint16_t GRAY     = Color(128, 128, 128);
    const uint16_t BLACK    = Color(  0,   0,   0);

    const uint16_t RED      = Color(255,   0,   0);
    const uint16_t GREEN    = Color(  0, 255,   0);
    const uint16_t BLUE     = Color(  0,   0, 255);

    const uint16_t YELLOW   = Color(255, 255,   0);
    const uint16_t CYAN     = Color(  0, 255, 255);
    const uint16_t MAGENTA  = Color(255,   0, 255);
    
    const uint16_t ORANGE   = Color(255, 165,   0);
    const uint16_t PURPLE   = Color(128,   0, 128);

    inline uint16_t RANDOM() // must seed rand() before use
    {
        uint8_t r = rand() % 32; // 0–31
        uint8_t b = rand() % 64; // 0–63
        uint8_t g = rand() % 32; // 0–31

        return (r << 11) | (b << 5) | g;
    }
};