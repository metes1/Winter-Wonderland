#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Particle.h"
#include "mathLib3D.h"
#include <list>

class particleSystem
{
	public:
	particleSystem();
	std::list<Particle> particles;
	void add();
	void update(bool windB);
};

#endif