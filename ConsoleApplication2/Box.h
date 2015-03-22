#ifndef BOX_INCLUDED
#define BOX_INCLUDED

#include <cyclone/cyclone.h>

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

#define CORNER_COUNT 8
#define RIB_COUNT 12
#define SIZE 2

class Box : cyclone::CollisionBox
{

public:
	Box();
	Box(cyclone::Vector3 position, double mass);
	virtual ~Box();

	void createBox(cyclone::Vector3 position, double mass);

	/*
	cyclone::CollisionSphere getCollisionSphere() const
{
	cyclone::CollisionSphere sphere;
	sphere.body = body;
	sphere.radius = halfSize.x;
	sphere.offset = cyclone::Matrix4();
	if(halfSize.y < sphere.radius) sphere.radius = halfSize.y;
	if(halfSize.z < sphere.radius) sphere.radius = halfSize.z;
	sphere.calculateInternals();
	return sphere;
}
	//*/
	void render()
	{
	GLfloat mat[16];
	body->getGLTransform(mat);

	if(body->getAwake()) glColor3f(0.5f,0.3f,0.3f);
	else glColor3f(0.3f,0.3f,0.5f);

	glPushMatrix();
	glMultMatrixf(mat);
	glScalef(halfSize.x*2,halfSize.y*2,halfSize.z*2);
	glutSolidCube(1.0f);
	glPopMatrix();

	}

	/** Render the box */
	virtual void display();

	// getters and setters
	double getMass() {return mass;}
	cyclone::Particle* getCorners() {return corners;}
	cyclone::ParticleRod* getRibs() {return ribs;}


private:
	cyclone::RigidBody* body;
	int mass;
	cyclone::Particle* corners;
	cyclone::ParticleRod* ribs;
};


#endif