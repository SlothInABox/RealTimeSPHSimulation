#pragma once
#include <vector>
#include "Particle.h"

struct Cell
{
	std::vector<Particle*> particles;
	std::vector<Cell*> neighbors;

	Cell() = default;
	~Cell() = default;

	void ResetCell();
	void AddParticle(Particle& particle);
};