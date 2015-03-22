#include "Box.h"

Box::Box()
{
	printf("empty box\n\n");

	//body = new cyclone::RigidBody();
	//createBox(*new cyclone::Vector3(0,0,0), 2.2);
}

Box::Box(cyclone::Vector3 position, double mass)
{
	//createBox(position,mass);
	body = new cyclone::RigidBody();
	createBox(position, mass);
}

Box::~Box()
{
	if (corners) delete[] corners;
	if (ribs) delete[] ribs;
	delete body;
}

void Box::createBox(cyclone::Vector3 position, double mass)
{	
	body->setPosition(position);
    body->setOrientation(*new cyclone::Quaternion());
    body->setVelocity(*new cyclone::Vector3());
    body->setRotation(*new cyclone::Vector3());
    halfSize = cyclone::Vector3(SIZE/2, SIZE/2, SIZE/2);

    body->setMass(mass);

	
    cyclone::Matrix3 tensor;
    tensor.setBlockInertiaTensor(halfSize, mass);
    body->setInertiaTensor(tensor);
	
    body->setLinearDamping(0.95f);
    body->setAngularDamping(0.8f);
    body->clearAccumulators();
    body->setAcceleration(cyclone::Vector3::GRAVITY);
	
    body->setCanSleep(false);
    body->setAwake();
	
    body->calculateDerivedData();

	// hier weet hij alles nog
	calculateInternals();
	/*
	//*/

	/*
	// create corners
	corners = new cyclone::Particle[CORNER_COUNT];
	for(int i = 0; i < CORNER_COUNT; i++)
	{
		//   7----8
		//  /|   /|
		// 5----6 |
		// | 3--|-4
		// |/   |/
		// 1----2
		corners[i].setPosition(
			position.x + (i%2) * SIZE,
			position.y + (i >= (CORNER_COUNT/2) ? SIZE : 0),
			position.z + (((int)(i/2) % 2) * SIZE)
			);
		printf("corner%d: %f,%f,%f \n", i, corners[i].getPosition().x, corners[i].getPosition().y, corners[i].getPosition().z);
		corners[i].setVelocity(0.0f,0.0f,0.0f);
		corners[i].setDamping(0.7f);
		//corners[i].setAcceleration(cyclone::Vector3::GRAVITY);
		corners[i].clearAccumulator();
		corners[i].setMass(mass);
		//world->getParticles().push_back(particles + i);
	}
	//*/

	/*
	// create rods
	ribs = new cyclone::ParticleRod[RIB_COUNT];
	int j = 0; // ribcounter
	for(int i = 0; i < CORNER_COUNT; i++)
	{
		if(i%2 == 0){ // x
			ribs[j].particle[0] = &corners[i];
			ribs[j].particle[1] = &corners[i+1];
			ribs[j].length = SIZE;
			j++;
		}
		if(i <= (CORNER_COUNT/2)) // y
		{
			ribs[j].particle[0] = &corners[i]; 
			ribs[j].particle[1] = &corners[i+4];
			ribs[j].length = SIZE;
			j++;
		}
		if(((int)(i/2) % 2) == 0 ) // z
		{
			ribs[j].particle[0] = &corners[i]; 
			ribs[j].particle[1] = &corners[i+2];
			ribs[j].length = SIZE;
			j++;
		}
		//world->getContactGenerators().push_back(&ribs[i]);
	}
	printf("ribs added: %d", j);
	//*/
}

void Box::display()
{
	glEnable(GL_NORMALIZE);
    glColor3f(1,0,0);
	render();
	glDisable(GL_NORMALIZE);
	/*
	// draw particle (points)
	glColor3f(1,0,0);
	for(int i = 0; i < CORNER_COUNT; i++){
		glPushMatrix();
		cyclone::Vector3 position = corners[i].getPosition();
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(0.1f, 10, 10);
		glPopMatrix();
	}
	//*/

	/*
	// draw ribs
	glColor3f(0,0,1);
    for (int i = 0; i < RIB_COUNT; i++)
    {
        const cyclone::Vector3 &point1 = ribs[i].particle[0]->getPosition();
        const cyclone::Vector3 &point2 = ribs[i].particle[1]->getPosition();
        glVertex3f(point1.x, point1.y, point1.z);
        glVertex3f(point2.x, point2.y, point2.z);
    }
	//*/
}

