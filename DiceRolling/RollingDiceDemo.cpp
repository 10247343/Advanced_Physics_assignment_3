#include "RollingDiceDemo.h"

#include <sstream>
#include <string>

RollingDiceDemo::RollingDiceDemo()
{
	positionReset = new cyclone::Vector3[NUMBER_OF_DICE];
	directionReset = new cyclone::Vector3[NUMBER_OF_DICE];
	newgame();
}

/** ShootBoxDemo destructor function */
RollingDiceDemo::~RollingDiceDemo()
{
	DeleteDice();
}

void RollingDiceDemo::DeleteDice(int index)
{
	// free the boxes memory and pop from back
	while(dice.size() > index){
		delete dice[dice.size()-1];
		dice.pop_back();
	}
}

void RollingDiceDemo::generateContacts()
{
    // Create the ground plane data
    cyclone::CollisionPlane plane;
    plane.direction = cyclone::Vector3(0,1,0);
    plane.offset = 0;

    // Set up the collision data structure
    cData.reset(maxContacts);
    cData.friction = (cyclone::real)0.9;
    cData.restitution = (cyclone::real)0.6;
    cData.tolerance = (cyclone::real)0.1;

	
	// Perform exhaustive collision detection on the ground plane
    cyclone::Matrix4 transform, otherTransform;
    cyclone::Vector3 position, otherPosition;
	// pick a box
	for(std::vector<Dice*>::iterator di = dice.begin(); di != dice.end(); ++di )
    {
        // Check for collisions with the ground plane
        if (!cData.hasMoreContacts()) return;
		
		//*
		if(cyclone::CollisionDetector::sphereAndTruePlane(di[0]->getCollisionSphere(),plane, &cData) > 0){
			if(cyclone::CollisionDetector::boxAndHalfSpace(*di[0], plane, &cData) > 0)
				//*/
		/*
        if(cyclone::CollisionDetector::boxAndHalfSpace(*di[0], plane, &cData) > 0){
			if(cyclone::CollisionDetector::sphereAndTruePlane(di[0]->getCollisionSphere(),plane, &cData) > 0)
				//*/
			{
				cData.contactCount -= 1;
			}
			else
			{
				//remove contact if not both colliders hit
				cData.contactCount -= 1;
			}
		}
        // Check for collisions with the other boxes that are still remaining for check
		for(std::vector<Dice*>::iterator other = di+1; other != dice.end(); ++other )
        {
			if(sqrtf(
				powf(di[0]->body->getPosition().x - other[0]->body->getPosition().x, 2.0) +
				powf(di[0]->body->getPosition().y - other[0]->body->getPosition().y, 2.0) +
				powf(di[0]->body->getPosition().z - other[0]->body->getPosition().z, 2.0))
				< 
				(other[0]->halfSize.x + di[0]->halfSize.x)
				)
			{
				//box collision
				if (!cData.hasMoreContacts()) return;
				cyclone::CollisionDetector::boxAndBox(*di[0], *other[0], &cData);
			}
		}
    }
}

void RollingDiceDemo::reset()
{
    // Reset the contacts
    cData.contactCount = 0;

	// clear memory
	DeleteDice(NUMBER_OF_DICE);

	printf("reset function\n");
	for(int i = 0; i < NUMBER_OF_DICE; i++)
	{
		// positionReset[i] holds the position of the dice so the game will be the same
		// directionReset[i] holds the direction of the dice so the game will be the same
		dice[i]->setDice(positionReset[i], directionReset[i]);
	}
	diceReleased = false;
}

void RollingDiceDemo::newgame()
{
	printf("Start new game\n");
	pauseSimulation = false; // start simulation

	// If dice vector is not empty clear vector before new game
	if(!dice.empty())
	{
		DeleteDice();
	}

	for(int i = 0; i < NUMBER_OF_DICE; i++)
	{
		cyclone::Random random = cyclone::Random(TimingData::get().getTime());
		cyclone::Vector3 position = cyclone::Vector3(
			random.randomReal()*MAX_SPACE_BETWEEN,
			random.randomReal()*MAX_SPACE_BETWEEN + 3,
			random.randomReal()*MAX_SPACE_BETWEEN
			); /////////////////////////////////////////////////////////////////random
		cyclone::Vector3 direction = cyclone::Vector3(1,1,1); ////////////////////////////////////////////////////////////////random

		dice.push_back(new Dice());
		positionReset[i] = position;
		directionReset[i] = direction;
		dice[i]->setDice(position, direction);
	}

	diceReleased = false;
}

void RollingDiceDemo::updateObjects(cyclone::real duration)
{
	for(std::vector<Dice*>::iterator di = dice.begin(); di != dice.end(); ++di )
    {
        di[0]->body->integrate(duration);
        di[0]->calculateInternals();
    }
}

void RollingDiceDemo::initGraphics()
{
	// placing a light in the world
    GLfloat lightAmbient[] = {0.8f,0.8f,0.8f,1.0f};
	//GLfloat lightAmbient1[] = {0.8f,0.8f,0.8f,1.0f};
    GLfloat lightDiffuse[] = {0.9f,0.95f,1.0f,1.0f};
	//GLfloat lightDiffuse1[] = {0.9f,0.95f,1.0f,1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);

    glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

    Application::initGraphics();
}

/** retrun demo title */
const char* RollingDiceDemo::getTitle()
{
	return "Assignment5, Rolling dice Demo";
}

/** update all objects in the demo */
void RollingDiceDemo::update()
{
	// time past since last update frame
	float timepast = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (timepast <= 0.0f) return;

	RigidBodyApplication::update();
}

/** draw the world */
void RollingDiceDemo::display()
{
	const static GLfloat lightPosition[] = {0.7f,-1,0.4f,0};
    const static GLfloat lightPositionMirror[] = {0.7f,1,0.4f,0};

    RigidBodyApplication::display();

	// Draw some circles to indicate the floor
    glColor3f(0.75, 0.75, 0.75);
    for (unsigned i = 1; i < 20; i++)
    {
        glBegin(GL_LINE_LOOP);
        for (unsigned j = 0; j < 32; j++)
        {
            float theta = 3.1415926f * j / 16.0f;
            glVertex3f(i*cosf(theta),0.0f,i*sinf(theta));
        }
        glEnd();
    }
    glBegin(GL_LINES);
    glVertex3f(-20,0,0);
    glVertex3f(20,0,0);
    glVertex3f(0,0,-20);
    glVertex3f(0,0,20);
    glEnd();
	
	// Render the dice
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_NORMALIZE);
    glColor3f(1,0,0);
	for(std::vector<Dice*>::iterator di = dice.begin(); di != dice.end(); ++di )
	{
		di[0]->render();
	}
    glDisable(GL_NORMALIZE);

    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    glDisable(GL_DEPTH_TEST);

	

	RenderText();
}

void RollingDiceDemo::RenderText()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	std::stringstream ss;
	ss << "Press R to reset the game.\n";
	ss << "Press N to start a game.\n";
	ss << "Press Space to roll the dice.\n";
	renderText(10.0f, 34.0f, ss.str().c_str());
}

/** key handler */
void RollingDiceDemo::key(unsigned char key)
{
	RigidBodyApplication::key(key);
    switch(key)
    {
	case 'n': printf( "new game\n"); newgame(); break;
	case 'r': printf( "retry game\n"); break;
	case 32: printf( "shoot\n"); RollDice(); break;
    }
	
}

void RollingDiceDemo::mouseDrag(int x, int y)
{
	RigidBodyApplication::mouseDrag(x, y);
}

void RollingDiceDemo::RollDice()
{
	//Early out if we're not allowed to roll yet.
	if (diceReleased)
		return;
	diceReleased = true;
	
	cyclone::Random random;

	for(int i = 0; i < dice.size(); i++)
	{
		float xaxis = random.randomReal()*1000;
		float zaxis = random.randomReal()*1000;
		random = cyclone::Random(TimingData::get().getTime());
		//Wake 'm up.
		dice[i]->body->setAwake();
		//Before you go go.
		//dice[i]->body->position = cyclone::Vector3(1,1,1);//////////////////////////////////////////////////randomness
		dice[i]->body->addForce(cyclone::Vector3(300+xaxis,0,300+zaxis));///////////////////////////////////////////////////randomness
		dice[i]->body->addTorque(cyclone::Vector3(500+xaxis,400,500+zaxis));
	}
}