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
    inline uint16_t Color(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((b & 0xF8) << 8) | ((r & 0xFC) << 3) | (g >> 3);
    }
 
    const uint16_t WHITE     = Color(255, 255, 255);
    const uint16_t LIGHTGRAY = Color(170, 170, 170);
    const uint16_t GRAY      = Color(128, 128, 128);
    const uint16_t DARKGRAY  = Color(85,  85,  85);
    const uint16_t BLACK     = Color(0,    0,    0);
 
    const uint16_t RED       = Color(255, 0, 0);
    const uint16_t GREEN     = Color(0, 255, 0);
    const uint16_t BLUE      = Color(0, 0, 255);

    const uint16_t YELLOW    = Color(255, 255, 0);
    const uint16_t CYAN      = Color(0, 255, 255);
    const uint16_t MAGENTA   = Color(255, 0, 255);
 
    const uint16_t ORANGE    = Color(255, 160, 0);
    const uint16_t PURPLE    = Color(160, 0, 160);
 
    inline uint16_t RANDOM()
    {
        uint8_t r = rand() & 0x1F; // 5 bits
        uint8_t g = rand() & 0x3F; // 6 bits
        uint8_t b = rand() & 0x1F; // 5 bits

        return Color(r, g, b);
    }
}

namespace Random
{
    inline float Rand01()
    {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    inline float Range(float min, float max)
    {
        return min + (max - min) * Rand01();
    }
}

// uint32_t RenderSystemMask = TransformBit | RenderableBit | SpriteBit; <-- to use like this
// if (e->maks & RenderSystemMask)
// {
//    ...
// }
enum ComponentBit : uint32_t
{
    // AI
    AIBit                  = 1u << 0,

    // Core
    JumpBit                = 1u << 1,
    MovementBit            = 1u << 2,
    TimerBit               = 1u << 3,
    TransformBit           = 1u << 4,

    // Events
    OnEventBit             = 1u << 5,

    // Input
    InputIntentBit         = 1u << 6,
    InputMappingBit        = 1u << 7,

    // Physics
    ColliderBit            = 1u << 8,
    CollisionResponseBit   = 1u << 9,
    PhysicsBit             = 1u << 10,
    PhysicsMaterialBit     = 1u << 11,
    AnimationBit           = 1u << 12,

    // Rendering
    CameraBit              = 1u << 13,
    FollowTargetBit        = 1u << 14,
    RectangleBit           = 1u << 15,
    RenderableBit          = 1u << 16,
    SpriteBit              = 1u << 17,

    // Tilemap
    TilemapBit             = 1u << 18,

    // UI
    UIButtonBit            = 1u << 19,
    UITextBit              = 1u << 20
};