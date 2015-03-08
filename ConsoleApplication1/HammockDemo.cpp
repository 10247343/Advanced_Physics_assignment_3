#include "HammockDemo.h"

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