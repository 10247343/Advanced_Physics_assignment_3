#ifndef HAMMOCK_INCLUDED
#define HAMMOCK_INCLUDED

#include <cyclone/cyclone.h>
#include <cyclone/world.h>

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

#include "Box.h"

#define NUMBER_OF_BOXES 16
#define PARTICLE_COUNT 8
#define ROD_COUNT 12

#define PARTICLE_MASS 1

#define MASSOBJECT_MASS 50
#if PARTICLE_COUNT % 2 == 0
#define NUMBER_OF_QUADRILATERALS (PARTICLE_COUNT - 2) / 2
#endif

class ShootBoxDemo : public RigidBodyApplication
{
	
public:
	/** constructor and destructor */
	ShootBoxDemo();
	virtual ~ShootBoxDemo();

	/** Return the title of the assignment */
	virtual const char* getTitle();
	/** update the scene */
	virtual void update();
	/** key listener */
	virtual void key(unsigned char key);
	/** Render the world */
	virtual void display();
	/** Processes the contact generation code. */
    virtual void generateContacts();
    /** Processes the objects in the simulation forward in time. */
    virtual void updateObjects(cyclone::real duration);
    /** Resets the position of all the bones. */
    virtual void reset();
	/** Sets up the rendering. */
    virtual void initGraphics();
    virtual void mouseDrag(int x, int y);

private:
	Box* boxes;
	cyclone::Particle* particles;
	cyclone::ParticleRod* rods;
	cyclone::ParticleWorld* world;
	cyclone::Vector3 lookTo;

	void ShootBox();
	cyclone::real GetRad(cyclone::real degrees);
	cyclone::Matrix3 RotateZ(cyclone::real degrees);
	cyclone::Matrix3 RotateY(cyclone::real degrees);
};



Application* getApplication()
{
	return new ShootBoxDemo();
}

#endif