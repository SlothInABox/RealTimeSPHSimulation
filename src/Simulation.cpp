#include "Simulation.h"

Simulation::Simulation()
{
	for (float y = SimulationParameters::BOUNDARY_EPSILON; y < WindowParameters::VIEW_HEIGHT - SimulationParameters::BOUNDARY_EPSILON * 2.0f; y += SolverParameters::KERNEL_RADIUS)
	{
		for (float x = WindowParameters::VIEW_WIDTH / 4; x <= WindowParameters::VIEW_WIDTH / 2; x += SolverParameters::KERNEL_RADIUS)
		{
			if (particles.size() < SimulationParameters::DAM_PARTICLES)
			{
				float jitter = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
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

void Simulation::Update()
{
	CalculateDensityPressure();
	CalculateForces();
	Integrate();
}

void Simulation::CalculateDensityPressure()
{
	for (Particle& particle : particles)
	{
		particle.density = 0;
		for (Particle& otherParticle : particles)
		{
			Vector2D<float> disp = particle.position - otherParticle.position;
			float distSqrd = disp.MagnitudeSqrd();

			if (distSqrd < SolverParameters::KERNEL_RADIUS_SQRD)
			{
				particle.density += SolverParameters::PARTICLE_MASS * Kernel::POLY6 * std::pow(SolverParameters::KERNEL_RADIUS_SQRD - distSqrd, 3.0f);
			}
		}
		particle.pressure = SolverParameters::GAS_CONST * (particle.density - SolverParameters::REST_DENSITY);
	}
}

void Simulation::CalculateForces()
{
	for (Particle& particle : particles)
	{
		Vector2D<float> pressureForce;
		Vector2D<float> viscosityForce;

		for (Particle& otherParticle : particles)
		{
			if (particle == otherParticle)
			{
				continue;
			}

			Vector2D<float> disp = otherParticle.position - particle.position;
			float dist = disp.Magnitude();

			if (dist < SolverParameters::KERNEL_RADIUS)
			{
				// Pressure force contribution
				pressureForce += -disp.Normalize() * SolverParameters::PARTICLE_MASS * (particle.pressure + otherParticle.pressure) / (2.0f * otherParticle.density) * Kernel::SPIKY_GRAD * std::pow(SolverParameters::KERNEL_RADIUS - dist, 3.0f);
				// Viscosity force contribution
				viscosityForce += SolverParameters::VISCOSITY * SolverParameters::PARTICLE_MASS * (otherParticle.velocity - particle.velocity) / otherParticle.density * Kernel::VISC_LAP * (SolverParameters::KERNEL_RADIUS - dist);
			}
		}

		Vector2D<float> gravityForce = SolverParameters::G * SolverParameters::PARTICLE_MASS / particle.density;
		particle.force = pressureForce + viscosityForce + gravityForce;
	}
}

void Simulation::Integrate()
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
