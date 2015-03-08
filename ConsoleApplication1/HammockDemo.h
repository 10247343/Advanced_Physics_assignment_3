#ifndef HAMMOCK_INCLUDED
#define HAMMOCK_INCLUDED

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

#define PARTICLE_COUNT 8
#define CABLE_COUNT 10
#define SUPPORT_COUNT 4

#define PARTICLE_MASS 2

class HammockDemo : public Application
{
	cyclone::Particle *particles;
	cyclone::ParticleCable *cables;
	cyclone::ParticleCableConstraint *supports;

public:
	/** constructor and destructor */
	HammockDemo();
	virtual ~HammockDemo();

	/** creation of hammock */
	void createHammock();

	/** Return the title of the assignment */
	virtual const char* getTitle();
	/** update the scene */
	virtual void update();
	/** key listener */
	virtual void key(unsigned char key);
	/** Render the world */
	virtual void display();

	
};



Application* getApplication()
{
	return new HammockDemo();
}

#endif