#include "HammockDemo.h"

HammockDemo::HammockDemo()
{
	createHammock();
}

/** HammockDemo destructor function, clearing all particle arrays */
HammockDemo::~HammockDemo()
{
	if (particles) delete[] particles;
	if (cables) delete[] cables;
	if (supports) delete[] supports;
}

/** creating the hammock */
void HammockDemo::createHammock()
{
	// creating all the points of the hammock
	particles = new cyclone::Particle[PARTICLE_COUNT];
	for(int i = 0; i < PARTICLE_COUNT; i++)
	{
		particles[i].setPosition(cyclone::real(i/2)*3.0f+2.0f,
			3.0f,
			i%2*2.0f-2.0f);
		particles[i].setVelocity(0.0f,0.0f,0.0f);
		particles[i].setDamping(0.7f);
		particles[i].setAcceleration(cyclone::Vector3::GRAVITY);
		particles[i].clearAccumulator();
		particles[i].setMass(PARTICLE_MASS);
	}

	// placing cables between the particles to reprecent the hammock
	cables = new cyclone::ParticleCable[CABLE_COUNT];
	for(int i = 0; i < 6; i++)
	{
		cables[i].particle[0] = &particles[i];
		cables[i].particle[1] = &particles[i+2];
		cables[i].maxLength = 2.0f;
		cables[i].restitution = 0.7f;
        //world.getContactGenerators().push_back(&cables[i]);
	}
	for(int i = 0; i < PARTICLE_COUNT; i += 2){
		int j = cyclone::real(i/2)+6;
		cables[j].particle[0] = &particles[i];
		cables[j].particle[1] = &particles[i+1];
		cables[j].maxLength = 1.5f;
		cables[j].restitution = 0.4f;
		//world.getContactGenerators().push_back(&cables[i]);
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
		supports[i].maxLength = 4.0f;
		supports[i].restitution = 0.8f;
		//world.getContactGenerators().push_back(&supports[i]);
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
		glutSolidSphere(0.5f, 10, 10);
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