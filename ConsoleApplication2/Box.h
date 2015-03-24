#ifndef BOX_INCLUDED
#define BOX_INCLUDED

#include <cyclone/cyclone.h>

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

// Size of the sides of the box
#define SIZE 2

class Box : public cyclone::CollisionBox
{

public:
	Box();
	virtual ~Box();

	/** Setting the propertys (location,mass,etc) */
	void setBox(cyclone::Vector3 position, double mass);

	/** Render the box */
	void render();

	// getters and setters
	double getMass() {return mass;}

private:
	// mass of the box
	int mass;
};


#endif