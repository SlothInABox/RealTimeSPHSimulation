#pragma once
#include "Vector2D.h"

class Particle
{
public:
	Vector2D<float> position;
	Vector2D<float> velocity;
	Vector2D<float> force;
	float density;
	float pressure;

	Particle();
	Particle(const Vector2D<float>& position);
	Particle(const Particle& other);
	~Particle() = default;

	bool operator==(const Particle& other) const;
private:
};