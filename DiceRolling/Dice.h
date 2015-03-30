#ifndef DICE_INCLUDED
#define DICE_INCLUDED

#include <cyclone/cyclone.h>

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

// Size of the sides of the dice
#define SIZE 1
#define MASS 2

class Dice : public cyclone::CollisionBox
{

public:
	Dice();
	virtual ~Dice();

	/** Setting the propertys (location,direction) */
	void setDice(cyclone::Vector3 position, cyclone::Vector3 direction);

	/** Render the dice */
	void render();

	/** extra functions */
	cyclone::CollisionSphere getCollisionSphere() const
    {
        cyclone::CollisionSphere sphere;
        sphere.body = body;
        sphere.radius = halfSize.x;
        sphere.offset = cyclone::Matrix4();
        if (halfSize.y < sphere.radius) sphere.radius = halfSize.y;
        if (halfSize.z < sphere.radius) sphere.radius = halfSize.z;
        sphere.calculateInternals();
        return sphere;
    }

private:
	//
};


#endif