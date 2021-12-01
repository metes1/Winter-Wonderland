#include "particleSystem.h"
#include "Particle.h"
#include "mathLib3D.h"
#include <math.h>
#include <stdlib.h>
#include <time.h> 
#include <list>

using namespace std;

bool onTerrain = true;

particleSystem::particleSystem()
{
    list<Particle> s;
    this->particles = s;
}

//updates each particle
void particleSystem::update(bool windB)
{
    //computes new position of each particle
    list<Particle>::iterator it;
    for (it = particles.begin(); it != particles.end(); it++){

        if (windB) {
            it->direction.dx = 0.7;
        }
        else {
            it->direction.dx = 0.0;
        }

        //update position
        it->position.px = it->position.px + (it->direction.dx * it->speed);
        it->position.py = it->position.py + (it->direction.dy * it->speed);
        it->position.pz = it->position.pz + (it->direction.dz * it->speed);

        //checks if particle is on terrain
        if ((it->position.px <= 257.0) && (it->position.px >= 0.0) && (it->position.pz <= 257.0) && (it->position.pz >= 0.0))
            onTerrain = true;
        else
            onTerrain = false;
        
        //increases age of particle, if too old they die
        //particles get erased if they fall a certain distance off the floor
        it->age -= 1;
        if (it->age <= 0 || it->position.py < -25) {
            it = particles.erase(it);
            --it;
        }
    }
}

//adds new particle to system
void particleSystem::add()
{
    for (int i = 0; i < 10; i++) {
        Particle p;
        particles.push_back(p);
    }
}
