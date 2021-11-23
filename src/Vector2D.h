#pragma once
#include <cmath>

template <typename T>
class Vector2D
{
public:
	T x;
	T y;

	Vector2D();
	Vector2D(T x, T y);
	Vector2D(const Vector2D& other);
	Vector2D(T value);
	~Vector2D() = default;

	Vector2D operator+(const Vector2D& other) const;
	Vector2D operator-(const Vector2D& other) const;
	Vector2D operator-() const;
	Vector2D operator*(T value) const;
	Vector2D operator*(const Vector2D& other) const;
	Vector2D operator/(T value) const;
	Vector2D operator/(const Vector2D& other) const;
	
	Vector2D operator+=(const Vector2D& other);
	Vector2D operator*=(const Vector2D& other);

	bool operator==(const Vector2D& other) const;
	bool operator!=(const Vector2D& other) const;

	float Magnitude() const;
	float MagnitudeSqrd() const;

	Vector2D<float> Normalize() const;
};

template <typename T>
Vector2D<T> Vector2D<T>::operator-() const
{
	return { -x, -y };
}

template <typename T>
Vector2D<T> Vector2D<T>::operator*=(const Vector2D& other)
{
	this->x *= other.x;
	this->y *= other.y;
	return *this;
}

template <typename T>
Vector2D<T> Vector2D<T>::operator+=(const Vector2D& other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

template <typename T>
Vector2D<T> Vector2D<T>::operator/(const Vector2D& other) const
{
	return { x / other.x, y / other.y };
}

template <typename T>
Vector2D<T> Vector2D<T>::operator/(T value) const
{
	return { x / value, y / value };
}

template <typename T>
Vector2D<float> Vector2D<T>::Normalize() const
{
	return *this * (1 / this->Magnitude());
}

template <typename T>
float Vector2D<T>::MagnitudeSqrd() const
{
	return (float)x * (float)x + (float)y * (float)y;
}

template <typename T>
float Vector2D<T>::Magnitude() const
{
	return std::sqrt(MagnitudeSqrd());
}

template <typename T>
bool Vector2D<T>::operator!=(const Vector2D& other) const
{
	return !(*this == other);
}

template <typename T>
bool Vector2D<T>::operator==(const Vector2D& other) const
{
	return x == other.x && y == other.y;
}

template <typename T>
Vector2D<T> Vector2D<T>::operator*(T value) const
{
	return { x * value, y * value };
}

template <typename T>
Vector2D<T> Vector2D<T>::operator*(const Vector2D& other) const
{
	return { x * other.x, y * other.y };
}

template <typename T>
Vector2D<T> Vector2D<T>::operator-(const Vector2D& other) const
{
	return { x - other.x, y - other.y };
}

template <typename T>
Vector2D<T> Vector2D<T>::operator+(const Vector2D& other) const
{
	return { x + other.x, y + other.y };
}

template <typename T>
Vector2D<T>::Vector2D(T value)
{
	x = value;
	y = value;
}

template <typename T>
Vector2D<T>::Vector2D(const Vector2D& other)
{
	x = (T)other.x;
	y = (T)other.y;
}

template <typename T>
Vector2D<T>::Vector2D(T x, T y)
{
	this->x = x;
	this->y = y;
}

template <typename T>
Vector2D<T>::Vector2D()
{
	x = (T)0;
	y = (T)0;
}

template <typename T>
inline Vector2D<T> operator*(const float l, const Vector2D<T>& r)
{
	return r * l;
}
