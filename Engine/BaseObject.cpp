#include "BaseObject.h"
#include "GL3W\include\glew.h"

BaseObject::BaseObject(Prim_Type t)
{
	type = t;
}

BaseObject::~BaseObject()
{
}

void BaseObject::Init()
{
	//Vertices Buffer
	glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh.num_vertices * 3, mesh.vertices, GL_STATIC_DRAW);
}

void BaseObject::Draw()
{
}
