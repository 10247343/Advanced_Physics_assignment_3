#ifndef BOX_INCLUDED
#define BOX_INCLUDED

#include <cyclone/cyclone.h>

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

class Box
{

public:
	Box();
	virtual ~Box();

	int getMass() {return mass;}
	cyclone::Particle* getCorners() {return corners;}
	cyclone::ParticleRod* getRibs() {return ribs;}


private:
	int mass;
	cyclone::Particle* corners;
	cyclone::ParticleRod* ribs;
};


#endif