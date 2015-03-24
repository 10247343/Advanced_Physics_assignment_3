#ifndef SHOOTBOX_INCLUDED
#define SHOOTBOX_INCLUDED

#include <cyclone/cyclone.h>

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

#include "Box.h"

#define NUMBER_OF_BOXES 16
#define TIME_BETWEEN_SHOTS 4.0f

class ShootBoxDemo : public RigidBodyApplication
{
	
public:
	/** constructor and destructor */
	ShootBoxDemo();
	virtual ~ShootBoxDemo();

	/** remove boxes from vector from back till given index*/
	void DeleteBoxes(int index = 0);

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

	/** Function to start a new game with new masses */
	virtual void newgame();

private:
	std::vector<Box*> boxes;
	Box* bulletBox;
	bool bulletShot;
	float shotTimer;
	double* massReset;
	double shotMass;
	cyclone::real bulletAcceleration;
	cyclone::Vector3 lookTo;

	/** Shoots the box by giving it force */
	void ShootBox();
	/** Quick function for converting degrees into radians */
	cyclone::real GetRad(cyclone::real degrees);
	/** Quick function for rotating around the Z-axis with a certain degrees */
	cyclone::Matrix3 RotateZ(cyclone::real degrees);
	/** Quick function for rotating around the Y-axis with a certain degrees */
	cyclone::Matrix3 RotateY(cyclone::real degrees);

	/** Renders the text required for this application */
	void RenderText();
};



Application* getApplication()
{
	return new ShootBoxDemo();
}

#endif