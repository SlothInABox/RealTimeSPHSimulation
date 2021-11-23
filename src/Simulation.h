#pragma once
#include <vector>
#include <iostream>
#include "Particle.h"
#include "Vector2D.h"
#include "LibConstants.h"

class Simulation
{
public:
	std::vector<Particle> particles;

	Simulation();
	~Simulation() = default;

	void Update();

private:
	void CalculateDensityPressure();
	void CalculateForces();
	void Integrate();
};