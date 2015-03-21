#include "ShootBoxDemo.h"

cyclone::Vector3 Shape::GetMid(const cyclone::Vector3& from, const cyclone::Vector3& to) const
{
	//Cast 0.5 to real for compatibility with cyclone.
	return from + (to - from) * (cyclone::real)0.5;
}

/*
#pragma region quadrilateral
Triangle* Quadrilateral::GetTriangles() const
{
	//Create 2 triangles.
	Triangle* toReturn = new Triangle[2];
	toReturn[0] = Triangle(p0, p1, p3);
	toReturn[1] = Triangle(p0, p2, p3);

	return toReturn;
}

bool Quadrilateral::IntersectsWithPoint(const cyclone::Vector3& v)
{
	bool toReturn;

	Triangle* triangles = GetTriangles();
	toReturn = triangles->IntersectsWithPoint(v) || (triangles + 1)->IntersectsWithPoint(v);
	delete[] triangles;

	return toReturn;
}

Line* Quadrilateral::GetCrossLines() const
{
	Line* toReturn = new Line[2];
	toReturn[0] = Line(GetMid(p0->getPosition(), p3->getPosition()), GetMid(p1->getPosition(), p2->getPosition()));
	toReturn[1] = Line(GetMid(p0->getPosition(), p1->getPosition()), GetMid(p2->getPosition(), p3->getPosition()));

	return toReturn;
}

const int Quadrilateral::FillArrayWithParticles(cyclone::Particle* array) const
{
	//Fill the given array with the particle values.
	*array = *p0;
	*(++array) = *p1;
	*(++array) = *p2;
	*(++array) = *p3;

	//return the length of the array.
	return 4;
}
#pragma endregion quadrilateral

#pragma region triangle
cyclone::real Triangle::GetLambda(const cyclone::Vector3& triangleLine, const cyclone::Vector3& p_p0)
{
	return (triangleLine * p_p0) / (triangleLine * triangleLine);
}

bool Triangle::IntersectsWithPoint(const cyclone::Vector3& p)
{
	/** Equation: p = p0 + (p1 - p0) * t + (p2 - p0) * s
	With: 
	p = point
	triangle(p0, p1, p2)

	Theory: A point is one of the triangle's point + scaled vectors from that triangle's
	point to the other triangle's points added up together. In this case we take p0
	and then use scaled vectors A(p1 - p0) and B(p2 - p0).

	We use the projection equation's lambda (lambda * (b / |b|)). Point intersects if
	0 <= lambda <= 1 for both s and t and t + s <= 1 
	lambda is calculated by dot(p - p0, X)/(dot(X, X) with 
	X = A or B 
	t, s and 1 - t - s are called the Barycentric coordinates.

	We are using the xz position, so all the y values have to be 0. */

/*

	cyclone::Vector3 p_p0(cyclone::Vector3(p.x, 0, p.z) - p0->getPosition()); 
	p_p0.y = 0;
	cyclone::Vector3 p1_p0((p1->getPosition() - p0->getPosition()));
	p1_p0.y = 0;
	cyclone::real t(GetLambda(p1_p0, p_p0));
	if (t < 0 || t > 1)
		return false; //t is to high/low.

	cyclone::Vector3 p2_p0((p2->getPosition() - p0->getPosition()));
	p2_p0.y = 0;
	cyclone::real s(GetLambda(p2_p0, p_p0));
	if (s < 0 || s > 1)
		return false; //s is to high/low.

	if (s + t <= 1)
		return true; //Intersection.

	return false; //No intersection.
}

Line* Triangle::GetCrossLines() const
{
	Line* toReturn = new Line[2];
	toReturn[0] = Line(p0->getPosition(), p1->getPosition());
	toReturn[1] = Line(GetMid(p0->getPosition(), p1->getPosition()), p2->getPosition());

	return toReturn;
}

const int Triangle::FillArrayWithParticles(cyclone::Particle* array) const
{
	//Fill the given array with the particle values.
	*array = *p0;
	*(++array) = *p1;
	*(++array) = *p2;

	//return the length of the array.
	return 3;
}
#pragma endregion triangle
//*/

ShootBoxDemo::ShootBoxDemo()
{
	cyclone::Vector3 test2 = *new cyclone::Vector3(2,0,4); 
	test2.invert();

	//*
	//world = new cyclone::ParticleWorld(40);
	boxes = new Box[NUMBER_OF_BOXES];
	
	cyclone::Particle test = *new cyclone::Particle();
	test.setMass(2.2);

	//*
	for(int i = 0; i < NUMBER_OF_BOXES; i++)
	{
		boxes[i] = *new Box(*new cyclone::Vector3(0,0,0), 2.2);
	}
	//*/
}

/** HammockDemo destructor function, clearing all particle arrays */
ShootBoxDemo::~ShootBoxDemo()
{
	if (particles) delete[] particles;
	if (rods) delete[] rods;
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
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// place camera
	glLoadIdentity();
	gluLookAt(10.0, 10.0, 0.0,
		10.0, 0.0, 0.0,
		0.0, 0.0, 1.0);

	for(int i = 0; i < NUMBER_OF_BOXES; i++)
	{
		boxes[i].display();
	}

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


