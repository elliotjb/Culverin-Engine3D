#include "PhysBody3D.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ---------------------------------------------------------
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{
	body->setUserPointer(this);
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::Torque(float x, float y, float z)
{
	body->applyTorqueImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

void PhysBody3D::SetKinematic_Transform()
{
	if (this->is_kinematic == true)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

vec3 PhysBody3D::GetPos() const
{
	btTransform t = body->getWorldTransform();
	vec3 position(t.getOrigin().getX(), t.getOrigin().getY(), t.getOrigin().getZ());
	return position;
}

void PhysBody3D::SetAngVel(float x, float y, float z)
{
	body->SetAngularVelocity(btVector3(x, y, z));
}

void PhysBody3D::Move(float x, float y, float z)
{
	btTransform newTrans = body->getWorldTransform();
	newTrans.getOrigin() += (btVector3(x*0.1f, y*0.1f, z*0.1f));
	body->setWorldTransform(newTrans);
}

// ---------------------------------------------------------
void PhysBody3D::SetAsSensor(bool is_sensor)
{
	if (this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if (is_sensor == true)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			body->setCollisionFlags(body->getCollisionFlags() &~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

// ---------------------------------------------------------
bool PhysBody3D::IsSensor() const
{
	return is_sensor;
}

bool PhysBody3D::IsKinematic() const
{
	return is_kinematic;
}

btRigidBody* PhysBody3D::GetRigidBody()
{
	return body;
}
