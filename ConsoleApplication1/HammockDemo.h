#ifndef HAMMOCK_INCLUDED
#define HAMMOCK_INCLUDED

#include <app.h>

class HammockDemo : public Application
{

};

Application* getApplication()
{
	return new HammockDemo();
}

#endif