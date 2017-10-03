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

	//Indices Buffer
	glGenBuffers(1, (GLuint*) &(mesh.id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

}

void BaseObject::Draw()
{
	if (mesh.num_vertices > 0 && mesh.num_indices > 0)
	{
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);

		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, NULL);
		
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	else
	{
		LOG("Cannot draw the mesh");
	}

}
