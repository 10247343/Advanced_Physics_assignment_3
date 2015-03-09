#ifndef HAMMOCK_INCLUDED
#define HAMMOCK_INCLUDED

#include <cyclone/cyclone.h>

#include "../utils/app.h"
#include "../utils/timing.h"
#include "../utils/ogl_headers.h"

#define PARTICLE_COUNT 8
#define CABLE_COUNT 10
#define SUPPORT_COUNT 4

#define PARTICLE_MASS 2

#define MASSOBJECT_MASS 20
#if PARTICLE_COUNT % 2 == 0
#define NUMBER_OF_RECTS (PARTICLE_COUNT - 4) / 2
#endif

/** Simple line container. */
struct Line
{
	Line(cyclone::Vector3 start = cyclone::Vector3(), cyclone::Vector3 end = cyclone::Vector3()) : start(start), end(end) { }

	cyclone::Vector3 start;
	cyclone::Vector3 end;

	cyclone::Vector3 GetLine() { return end - start; }
};

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
	bool IntersectsWithPoint(const cyclone::Vector3&) = 0;
};

/** Rect implementation of Shape. */
struct Rect : public Shape
{
	Rect() : p0(0x0), p1(0x0), p2(0x0), p3(0x0)
	Rect(cyclone::Particle* a0, cyclone::Particle* a1, cyclone::Particle* a2, cyclone::Particle* a3) :
		p0(a0), p1(a1), p2(a2), p3(a3)

	cyclone::Particle* p0;
	cyclone::Particle* p1;
	cyclone::Particle* p2;
	cyclone::Particle* p3;

	bool IntersectsWithPoint(const cyclone::Vector3&);

	/** Returns the lines parallel to p1-p0 and p2-p1. */
	Line* GetCrossLines() const;
	virtual const int FillArrayWithParticles(cyclone::Particle*) const;
};

/** Triangle implementation of Shape.
This struct was supposed to be used like Rect, but got changed.
Now this struct functions for an intersection check with a point. */
struct Triangle : public Shape
{
	Triangle() : p0(0x0), p1(0x0), p2(0x0)
	Triangle(cyclone::Particle* a0, cyclone::Particle* a1, cyclone::Particle* a2) :
		p0(a0), p1(a1), p2(a2)

	cyclone::Particle* p0;
	cyclone::Particle* p1;
	cyclone::Particle* p2;

	bool IntersectsWithPoint(const cyclone::Vector3&);

	/** Returns line p1-p0 and tline p2-(GetMid(p0,p1)). */
	virtual Line* GetCrossLines() const;
	virtual const int FillArrayWithParticles(cyclone::Particle*) const;
};

class HammockDemo : public Application
{
public:
	/** constructor and destructor */
	HammockDemo();
	virtual ~HammockDemo();

	/** creation of hammock */
	void createHammock();

	/** Return the title of the assignment */
	virtual const char* getTitle();
	/** update the scene */
	virtual void update();
	/** key listener */
	virtual void key(unsigned char key);
	/** Render the world */
	virtual void display();
	/** Calculates the mass object its position on the hammock. */
	void SetMassPosition(const Shape&);
	void AddMassToParticlesIn(const Shape&);

private:
	cyclone::Vector3 massRelativePos;
	cyclone::Vector3 massPos;
	cyclone::Particle *particles;
	cyclone::ParticleCable *cables;
	cyclone::ParticleCableConstraint *supports;
	Rect* rects;
};



Application* getApplication()
{
	return new HammockDemo();
}

#endif