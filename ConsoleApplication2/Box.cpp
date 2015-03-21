#include "Box.h";

Box::Box()
{

}

Box::~Box()
{
	if (corners) delete[] corners;
	if (ribs) delete[] ribs;
}

