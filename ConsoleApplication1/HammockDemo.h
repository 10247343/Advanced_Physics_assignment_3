#ifndef HAMMOCK_INCLUDED
#define HAMMOCK_INCLUDED

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

class HammockDemo : public Application
{
public:
	/** Return the title of the assignment */
	virtual const char* getTitle();
	/** update the scene */
	virtual void update();
	/** key listener */
	virtual void key(unsigned char key);
	/** Render the world */
	virtual void render();
};



Application* getApplication()
{
	return new HammockDemo();
}

#endif