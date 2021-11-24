#include "Cell.h"


void Cell::ResetCell()
{
	particles.clear();
}

void Cell::AddParticle(Particle& particle)
{
	Particle* particlePtr = &particle;
	particles.push_back(particlePtr);
}
