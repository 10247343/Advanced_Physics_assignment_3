#include "ShootBoxDemo.h"

ShootBoxDemo::ShootBoxDemo()
{
	//*
	//world = new cyclone::ParticleWorld(40);
	//cyclone::World* w = new cyclone::World(40);
	

	boxes = new Box[NUMBER_OF_BOXES];

	//*
	for(int i = 0; i < NUMBER_OF_BOXES; i++)
	{
		cyclone::Vector3 position = *new cyclone::Vector3(0,(SIZE+0.1)*((int)(i/4)),(SIZE+0.1)*(i%4));
		
		boxes[i] = *new Box(position, 2.2);
	}
	//*/
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

}

void ShootBoxDemo::reset()
{
    // Reset the contacts
    cData.contactCount = 0;
}

void ShootBoxDemo::updateObjects(cyclone::real duration)
{
    
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

	Application::update();
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
    glVertex3f(lookTo.x, lookTo.y, lookTo.z);
    cyclone::Vector3 temp(lookTo * 10);
    glVertex3f(temp.x, temp.y, temp.z);
	/*
	glVertex3f(-20,0,0);
    glVertex3f(20,0,0);
    glVertex3f(0,0,-20);
    glVertex3f(0,0,20);
	//*/

	/*
	// draw particle (points)
	glColor3f(1,0,0);
	for(int i = 0; i < PARTICLE_COUNT; i++){
		glPushMatrix();
		cyclone::Vector3 position = particles[i].getPosition();
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(0.1f, 10, 10);
		glPopMatrix();
	}
	//*/

	/*
	// draw beem
	glColor3f(1,0,0);
    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        const cyclone::Vector3 &point1 = rods[i].particle[0]->getPosition();
        const cyclone::Vector3 &point2 = rods[i].particle[1]->getPosition();
        glVertex3f(point1.x, point1.y, point1.z);
        glVertex3f(point2.x, point2.y, point2.z);
    }
	//*/
	glEnd();
}

/** key handler */
void ShootBoxDemo::key(unsigned char key)
{
    switch(key)
    {
	case '-': printf( "mass down"); break;
	case '+': printf( "mass up"); break;
	case 'n': printf( "new game"); break;
	case 'r': printf( "retry game"); break;
	case 32: printf( "shoot"); break;
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
		real_cos(rad), -real_sin(rad), 0,
		real_sin(rad), real_cos(rad), 0,
		0, 0, 1); 
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
		real_cos(rad), 0, real_sin(rad),
		0, 1, 0,
		-real_sin(rad), 0, real_cos(rad)); 
}
