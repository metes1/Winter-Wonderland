#include "mathLib3D.h"
#include "Particle.h"
#include <stdlib.h>
#include <time.h> 

//defines a particle object
Particle::Particle()
{
	this->position = Point3D((float)rand()/(float)(RAND_MAX/250.0) + 5.0, 200.0, (float)rand()/(float)(RAND_MAX/250.0) + 5.0);
	this->direction = Vec3D(0.0, -1.0, 0.0);
	this->speed = 1.0;
    this->age = 200000;
}
