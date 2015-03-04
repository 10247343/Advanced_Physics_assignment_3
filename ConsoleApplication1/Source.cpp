#include <cyclone/cyclone.h>
#include <cyclone/world.h>

int main()
{
	cyclone::World* world = new cyclone::World(30);

	delete world;

	return 0;
}