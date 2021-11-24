#pragma once
#include <vector>
#include <iostream>
#include <cfloat>
#include <cmath>
#include "Particle.h"
#include "Vector2D.h"
#include "LibConstants.h"
#include "Cell.h"

class Fluid
{
public:
	std::vector<Particle> particles;

	Fluid();
	~Fluid() = default;

	void Update();

private:
	std::vector<std::vector<Cell>> grid;

	void CreateParticles();
	void CreateGrid();

	void CalculateDensityPressure();
	void CalculateForces();
	void Integrate();

	void UpdateGrid();
};