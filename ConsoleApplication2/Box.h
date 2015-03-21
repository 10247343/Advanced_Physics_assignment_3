#ifndef BOX_INCLUDED
#define BOX_INCLUDED

#include <cyclone/cyclone.h>

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

#define CORNER_COUNT 8
#define SIZE 2

class Box
{

public:
	Box();
	Box(cyclone::Vector3 position, double mass);
	virtual ~Box();

	/** Render the box */
	virtual void display();

	// getters and setters
	double getMass() {return mass;}
	cyclone::Particle* getCorners() {return corners;}
	cyclone::ParticleRod* getRibs() {return ribs;}


private:
	int mass;
	cyclone::Particle* corners;
	cyclone::ParticleRod* ribs;
};


#endif