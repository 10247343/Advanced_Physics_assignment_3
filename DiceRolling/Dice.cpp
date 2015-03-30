#include "Dice.h"

Dice::Dice()
{
	body = new cyclone::RigidBody;
}

Dice::~Dice()
{
	delete body;
}

/** Setting the propertys of the box */
void Dice::setDice(cyclone::Vector3 position, cyclone::Vector3 direction)
{	
	body->setPosition(position);
    body->setOrientation(cyclone::Quaternion());
    body->setVelocity(cyclone::Vector3());
    body->setRotation(cyclone::Vector3());
    halfSize = cyclone::Vector3(SIZE/1.6, SIZE/1.6, SIZE/1.6); // make circle a bit bigger than the actual cube

    body->setMass(MASS);
	body->addForce(direction);
	
    cyclone::Matrix3 tensor;
    tensor.setBlockInertiaTensor(halfSize, MASS);
    body->setInertiaTensor(tensor);
	
	// this makes the movement of the dice more natural
    body->setLinearDamping(0.85f);
    body->setAngularDamping(0.97f); // relly alow the roll
    body->clearAccumulators();
    body->setAcceleration(cyclone::Vector3::GRAVITY);
	
	// Set dice to sleep if no collision has happend yet
    body->setCanSleep(true);
    body->setAwake(false);
	//body->setAwake(true);
	
    body->calculateDerivedData();

	calculateInternals();
}

void Dice::render()
{
	//*
	GLfloat mat[16];
	body->getGLTransform(mat);

	// change collor to indicate state
	if(body->getAwake()) glColor3f(1.0f,0.0f,0.0f);
	else glColor3f(0.0f,0.0f,1.0f);

	glPushMatrix();
	glMultMatrixf(mat);
	glScalef(halfSize.x*2,halfSize.y*2,halfSize.z*2);
	//* // try alternative cubef
	float disp = SIZE/2.0;
	float side = SIZE/3.0;
	//*
#pragma region sides
	// begin sides of cube
	glBegin(GL_QUADS);
	//front
	glColor3f(0,0,1); // blue
	glVertex3f(-side,-side,disp); // lower left
	glVertex3f(-side,side,disp); // top left
	glVertex3f(side,side,disp); // top right
	glVertex3f(side,-side,disp); // lower right
	//back
	glColor3f(0,1,0); // green
	glVertex3f(-side,-side,-disp); // lower left
	glVertex3f(-side,side,-disp); // top left
	glVertex3f(side,side,-disp); // top right
	glVertex3f(side,-side,-disp); // lower right
	//left
	glColor3f(0,1,1); // cyan
	glVertex3f(-disp,-side,-side); // lower left
	glVertex3f(-disp,side,-side); // top left
	glVertex3f(-disp,side,side); // top right
	glVertex3f(-disp,-side,side); // lower right
	//right
	glColor3f(1,0,0); // red
	glVertex3f(disp,-side,-side); // lower left
	glVertex3f(disp,side,-side); // top left
	glVertex3f(disp,side,side); // top right
	glVertex3f(disp,-side,side); // lower right
	//top
	glColor3f(1,0,1); // magenta
	glVertex3f(-side,disp,-side); // lower left
	glVertex3f(-side,disp,side); // top left
	glVertex3f(side,disp,side); // top right
	glVertex3f(side,disp,-side); // lower right
	//bottom
	glColor3f(1,1,0); // yellow
	glVertex3f(-side,-disp,-side); // lower left
	glVertex3f(-side,-disp,side); // top left
	glVertex3f(side,-disp,side); // top right
	glVertex3f(side,-disp,-side); // lower right
	// end sides of cube
	glEnd();
#pragma endregion sides
	//*/
	//* 
#pragma region corners
	glBegin(GL_TRIANGLES);
	glColor3f(1,1,1);
	//back top left
	glVertex3f(-side,side,disp); 
	glVertex3f(-side,disp,side); 
	glVertex3f(-disp,side,side); 
	//back top right
	glVertex3f(side,side,disp); 
	glVertex3f(side,disp,side); 
	glVertex3f(disp,side,side); 
	//back bottom left
	glVertex3f(-side,-side,disp); 
	glVertex3f(-side,-disp,side); 
	glVertex3f(-disp,-side,side); 
	//back bottom right
	glVertex3f(side,-side,disp); 
	glVertex3f(side,-disp,side); 
	glVertex3f(disp,-side,side); 
	//front top left
	glVertex3f(-side,side,-disp); 
	glVertex3f(-side,disp,-side); 
	glVertex3f(-disp,side,-side); 
	//front top right
	glVertex3f(side,side,-disp); 
	glVertex3f(side,disp,-side); 
	glVertex3f(disp,side,-side); 
	//front bottom left
	glVertex3f(-side,-side,-disp); 
	glVertex3f(-side,-disp,-side); 
	glVertex3f(-disp,-side,-side); 
	//front bottom right
	glVertex3f(side,-side,-disp); 
	glVertex3f(side,-disp,-side); 
	glVertex3f(disp,-side,-side);
	glEnd();
#pragma endregion corners
	//*/
	//*
#pragma region ribs
	// begin ribs of cube
	glBegin(GL_QUADS);
	glColor3f(0.9,0.9,0.9);
	//front top
	glVertex3f(-side,side,-disp);
	glVertex3f(-side,disp,-side); 
	glVertex3f(side,disp,-side); 
	glVertex3f(side,side,-disp); 
	//front bottom
	glVertex3f(-side,-side,-disp);
	glVertex3f(-side,-disp,-side); 
	glVertex3f(side,-disp,-side); 
	glVertex3f(side,-side,-disp);
	//front right
	glVertex3f(-disp,-side,-side);
	glVertex3f(-disp,side,-side); 
	glVertex3f(-side,side,-disp); 
	glVertex3f(-side,-side,-disp);
	//front left
	glVertex3f(disp,-side,-side);
	glVertex3f(disp,side,-side); 
	glVertex3f(side,side,-disp); 
	glVertex3f(side,-side,-disp);

	//back top
	glVertex3f(-side,side,disp);
	glVertex3f(-side,disp,side); 
	glVertex3f(side,disp,side); 
	glVertex3f(side,side,disp); 
	//back bottom
	glVertex3f(-side,-side,disp);
	glVertex3f(-side,-disp,side); 
	glVertex3f(side,-disp,side); 
	glVertex3f(side,-side,disp);
	//back right
	glVertex3f(-disp,-side,side);
	glVertex3f(-disp,side,side); 
	glVertex3f(-side,side,disp); 
	glVertex3f(-side,-side,disp);
	//back left
	glVertex3f(disp,-side,side);
	glVertex3f(disp,side,side); 
	glVertex3f(side,side,disp); 
	glVertex3f(side,-side,disp);

	//left top
	glVertex3f(disp,side,side);
	glVertex3f(side,disp,side); 
	glVertex3f(side,disp,-side); 
	glVertex3f(disp,side,-side);
	//left bottom
	glVertex3f(disp,-side,side);
	glVertex3f(side,-disp,side); 
	glVertex3f(side,-disp,-side); 
	glVertex3f(disp,-side,-side);

	//right top
	glVertex3f(-disp,side,side);
	glVertex3f(-side,disp,side); 
	glVertex3f(-side,disp,-side); 
	glVertex3f(-disp,side,-side);
	//right bottom
	glVertex3f(-disp,-side,side);
	glVertex3f(-side,-disp,side); 
	glVertex3f(-side,-disp,-side); 
	glVertex3f(-disp,-side,-side);


	glEnd();
#pragma endregion ribs
	//*/

	//*/ // end alternative cube

	//glutSolidCube(1.0f);
	glPopMatrix();
	//*/
	
}
