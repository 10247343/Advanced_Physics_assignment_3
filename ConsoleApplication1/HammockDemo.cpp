#include "HammockDemo.h"

const char* HammockDemo::getTitle()
{
	return "Assignment3, Hammock Demo";
}

void HammockDemo::update()
{
	// time past since last update frame
	float timepast = (float)TimingData::get().lastFrameDuration * 0.001f;
	if (timepast <= 0.0f) return;

	Application::update();
}

void HammockDemo::render()
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// place camera
	glLoadIdentity();
	gluLookAt(-25.0, 5.0, 0.0, 0.0, 5.0, 5.0, 0.0, 2.0, 0.0);
}

void HammockDemo::key(unsigned char key)
{
    switch(key)
    {
    case '1': printf("test");//currentShotType = PISTOL; break;
    }
}
