#include "Cylindre.h"

Cylindre::Cylindre()
{
}

Cylindre::Cylindre(const float3 pos, float lenght, float radius)
{

}

Cylindre::~Cylindre()
{
}

void Cylindre::Create()
{

}

void Cylindre::MakeCylinder(float radius, float length, unsigned int numSteps)
{

/*float hl = length * 0.5f;
	float a = 0.0f;
	float step = pi * 2 / (float)numSteps;
	for (int i = 0; i < numSteps; ++i)
	{
		float x = math::Cos(a) * radius;
		float y = math::Sin(a) * radius;
		vertex.push_back(float3(x, y, -hl));
		vertex.push_back(float3(x, y, hl));

		a += step;
	}
	/*glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < numSteps; ++i)
	{
		float x = math::Cos(a) * radius;
		float y = math::Sin(a) * radius;
		glVertex3f(x, y, -hl);
		glVertex3f(x, y, hl);

		a += step;
	}
	glEnd();*/

}