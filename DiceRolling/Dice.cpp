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
    halfSize = cyclone::Vector3(SIZE/2, SIZE/2, SIZE/2);

    body->setMass(MASS);
	body->addForce(direction);
	
    cyclone::Matrix3 tensor;
    tensor.setBlockInertiaTensor(halfSize, MASS);
    body->setInertiaTensor(tensor);
	
	// this makes the movement of the dice more natural
    body->setLinearDamping(0.95f);
    body->setAngularDamping(0.9f);
    body->clearAccumulators();
    body->setAcceleration(cyclone::Vector3::GRAVITY);
	
	// Set dice to sleep if no collision has happend yet
    body->setCanSleep(true);
    body->setAwake(false);
	
    body->calculateDerivedData();

	calculateInternals();
}

void Dice::render()
{
	GLfloat mat[16];
	body->getGLTransform(mat);

	// change collor to indicate state
	if(body->getAwake()) glColor3f(1.0f,0.0f,0.0f);
	else glColor3f(0.0f,0.0f,1.0f);

	glPushMatrix();
	glMultMatrixf(mat);
	glScalef(halfSize.x*2,halfSize.y*2,halfSize.z*2);
	glutSolidCube(1.0f);
	glPopMatrix();
}