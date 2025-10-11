#pragma once
#include <cmath>

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


    void Normalize()
    {
        float length = std::sqrt(x * x + y * y);

        if (length != 0.0f) 
        {
            x /= length;
            y /= length;
        }
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