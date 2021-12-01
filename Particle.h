#ifndef PARTICLE_H
#define PARTICLE_H

#include "mathLib3D.h"

class Particle {
public:
	Particle();
	Point3D position;
	Vec3D direction;
	float speed;
	int age;
};

#endif