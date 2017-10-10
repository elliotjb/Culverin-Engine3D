#include "Transform.h"
#include "Component.h"

Transform::Transform(Comp_Type t) :Component(t)
{
}

Transform::~Transform()
{
}

void Transform::SetPos(float3 pos)
{
	position = pos;
}

void Transform::SetRot(float3 rot)
{
	rotation = rot;
}

void Transform::SetSize(float3 size)
{
	this->size = size;
}
