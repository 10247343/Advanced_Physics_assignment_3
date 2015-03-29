#ifndef SHOOTBOX_INCLUDED
#define SHOOTBOX_INCLUDED

#include <cyclone/cyclone.h>

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

#include "Dice.h"

#define NUMBER_OF_DICE 3
#define MAX_SPACE_BETWEEN 5

class RollingDiceDemo : public RigidBodyApplication
{
	
public:
	/** constructor and destructor */
	RollingDiceDemo();
	virtual ~RollingDiceDemo();

	/** remove boxes from vector from back till given index*/
	void DeleteDice(int index = 0);

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

	/** Function to start a new game with new hightes */
	virtual void newgame();

private:
	std::vector<Dice*> dice;
	cyclone::Vector3* positionReset;
	cyclone::Vector3* directionReset;
	bool diceReleased;

	/** Roll the dice */
	void RollDice();

	/** Renders the text required for this application */
	void RenderText();
};

Application* getApplication()
{
	return new RollingDiceDemo();
}

#endif