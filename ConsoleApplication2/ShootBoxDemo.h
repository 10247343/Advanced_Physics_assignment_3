#ifndef HAMMOCK_INCLUDED
#define HAMMOCK_INCLUDED

#include <cyclone/cyclone.h>

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

#define PARTICLE_COUNT 8
#define ROD_COUNT 12

#define PARTICLE_MASS 1

#define MASSOBJECT_MASS 50
#if PARTICLE_COUNT % 2 == 0
#define NUMBER_OF_QUADRILATERALS (PARTICLE_COUNT - 2) / 2
#endif

#pragma region line
/** Simple line container. */
struct Line
{
	Line(cyclone::Vector3 start = cyclone::Vector3(), cyclone::Vector3 end = cyclone::Vector3()) : start(start), end(end) { }

	cyclone::Vector3 start;
	cyclone::Vector3 end;

	cyclone::Vector3 GetLine() { return end - start; }
};
#pragma endregion line

#pragma region shape
/** Basic shape struct with member functions used for 
calculating the position of the mass object. */
struct Shape
{
	/** Gets the center coordinate of the line between vector to and from. */
	cyclone::Vector3 GetMid(const cyclone::Vector3& from, const cyclone::Vector3& to) const;
	/** Gets the cross lines from this shape. Needs shape specific implementation. 
	These lines will form a plane which is used for calculating the position of the mass object. */
	virtual Line* GetCrossLines() const = 0;
	virtual const int FillArrayWithParticles(cyclone::Particle*) const = 0;
	virtual bool IntersectsWithPoint(const cyclone::Vector3&) = 0;
};
#pragma endregion shape

#pragma region quadrilateral
/** Quadrilateral implementation of Shape. */
struct Triangle;
struct Quadrilateral : public Shape
{
	Quadrilateral() : p0(0x0), p1(0x0), p2(0x0), p3(0x0) { }
	Quadrilateral(cyclone::Particle* a0, cyclone::Particle* a1, cyclone::Particle* a2, cyclone::Particle* a3) :
		p0(a0), p1(a1), p2(a2), p3(a3) { }

	cyclone::Particle* p0;
	cyclone::Particle* p1;
	cyclone::Particle* p2;
	cyclone::Particle* p3;

	virtual bool IntersectsWithPoint(const cyclone::Vector3&);
	Triangle* GetTriangles() const;

	/** Returns the lines parallel to p1-p0 and p2-p1. */
	Line* GetCrossLines() const;
	virtual const int FillArrayWithParticles(cyclone::Particle*) const;
};
#pragma endregion quadrilateral

#pragma region triangle
/** Triangle implementation of Shape.
This struct was supposed to be used like Quadrilateral, but got changed.
Now this struct functions for an intersection check with a point. */
struct Triangle : public Shape
{
	Triangle() : p0(0x0), p1(0x0), p2(0x0) { }
	Triangle(cyclone::Particle* a0, cyclone::Particle* a1, cyclone::Particle* a2) :
		p0(a0), p1(a1), p2(a2) { }

	cyclone::Particle* p0;
	cyclone::Particle* p1;
	cyclone::Particle* p2;

	virtual bool IntersectsWithPoint(const cyclone::Vector3&);
	cyclone::real GetLambda(const cyclone::Vector3& triangleVec, const cyclone::Vector3& p_p0);

	/** Returns line p1-p0 and tline p2-(GetMid(p0,p1)). */
	virtual Line* GetCrossLines() const;
	virtual const int FillArrayWithParticles(cyclone::Particle*) const;
};
#pragma endregion triangle

class ShootBoxDemo : public Application
{
	
public:
	/** constructor and destructor */
	ShootBoxDemo();
	virtual ~ShootBoxDemo();

	/** Return the title of the assignment */
	virtual const char* getTitle();
	/** update the scene */
	virtual void update();
	/** key listener */
	virtual void key(unsigned char key);
	/** Render the world */
	virtual void display();

private:
	cyclone::Particle *particles;
	cyclone::ParticleRod *rods;
	cyclone::ParticleWorld *world;
};



Application* getApplication()
{
	return new ShootBoxDemo();
}

#endif