#include "Box.h"

Box::Box()
{
	printf("empty box");
}

Box::Box(cyclone::Vector3 position, double mass)
{
	//*
	corners = new cyclone::Particle[CORNER_COUNT];
	for(int i = 0; i < CORNER_COUNT; i++)
	{
		corners[i].setPosition(
			position.x + (i%2) * SIZE,
			position.y + (i > (CORNER_COUNT/2) ? SIZE : 0),
			position.z + (((int)(i/2) % 2) * SIZE)
			);
		//printf("%f,%f,%f \n", particles[i].getPosition().x, particles[i].getPosition().y, particles[i].getPosition().z);
		corners[i].setVelocity(0.0f,0.0f,0.0f);
		corners[i].setDamping(0.7f);
		//corners[i].setAcceleration(cyclone::Vector3::GRAVITY);
		corners[i].clearAccumulator();
		corners[i].setMass(mass);
		//world->getParticles().push_back(particles + i);
	}
	//*/
}

Box::~Box()
{
	if (corners) delete[] corners;
	if (ribs) delete[] ribs;
}

void Box::display()
{

	//*
	// draw particle (points)
	glColor3f(1,0,0);
	for(int i = 0; i < 8; i++){
		glPushMatrix();
		cyclone::Vector3 position = corners[i].getPosition();
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(0.1f, 10, 10);
		glPopMatrix();
	}
	//*/
}

