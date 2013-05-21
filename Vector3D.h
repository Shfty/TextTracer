#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>

struct Vector3D
{
    float X = 0;
    float Y = 0;
    float Z = 0;

    Vector3D()
    {
    }

    Vector3D(int x, int y, int z)
    {
        X = (float)x;
        Y = (float)y;
        Z = (float)z;
    }

    Vector3D(float x, float y, float z)
    {
        X = x;
        Y = y;
        Z = z;
    }

    // Utility Methods
    const float Length()
    {
        return std::sqrt(X*X + Y*Y + Z*Z);
    }

    void Normalize()
    {
        float length = Length();
        X = X / length;
        Y = Y / length;
        Z = Z / length;
    }

    static const Vector3D Normalize(Vector3D vec)
    {
        float length = vec.Length();
        return Vector3D(vec.X / length, vec.Y / length, vec.Z / length);
    }

    static const float Dot(const Vector3D vec1, const Vector3D vec2)
    {
        return vec1.X * vec2.X + vec1.Y * vec2.Y + vec1.Z * vec2.Z;
    }

    static const Vector3D Cross(const Vector3D vec1, const Vector3D vec2)
    {
        Vector3D outVec;

        outVec.X = (vec1.Y * vec2.Z) - (vec2.Y * vec1.Z);
        outVec.Y = (vec1.Z * vec2.X) - (vec2.Z * vec1.X);
        outVec.Z = (vec1.X * vec2.Y) - (vec2.X * vec1.Y);

        return outVec;
    }

    // Operators
    Vector3D operator+(const Vector3D& rhs)
    {
        return Vector3D(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
    }

    Vector3D operator-(const Vector3D& rhs)
    {
        return Vector3D(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
    }

    Vector3D operator*(const float& rhs)
    {
        return Vector3D(X * rhs, Y * rhs, Z * rhs);
    }

    // Constants for convenience
    static const Vector3D Up()
    {
        return Vector3D(0, 1, 0);
    }

    static const Vector3D Down()
    {
        return Vector3D(0, -1, 0);
    }

    static const Vector3D Left()
    {
        return Vector3D(-1, 0, 0);
    }

    static const Vector3D Right()
    {
        return Vector3D(1, 0, 0);
    }

    static const Vector3D Forward()
    {
        return Vector3D(0, 0, -1);
    }

    static const Vector3D Backward()
    {
        return Vector3D(0, 0, 1);
    }

    static const Vector3D Zero()
    {
        return Vector3D(0, 0, 0);
    }

    static const Vector3D One()
    {
        return Vector3D(1, 1, 1);
    }
};

#endif // VECTOR3D_H
