#include "Particle.h"

Particle::Particle()
{
	position = Vector2D<float>();
	velocity = Vector2D<float>();
	force = Vector2D<float>();
	density = 0;
	pressure = 0;
}

Particle::Particle(const Vector2D<float>& position)
{
	this->position = position;

	velocity = Vector2D<float>();
	force = Vector2D<float>();
	density = 0;
	pressure = 0;
}

Particle::Particle(const Particle& other)
{
	position = other.position;
	velocity = other.velocity;
	force = other.force;
	density = other.density;
	pressure = other.pressure;
}

bool Particle::operator==(const Particle& other) const
{
	return position == other.position;
}

