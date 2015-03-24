#include "ShootBoxDemo.h"

#include <sstream>
#include <string>

ShootBoxDemo::ShootBoxDemo()
{
	//*
	//world = new cyclone::ParticleWorld(40);
	//cyclone::World* w = new cyclone::World(40);
	
	//world = new cyclone::World(80);
	boxes = new Box[NUMBER_OF_BOXES];
	massReset = new double[NUMBER_OF_BOXES];
	newgame();
}

/** HammockDemo destructor function, clearing all particle arrays */
ShootBoxDemo::~ShootBoxDemo()
{
	if (particles) delete[] particles;
	if (rods) delete[] rods;
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

	//*
	// Perform exhaustive collision detection on the ground plane
    cyclone::Matrix4 transform, otherTransform;
    cyclone::Vector3 position, otherPosition;
    for (Box *box = boxes; box < boxes+NUMBER_OF_BOXES; box++)
    {
        // Check for collisions with the ground plane
        if (!cData.hasMoreContacts()) return;
        cyclone::CollisionDetector::boxAndHalfSpace(*box, plane, &cData);

//        cyclone::CollisionSphere boxSphere = box->getCollisionSphere();

        // Check for collisions with each other box
        for (Box *other = box+1; other < boxes+NUMBER_OF_BOXES; other++)
        {
			//box collision
			//*
            if (!cData.hasMoreContacts()) return;
            cyclone::CollisionDetector::boxAndBox(*box, *other, &cData);
			//*/

			/*
            if (!cData.hasMoreContacts()) return;

            cyclone::CollisionSphere otherSphere = other->getCollisionSphere();
			
			cyclone::CollisionDetector::boxAndBox(
				box,
				other,
				&cData
				);

			
            cyclone::CollisionDetector::sphereAndSphere(
                boxSphere,
                otherSphere,
                &cData
                );
			//*/
        }
    }
	//*/
}

void ShootBoxDemo::reset()
{
    // Reset the contacts
    cData.contactCount = 0;

	printf("reset function\n");
	for(int i = 0; i < NUMBER_OF_BOXES-1; i++)
	{
		//cyclone::Vector3 position = cyclone::Vector3(-20,(SIZE+0.1)*((int)(i/4)),(SIZE+0.1)*(i%4));
		cyclone::Vector3 position = cyclone::Vector3(-20,(SIZE)*((int)(i/4)),(SIZE)*(i%4));
		//cyclone::Vector3 position = cyclone::Vector3(-20,(SIZE-0.1)*((int)(i/4)),(SIZE-0.1)*(i%4));

		boxes[i].setBox(position, massReset[i]);
	}
	boxes[NUMBER_OF_BOXES-1].setBox(cyclone::Vector3(0,0,0),shotMass);
}

void ShootBoxDemo::newgame()
{
	pauseSimulation = false;
	shotMass = 2.2;
	bulletAcceleration = cyclone::real(5000.0f);

	//*
	for(int i = 0; i < NUMBER_OF_BOXES - 1; i++)
	{
		//cyclone::Vector3 position = cyclone::Vector3(-20,(SIZE+0.1)*((int)(i/4)),(SIZE+0.1)*(i%4));
		cyclone::Vector3 position = cyclone::Vector3(-20,(SIZE)*((int)(i/4)),(SIZE)*(i%4));
		//cyclone::Vector3 position = cyclone::Vector3(-20,(SIZE-0.1)*((int)(i/4)),(SIZE-0.1)*(i%4));
		double mass = cyclone::Random().randomReal()*3.0;

		massReset[i] = mass;
		boxes[i].setBox(position, mass);
	}

	//boxes[NUMBER_OF_BOXES - 1] = *new Box(cyclone::Vector3(0, 0, 0), 2.2);
	boxes[NUMBER_OF_BOXES - 1].setBox(cyclone::Vector3(0, 0, 0), shotMass);
	bulletBox = boxes + (NUMBER_OF_BOXES - 1);
	bulletShot = false;

	lookTo = cyclone::Vector3(-1, 0, 0);
	//*/
}

void ShootBoxDemo::updateObjects(cyclone::real duration)
{
    for (Box *box = boxes; box < boxes+NUMBER_OF_BOXES; box++)
    {
        box->body->integrate(duration);
        box->calculateInternals();
    }

	//world->runPhysics(duration);
}

void ShootBoxDemo::initGraphics()
{
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

	//world->runPhysics(timepast);

	RigidBodyApplication::update();
	//Application::update();
}

/** draw the world */
void ShootBoxDemo::display()
{
	/*
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// place camera
	glLoadIdentity();
	gluLookAt(0.0, 0.0, -10.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	//*/
	const static GLfloat lightPosition[] = {0.7f,-1,0.4f,0};
    const static GLfloat lightPositionMirror[] = {0.7f,1,0.4f,0};

    RigidBodyApplication::display();

	// Render the bones
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_NORMALIZE);
    glColor3f(1,0,0);
    for(int i = 0; i < NUMBER_OF_BOXES; i++)
	{
		//boxes[i].display();
		boxes[i].render();
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
	case 32: printf( "shoot\n"); ShootBox(); break;
    }
	
}

void ShootBoxDemo::mouseDrag(int x, int y)
{
	RigidBodyApplication::mouseDrag(x, y);

	//Update the lookTo for aiming the box.
	lookTo = RotateZ(phi) * RotateY(theta) * cyclone::Vector3(-1, 0, 0);
}

void ShootBoxDemo::ShootBox()
{
	//Early out.
	if (!bulletBox || bulletShot)
		return;

	bulletBox->body->addForce(lookTo * bulletAcceleration);
}

cyclone::real ShootBoxDemo::GetRad(cyclone::real degrees)
{
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
	return cyclone::Matrix3(
		real_cos(rad),	-real_sin(rad),	0,
		real_sin(rad),	real_cos(rad),	0,
		0,				0,				1); 
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
	return cyclone::Matrix3(
		real_cos(rad),	0,	real_sin(rad),
		0,				1,	0,
		-real_sin(rad),	0,	real_cos(rad)); 
}
