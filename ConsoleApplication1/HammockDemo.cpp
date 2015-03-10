#include "HammockDemo.h"

HammockDemo::HammockDemo()
{
	world = new cyclone::ParticleWorld(40);
	createHammock();
}

/** HammockDemo destructor function, clearing all particle arrays */
HammockDemo::~HammockDemo()
{
	if (particles) delete[] particles;
	if (cables) delete[] cables;
	if (supports) delete[] supports;
	if (rods) delete[] rods;
}


cyclone::Vector3 Shape::GetMid(const cyclone::Vector3& from, const cyclone::Vector3& to) const
{
	//Cast 0.5 to real for compatibility with cyclone.
	return from + (to - from) * (cyclone::real)0.5;
}

Line* Rect::GetCrossLines() const
{
	Line* toReturn = new Line[2];
	toReturn[0] = Line(GetMid(p0, p3), GetMid(p1, p2));
	toReturn[1] = Line(GetMid(p0, p1), GetMid(p2, p3));

	return toReturn;
}

Line* Triangle::GetCrossLines() const
{
	Line* toReturn = new Line[2];
	toReturn[0] = Line(p0, p1);
	toReturn[1] = Line(GetMid(p0, p1), p2);

	return toReturn;
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
			supports[i].anchor = cyclone::Vector3(-2,5,0);
		else 
			supports[i].anchor = cyclone::Vector3(16,5,0);
		supports[i].maxLength = 6.0f;
		supports[i].restitution = 0.9f;
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
	
	Application::update();
}

/** draw the world */
void HammockDemo::display()
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// place camera
	glLoadIdentity();
	gluLookAt(0.0, 10.0, -15.0, 2.0, 2.0, 2.0, 0.0, 1.0, 0.0);

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
}

/** key handler */
void HammockDemo::key(unsigned char key)
{
    switch(key)
    {
    case '1': printf( "test string");//currentShotType = PISTOL; break;
    }
}

void HammockDemo::SetMassPosition(const Shape& s)
{
	//Create a cross with the lines Parallel to the rect's lines.
	//These lines will form the plane we will use to place the mass.
	Line* lines = s.GetCrossLines();

	//Use one of the plane's points as a reference point fo calculate the offset.
	cyclone::Vector3 referencePoint(lines->start);
	cyclone::Vector3 xzOffset(massRelativePos - cyclone::Vector3(referencePoint.x, 0, referencePoint.z));

	//Now project the offset on the plane.
	//To do so we first need the plane its normal. Every line perpendicular to
	//the normal will be on the plane.
	cyclone::Vector3 planeNormal(lines->GetLine().vectorProduct((++lines)->GetLine()));
	planeNormal.normalise();

	//Now calculate projection line x on plane V with normal n using projection = x - (x * n)n
	cyclone::Vector3 xyzOffset(xzOffset - planeNormal * (xzOffset * planeNormal));

	//Now set the mass its position.
	massPos = referencePoint + xyzOffset;

	//Lastly clean up the mess we made.
	delete[] lines;
}