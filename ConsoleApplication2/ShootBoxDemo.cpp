#include "ShootBoxDemo.h"

#include <sstream>
#include <string>

ShootBoxDemo::ShootBoxDemo()
{
	massReset = new double[NUMBER_OF_BOXES];
	newgame();
}

/** ShootBoxDemo destructor function */
ShootBoxDemo::~ShootBoxDemo()
{
	DeleteBoxes();
}

void ShootBoxDemo::DeleteBoxes(int index)
{
	// free the boxes memory and pop from back
	while(boxes.size() > index){
		delete boxes[boxes.size()-1];
		boxes.pop_back();
	}
}

void ShootBoxDemo::generateContacts()
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
	for(std::vector<Box*>::iterator box = boxes.begin(); box != boxes.end(); ++box )
    {
        // Check for collisions with the ground plane
        if (!cData.hasMoreContacts()) return;
        cyclone::CollisionDetector::boxAndHalfSpace(*box[0], plane, &cData);

        // Check for collisions with the other boxes that are still remaining for check
		for(std::vector<Box*>::iterator other = box+1; other != boxes.end(); ++other )
        {
			//box collision
            if (!cData.hasMoreContacts()) return;
            cyclone::CollisionDetector::boxAndBox(*box[0], *other[0], &cData);
        }
    }
}

void ShootBoxDemo::reset()
{
    // Reset the contacts
    cData.contactCount = 0;

	// set timer back to 0
	shotTimer = 0.0f;

	// clear memory
	DeleteBoxes(NUMBER_OF_BOXES + 1);

	printf("reset function\n");
	for(int i = 0; i < NUMBER_OF_BOXES; i++)
	{
		cyclone::Vector3 position = cyclone::Vector3(-20,(SIZE+0.01)*((int)(i/4)),(SIZE+0.01)*(i%4));
		// massReset[i] holds the mass of the box so the game will be the same
		boxes[i]->setBox(position, massReset[i]);
	}
	boxes[NUMBER_OF_BOXES]->setBox(cyclone::Vector3(0,0,0),shotMass);
	bulletShot = false;
}

void ShootBoxDemo::newgame()
{
	printf("Start new game\n");
	pauseSimulation = false; // start simulation
	shotMass = 2.2;
	bulletAcceleration = cyclone::real(5000.0f);
	shotTimer = 0.0f;

	// If boxes vector is not empty clear vector before new game
	if(!boxes.empty())
	{
		DeleteBoxes();
	}

	for(int i = 0; i < NUMBER_OF_BOXES; i++)
	{
		cyclone::Vector3 position = cyclone::Vector3(-20,(SIZE+0.01)*((int)(i/4)),(SIZE+0.01)*(i%4));
		double mass = cyclone::Random().randomReal()*5.0 + 3;

		// store the max of the box for reset
		massReset[i] = mass;
		boxes.push_back(new Box());
		boxes[i]->setBox(position, mass);
	}

	boxes.push_back(new Box());
	boxes[NUMBER_OF_BOXES]->setBox(cyclone::Vector3(0, 0, 0), shotMass);
	bulletShot = false;

	lookTo = cyclone::Vector3(-1, 0, 0);
}

void ShootBoxDemo::updateObjects(cyclone::real duration)
{
	for(std::vector<Box*>::iterator box = boxes.begin(); box != boxes.end(); ++box )
    {
        box[0]->body->integrate(duration);
        box[0]->calculateInternals();
    }
}

void ShootBoxDemo::initGraphics()
{
	// placing a light in the world
    GLfloat lightAmbient[] = {0.8f,0.8f,0.8f,1.0f};
    GLfloat lightDiffuse[] = {0.9f,0.95f,1.0f,1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glEnable(GL_LIGHT0);

    Application::initGraphics();
}

/** retrun demo title */
const char* ShootBoxDemo::getTitle()
{
	return "Assignment4, Shoot boxes Demo";
}

/** update all cables and suports in the demo */
void ShootBoxDemo::update()
{
	// time past since last update frame
	float timepast = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (timepast <= 0.0f) return;

	// bullet is shot we have to start a timer to spawn a new bulletBox
	if (bulletShot)
	{
		shotTimer += timepast;
		if (shotTimer >= TIME_BETWEEN_SHOTS)
		{
			bulletShot = false;
			shotTimer = 0.0f;
			Box* box(new Box());
			box->setBox(cyclone::Vector3(0, 0, 0), shotMass);
			boxes.push_back(box);
		}
	}

	RigidBodyApplication::update();
}

/** draw the world */
void ShootBoxDemo::display()
{
	const static GLfloat lightPosition[] = {0.7f,-1,0.4f,0};
    const static GLfloat lightPositionMirror[] = {0.7f,1,0.4f,0};

    RigidBodyApplication::display();

	// Render the boxes
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_NORMALIZE);
    glColor3f(1,0,0);
	for(std::vector<Box*>::iterator box = boxes.begin(); box != boxes.end(); ++box )
	{
		box[0]->render();
	}
    glDisable(GL_NORMALIZE);

    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    cyclone::Vector3 temp(lookTo * 10);
    glVertex3f(temp.x, temp.y, temp.z);
	glEnd();

	RenderText();
}

void ShootBoxDemo::RenderText()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	std::stringstream ss;
	ss << "Hold left mouse button and drag the mouse to aim.\n";
	ss << "Press R to reset with current values.\n";
	ss << "Press N to start a game with new mass values.\n";
	ss << "Press Space to shoot the bullet.\n";
	ss << "Press + or - to increase or decrease the mass.\n";
	ss << "Current mass: " << shotMass;
	renderText(10.0f, 64.0f, ss.str().c_str());
}

/** key handler */
void ShootBoxDemo::key(unsigned char key)
{
	RigidBodyApplication::key(key);
    switch(key)
    {
	case '+': printf( "mass up:%f\n",shotMass);shotMass += 0.1; break;
	case '-': printf( "mass down:%f\n",shotMass);shotMass -= 0.1; break;
	case 'n': printf( "new game\n"); newgame(); break;
	case 'r': printf( "retry game\n"); break;
	case 32: printf( "shoot\n");boxes[boxes.size() - 1]->body->setMass(shotMass); ShootBox(); break;
    }
	
}

void ShootBoxDemo::mouseDrag(int x, int y)
{
	RigidBodyApplication::mouseDrag(x, y);

	//Update the lookTo for aiming the box.
	//Use the phi and theta which is also used for the camera's rotation.
	lookTo = RotateZ(phi) * RotateY(theta) * cyclone::Vector3(-1, 0, 0);
}

void ShootBoxDemo::ShootBox()
{
	//Early out if we're not allowed to shoot yet.
	if (bulletShot)
		return;
	bulletShot = true;

	//Wake 'm up.
	boxes[boxes.size()-1]->body->setAwake();
	//Before you go go.
	boxes[boxes.size()-1]->body->addForce(lookTo * bulletAcceleration);
}

cyclone::real ShootBoxDemo::GetRad(cyclone::real degrees)
{
	//rad = degrees / 180 * PI.
	return degrees / cyclone::real(180.0f) * R_PI;
}

cyclone::Matrix3 ShootBoxDemo::RotateZ(cyclone::real degrees)
{
	cyclone::real rad(GetRad(degrees));
	/*
	Rotation matrix around Z axis.
	[cos(rad), -sin(rad), 0]
	[sin(rad),  cos(rad), 0]
	[       0,         0, 1]
	*/
	cyclone::real cosResult(real_cos(rad));
	cyclone::real sinResult(real_sin(rad));

	return cyclone::Matrix3(
		cosResult,	-sinResult,	0,
		sinResult,	cosResult,	0,
		0,			0,			1); 
}

cyclone::Matrix3 ShootBoxDemo::RotateY(cyclone::real degrees)
{
	cyclone::real rad(GetRad(-degrees));
	/*
	Rotation matrix around Y axis.
	[ cos(rad), 0, sin(rad)]
	[        0, 1,        0]
	[-sin(rad), 0, cos(rad)]
	*/
	cyclone::real cosResult(real_cos(rad));
	cyclone::real sinResult(real_sin(rad));

	return cyclone::Matrix3(
		cosResult,	0,	sinResult,
		0,			1,	0,
		-sinResult,	0,	cosResult); 
}
