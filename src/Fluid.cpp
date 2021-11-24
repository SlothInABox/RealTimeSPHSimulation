#include "Fluid.h"

Fluid::Fluid()
{
	CreateParticles();
	CreateGrid();
	UpdateGrid();
}

void Fluid::Update()
{
	CalculateDensityPressure();
	CalculateForces();
	Integrate();
	UpdateGrid();
}

void Fluid::CreateParticles()
{
	for (float y = SimulationParameters::BOUNDARY_EPSILON; y < WindowParameters::VIEW_HEIGHT - SimulationParameters::BOUNDARY_EPSILON * 2.0f; y += SolverParameters::KERNEL_RADIUS)
	{
		for (float x = WindowParameters::VIEW_WIDTH / 4; x <= WindowParameters::VIEW_WIDTH / 2; x += SolverParameters::KERNEL_RADIUS)
		{
			if (particles.size() < SimulationParameters::DAM_PARTICLES)
			{
				float jitter = (float)(rand()) / (float)(RAND_MAX);
				Vector2D<float> particlePosition(x + jitter, y);
				particles.emplace_back(Particle(particlePosition));
			}
			else
			{
				return;
			}
		}
	}
}

void Fluid::CreateGrid()
{
	for (float y = 0; y < WindowParameters::VIEW_HEIGHT; y+=SolverParameters::KERNEL_RADIUS * 2.0f)
	{
		std::vector<Cell> row;
		for (float x = 0; x < WindowParameters::VIEW_WIDTH; x+=SolverParameters::KERNEL_RADIUS * 2.0f)
		{
			row.emplace_back(Cell());
		}
		grid.push_back(row);
	}

	// Add nearest neighbors to cells
	for (int y = 0; y < grid.size(); y++)
	{
		for (int x = 0; x < grid[y].size(); x++)
		{
			for (int j = -1; j <= 1; j++)
			{
				int neighborY = y + j;
				if (neighborY < 0 || neighborY > grid.size() - 1)
				{
					continue;
				}

				for (int i = -1; i <= 1; i++)
				{
					int neighborX = x + i;
					if (neighborX < 0 || neighborX > grid[y].size() - 1)
					{
						continue;
					}

					Cell* neighbor = &grid[neighborY][neighborX];
					grid[y][x].neighbors.push_back(neighbor);
				}
			}
		}
	}
}

void Fluid::CalculateDensityPressure()
{
	for (std::vector<Cell>& row : grid)
	{
		for (Cell& cell : row)
		{
			for (Particle* particle : cell.particles)
			{
				particle->density = 0;

				for (Cell* neighbor : cell.neighbors)
				{
					for (Particle* otherParticle : neighbor->particles)
					{
						Vector2D<float> disp = particle->position - otherParticle->position;
						float distSqrd = disp.MagnitudeSqrd();

						if (distSqrd < SolverParameters::KERNEL_RADIUS_SQRD)
						{
							particle->density += SolverParameters::PARTICLE_MASS * Kernel::POLY6 * std::pow(SolverParameters::KERNEL_RADIUS_SQRD - distSqrd, 3.0f);
						}
					}
				}
				particle->pressure = SolverParameters::GAS_CONST * (particle->density - SolverParameters::REST_DENSITY);
			}
		}
	}
}

void Fluid::CalculateForces()
{
	//for (Particle& particle : particles)
	//{
	//	Vector2D<float> pressureForce;
	//	Vector2D<float> viscosityForce;

	//	for (Particle& otherParticle : particles)
	//	{
	//		if (particle == otherParticle)
	//		{
	//			continue;
	//		}

	//		Vector2D<float> disp = otherParticle.position - particle.position;
	//		float dist = disp.Magnitude();

	//		if (dist < SolverParameters::KERNEL_RADIUS)
	//		{
	//			// Pressure force contribution
	//			pressureForce += -disp.Normalize() * SolverParameters::PARTICLE_MASS * (particle.pressure + otherParticle.pressure) / (2.0f * otherParticle.density) * Kernel::SPIKY_GRAD * std::pow(SolverParameters::KERNEL_RADIUS - dist, 3.0f);
	//			// Viscosity force contribution
	//			viscosityForce += SolverParameters::VISCOSITY * SolverParameters::PARTICLE_MASS * (otherParticle.velocity - particle.velocity) / otherParticle.density * Kernel::VISC_LAP * (SolverParameters::KERNEL_RADIUS - dist);
	//		}
	//	}

	//	Vector2D<float> gravityForce = SolverParameters::G * SolverParameters::PARTICLE_MASS / particle.density;
	//	particle.force = pressureForce + viscosityForce + gravityForce;
	//}

	for (std::vector<Cell>& row : grid)
	{
		for (Cell& cell : row)
		{
			for (Particle* particle : cell.particles)
			{
				Vector2D<float> pressureForce;
				Vector2D<float> viscosityForce;

				for (Cell* neighbor : cell.neighbors)
				{
					for (Particle* otherParticle : neighbor->particles)
					{
						if (particle == otherParticle)
						{
							continue;
						}

						Vector2D<float> disp = otherParticle->position - particle->position;
						float dist = disp.Magnitude();

						if (dist < SolverParameters::KERNEL_RADIUS)
						{
							// Pressure force contribution
							pressureForce += -disp.Normalize() * SolverParameters::PARTICLE_MASS * (particle->pressure + otherParticle->pressure) / (2.0f * otherParticle->density) * Kernel::SPIKY_GRAD * std::pow(SolverParameters::KERNEL_RADIUS - dist, 3.0f);
							// Viscosity force contribution
							viscosityForce += SolverParameters::VISCOSITY * SolverParameters::PARTICLE_MASS * (otherParticle->velocity - particle->velocity) / otherParticle->density * Kernel::VISC_LAP * (SolverParameters::KERNEL_RADIUS - dist);
						}
					}
				}

				Vector2D<float> gravityForce = SolverParameters::G * SolverParameters::PARTICLE_MASS / particle->density;
				particle->force = pressureForce + viscosityForce + gravityForce;
			}
		}
	}
}

void Fluid::Integrate()
{
	for (Particle& particle : particles)
	{
		particle.velocity += SolverParameters::TIMESTEP * particle.force / particle.density;
		particle.position += SolverParameters::TIMESTEP * particle.velocity;

		// Boundary conditions
		if (particle.position.x - SimulationParameters::BOUNDARY_EPSILON < 0.0f)
		{
			particle.velocity *= SimulationParameters::BOUND_DAMPING;
			particle.position.x = SimulationParameters::BOUNDARY_EPSILON;
		}
		if (particle.position.x + SimulationParameters::BOUNDARY_EPSILON > WindowParameters::VIEW_WIDTH)
		{
			particle.velocity *= SimulationParameters::BOUND_DAMPING;
			particle.position.x = WindowParameters::VIEW_WIDTH - SimulationParameters::BOUNDARY_EPSILON;
		}
		if (particle.position.y - SimulationParameters::BOUNDARY_EPSILON < 0.0f)
		{
			particle.velocity *= SimulationParameters::BOUND_DAMPING;
			particle.position.y = SimulationParameters::BOUNDARY_EPSILON;
		}
		if (particle.position.y + SimulationParameters::BOUNDARY_EPSILON > WindowParameters::VIEW_HEIGHT)
		{
			particle.velocity *= SimulationParameters::BOUND_DAMPING;
			particle.position.y = WindowParameters::VIEW_HEIGHT - SimulationParameters::BOUNDARY_EPSILON;
		}
	}
}

void Fluid::UpdateGrid()
{
	for (std::vector<Cell>& i : grid)
	{
		for (Cell& cell : i)
		{
			cell.ResetCell();
		}
	}

	for (Particle& particle : particles)
	{
		// Find the correct cell that the particle is inside from it's position
		int y = particle.position.y / (SolverParameters::KERNEL_RADIUS * 2.0f);
		int x = particle.position.x / (SolverParameters::KERNEL_RADIUS * 2.0f);

		grid[y][x].AddParticle(particle);
	}
}
