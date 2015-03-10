#include "HammockDemo.h"

cyclone::Vector3 Shape::GetMid(const cyclone::Vector3& from, const cyclone::Vector3& to) const
{
	//Cast 0.5 to real for compatibility with cyclone.
	return from + (to - from) * (cyclone::real)0.5;
}

Triangle* Quadrilateral::GetTriangles() const
{
	//Create 2 triangles.
	Triangle* toReturn = new Triangle[2];
	toReturn[0] = Triangle(p0, p1, p3);
	toReturn[1] = Triangle(p1, p2, p3);

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

HammockDemo::HammockDemo()
{
	world = new cyclone::ParticleWorld(40);
	createHammock();

	massRelativePos = cyclone::Vector3(9, 0, 1);
	massPos = cyclone::Vector3(9, 0, 1);
}

/** HammockDemo destructor function, clearing all particle arrays */
HammockDemo::~HammockDemo()
{
	if (particles) delete[] particles;
	if (cables) delete[] cables;
	if (supports) delete[] supports;
	if (rods) delete[] rods;
}

/** creating the hammock */
void HammockDemo::createHammock()
{
	// creating all the points of the hammock
	particles = new cyclone::Particle[PARTICLE_COUNT];
	for(int i = 0; i < PARTICLE_COUNT; i++)
	{
		particles[i].setPosition(cyclone::real(i/2)*4.0f+2.0f,
			3.0f,
			i%2*2.0f-2.0f);
		printf("%f,%f,%f \n", particles[i].getPosition().x, particles[i].getPosition().y, particles[i].getPosition().z);
		particles[i].setVelocity(0.0f,0.0f,0.0f);
		particles[i].setDamping(0.7f);
		particles[i].setAcceleration(cyclone::Vector3::GRAVITY);
		particles[i].clearAccumulator();
		particles[i].setMass(PARTICLE_MASS);
		world->getParticles().push_back(particles + i);
	}

	//Check if the number of particles is even.
	//We only use the mass object if the number of particles is even.
#ifdef NUMBER_OF_QUADRILATERALS
	//Now create the Quadrilaterals.
	quadrilaterals = new Quadrilateral[NUMBER_OF_QUADRILATERALS];

	for (int i = 0; i < NUMBER_OF_QUADRILATERALS; i++)
	{
		int doubleI(i * 2);
		cyclone::Particle* p0 = &particles[doubleI++];
		cyclone::Particle* p1 = &particles[doubleI++];
		cyclone::Particle* p2 = &particles[doubleI++];
		cyclone::Particle* p3 = &particles[doubleI++];
		quadrilaterals[i] = Quadrilateral(p0, p1, p2, p3);
		p0 = 0x0; p1 = 0x0; p2 = 0x0; p3 = 0x0;
	}
#endif

	// placing cables between the particles to reprecent the hammock
	cables = new cyclone::ParticleCable[CABLE_COUNT];
	for(int i = 0; i < 6; i++)
	{
		cables[i].particle[0] = &particles[i];
		cables[i].particle[1] = &particles[i+2];
		cables[i].maxLength = 5.0f;
		cables[i].restitution = 0.5f;
        world->getContactGenerators().push_back(&cables[i]);
	}
	for(int i = 2; i < PARTICLE_COUNT-2; i += 2){
		int j = cyclone::real(i/2)+5;
		cables[j].particle[0] = &particles[i];
		cables[j].particle[1] = &particles[i+1];
		cables[j].maxLength = 2.5f;
		cables[j].restitution = 0.6f;
		world->getContactGenerators().push_back(&cables[i]);
	}

	// attaching suport to the hammock
	supports = new cyclone::ParticleCableConstraint[SUPPORT_COUNT];
	for(int i = 0; i < SUPPORT_COUNT; i++){
		int j = (i%2 + (cyclone::real(i/2)*(PARTICLE_COUNT-2)));
		supports[i].particle = &particles[j];
		if(i < SUPPORT_COUNT/2)
			supports[i].anchor = cyclone::Vector3(0,5,0);
		else 
			supports[i].anchor = cyclone::Vector3(16,5,0);
		supports[i].maxLength = 3.0f;
		supports[i].restitution = 0.7f;
		world->getContactGenerators().push_back(&supports[i]);
	}

	// placing rods in hammock to prevent collapsing
	rods = new cyclone::ParticleRod[ROD_COUNT];
	for(int i = 0; i < ROD_COUNT; i++)
	{
		int j = (i + i*(PARTICLE_COUNT-3));
		rods[i].particle[0] = &particles[j];
		rods[i].particle[1] = &particles[j+1];
	}

}

/** retrun demo title */
const char* HammockDemo::getTitle()
{
	return "Assignment3, Hammock Demo";
}

/** update all cables and suports in the demo */
void HammockDemo::update()
{
	// time past since last update frame
	float timepast = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (timepast <= 0.0f) return;

	world->runPhysics(timepast);
	
	//Check the quadrilateral the mass is on.
	Quadrilateral* colQuad(0x0);
	for (int i = 0; i < NUMBER_OF_QUADRILATERALS; i++)
	{
		if (quadrilaterals[i].IntersectsWithPoint(massRelativePos))
		{
			colQuad = &quadrilaterals[i];
			break;
		}
	}

	//Check if the mass is actually on a quadrilateral.
	if (colQuad)
	{
		//Reset the mass for the particles.
		for (int i = 0; i < PARTICLE_COUNT; i++)
			particles[i].setMass(PARTICLE_MASS);

		//Set massPos.
		SetMassPosition(*colQuad);
		//Now set the mass on the particles with the mass' current position.
		AddMassToParticlesIn(*colQuad);
	}

	Application::update();
}

/** draw the world */
void HammockDemo::display()
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// place camera
	glLoadIdentity();
	gluLookAt(0.0, 5.0, 15.0, 4.0, 0.0, 4.0, 0.0, 1.0, 0.0);

	//*
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

	glBegin(GL_LINES);
	//*
	// draw the cables
	glColor3f(0,1,0);
    for (int i = 0; i < CABLE_COUNT; i++)
    {
        const cyclone::Vector3 &point1 = cables[i].particle[0]->getPosition();
        const cyclone::Vector3 &point2 = cables[i].particle[1]->getPosition();
        glVertex3f(point1.x, point1.y, point1.z);
        glVertex3f(point2.x, point2.y, point2.z);
    }
	//*/

	//*
	// draw suport cables
	glColor3f(1, 1, 1);
    for (int i = 0; i < SUPPORT_COUNT; i++)
    {
        const cyclone::Vector3 &point1 = supports[i].particle->getPosition();
        const cyclone::Vector3 &point2 = supports[i].anchor;
        glVertex3f(point1.x, point1.y, point1.z);
        glVertex3f(point2.x, point2.y, point2.z);
    }
	//*/

	//*
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

	//Draw mass object.
	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslatef(massPos.x, massPos.y, massPos.z);
	glutSolidSphere(0.5f, 10, 10);
	glPopMatrix();
}

/** key handler */
void HammockDemo::key(unsigned char key)
{
    switch(key)
    {
	case '1': printf( "%f,%f,%f\n",world->getParticles()[3]->getPosition().x,world->getParticles()[3]->getPosition().y,world->getParticles()[3]->getPosition().z); break;
	case '2': printf( "%f,%f,%f\n",world->getParticles()[5]->getPosition().x,world->getParticles()[5]->getPosition().y,world->getParticles()[5]->getPosition().z); break;
	case 'w': massRelativePos.z -= 0.2f; break;
	case 's': massRelativePos.z += 0.2f; break;
	case 'a': massRelativePos.x -= 0.2f; break;
	case 'd': massRelativePos.x += 0.2f; break;
    }
}

void HammockDemo::SetMassPosition(const Quadrilateral& quadrilateral)
{	
	//Get the colliding traingle.
	Triangle* triangles = quadrilateral.GetTriangles();
	Triangle* target = triangles->IntersectsWithPoint(massRelativePos) ? triangles : (triangles + 1);

	//Get the lambdas for the relative point (so y = 0)
	cyclone::Vector3 tVec(target->p1->getPosition() - target->p0->getPosition()); //p1_p0
	cyclone::real t(target->GetLambda(cyclone::Vector3(tVec.x, 0, tVec.z), massRelativePos));
	cyclone::Vector3 sVec(target->p2->getPosition() - target->p0->getPosition()); //p2_p0
	cyclone::real s(target->GetLambda(cyclone::Vector3(sVec.x, 0, sVec.z), massRelativePos));

	//Now calculate the point with p = p0 + p1_p0 * t + p2_p0 * s.
	massPos = target->p0->getPosition() + tVec * t + sVec * s;

	//Now clean up the mess.
	delete[] triangles;
}

void HammockDemo::AddMassToParticlesIn(const Shape& s)
{
	//Get the paricles in an array and the length of the array.
	cyclone::Particle* particleArr = new cyclone::Particle[4];
	int length = s.FillArrayWithParticles(particleArr);

	//Get all the positions on the corrosponding indexes of the particle array.
	cyclone::Vector3* posArr = new cyclone::Vector3[length];

	//Some index variables.
	cyclone::Particle* parIndex = particleArr;
	cyclone::Vector3* posIndex = posArr;

	for (int i = 0; i < length; i++)
	{
		*posIndex = parIndex->getPosition();
		posIndex++; parIndex++; //Increment pointers.
	}

	//Reset the index variables.
	parIndex = particleArr;
	posIndex = posArr;

	//Safe check, avoid divide by zero by checking if mass object is standing on one 
	//of the particles. If so, apply all mass on that particle and exit member function.
	for (int i = 0; i < length; i++)
	{
		if (*posIndex == massPos)
		{
			//Give the particle all the mass.
			parIndex->setMass(PARTICLE_MASS + MASSOBJECT_MASS);

			//Clean up and exit this member function.
			delete[] particleArr;
			delete[] posArr;
			return;
		}
		posIndex++; parIndex++; //Increment pointers.
	}

	//Reset the index variables.
	parIndex = particleArr;
	posIndex = posArr;

	//Now calculate the total magnitude squared to each particle.
	//And to prevent doing this again, save all the values in an array.
	//We use squared instead of the actual magnitude to cause exponential differences in mass
	//dividing instead of linear dividing.
	cyclone::real* magnitudeArr = new cyclone::real[length];
	cyclone::real* magnitudeIndex = magnitudeArr; //Index variable.
	cyclone::real totalMagnitude(0);

	for (int i = 0; i < length; i++)
	{
		*magnitudeIndex = (*posIndex - massPos).squareMagnitude();
		totalMagnitude += *magnitudeIndex;

		magnitudeIndex++; posIndex++; //Increment pointers.
	}

	//Reset the index variables.
	magnitudeIndex = magnitudeArr;
	posIndex = posArr;

	//Loop through all the particles and give them the necessary mass.
	for (int i = 0; i < length; i++)
	{
		//Add the aditional mass by magnitude/totalmagnitude * MASSOBJECT_MASS.
		parIndex->setMass(PARTICLE_MASS + *magnitudeIndex * MASSOBJECT_MASS / totalMagnitude);

		posIndex++; parIndex++; magnitudeIndex++; //Increment pointers.
	}

	//Cleanup the mess.
	delete[] particleArr;
	delete[] posArr;
	delete[] magnitudeArr;
}