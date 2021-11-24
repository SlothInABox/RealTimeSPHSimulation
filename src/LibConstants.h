#pragma once
#include <cmath>
#include "Vector2D.h"

namespace SolverParameters
{
	const static Vector2D<float> G(0.0f, -10.0f);
	const static float GAS_CONST = 300.0f;
	const static float REST_DENSITY = 300.0f;
	const static float KERNEL_RADIUS = 16.0f;
	const static float KERNEL_RADIUS_SQRD = KERNEL_RADIUS * KERNEL_RADIUS;
	const static float PARTICLE_MASS = 2.5f;
	const static float VISCOSITY = 200.0f;
	const static float TIMESTEP = 0.0005f;
	const double PI = 3.14159265358979323846;
}

namespace Kernel
{
	const static float POLY6 = 4.0f / (SolverParameters::PI * std::pow(SolverParameters::KERNEL_RADIUS, 8.0f));
	const static float SPIKY_GRAD = -10.0f / (SolverParameters::PI * std::pow(SolverParameters::KERNEL_RADIUS, 5.0f));
	const static float VISC_LAP = 40.0f / (SolverParameters::PI * std::pow(SolverParameters::KERNEL_RADIUS, 5.0f));
}

namespace SimulationParameters
{
	const static float BOUNDARY_EPSILON = SolverParameters::KERNEL_RADIUS;
	const static float BOUND_DAMPING = -0.5f;
	const static int MAX_PARTICLES = 2500;
	const static int DAM_PARTICLES = 500;
	const static int BLOCK_PARTICLES = 250;
}

namespace WindowParameters
{
	const static int WINDOW_WIDTH = 800;
	const static int WINDOW_HEIGHT = 600;
	const static double VIEW_WIDTH = 1.5f * 800.0f;
	const static double VIEW_HEIGHT = 1.5f * 600.0f;
}
