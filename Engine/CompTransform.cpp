#include "CompTransform.h"
#include "Component.h"

CompTransform::CompTransform(Comp_Type t) :Component(t)
{
}

CompTransform::~CompTransform()
{
}

void CompTransform::SetPos(float3 pos)
{
	position = pos;
}

void CompTransform::SetRot(float3 rot)
{
	rotation = rot;
}

void CompTransform::SetScale(float3 scal)
{
	scale = scal;
}

void CompTransform::Init(float3 p, float3 r, float3 s)
{
	SetPos(p);
	SetRot(r);
	SetScale(s);
}