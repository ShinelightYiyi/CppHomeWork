#pragma once
#include<cmath>
#include <unordered_map>

class Vector2
{
public:
	int x, y;

	Vector2(): x(0),y(0){}
	Vector2(int x, int y) : x(x), y(y) {}  

    bool operator==(const Vector2& other) const 
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2& other) const 
    {
        return !(*this == other);
    }


    int distanceTo(const Vector2& other) const 
    {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }

    // 向量加法
    Vector2 operator+(const Vector2& other) const 
    {
        return Vector2(x + other.x, y + other.y);
    }

    // 向量减法
    Vector2 operator-(const Vector2& other) const 
    {
        return Vector2(x - other.x, y - other.y);
    }
};


namespace std 
{
    template <>
    struct hash<Vector2> 
    {
        size_t operator()(const Vector2& v) const 
        {
            return hash<int>()(v.x) ^ hash<int>()(v.y);
        }
    };
}