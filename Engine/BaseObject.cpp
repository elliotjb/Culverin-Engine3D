#include "BaseObject.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "GL3W\include\glew.h"



BaseObject::BaseObject(Prim_Type t):type(t)
{
}

BaseObject::BaseObject(Prim_Type t, const float3 p, bool k, Color c) :type(t), pos(p), isKynematic(k), color(c)
{
}

BaseObject::~BaseObject()
{
}

void BaseObject::Init()
{

	//Vertices Buffer
	glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, mesh.vertices, GL_STATIC_DRAW);

	//Indices Buffer
	glGenBuffers(1, (GLuint*) &(mesh.id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

	//Texture  Coords Buffer
	if (mesh.tex_coords != nullptr) // If the mesh has a texture
	{
		glGenBuffers(1, (GLuint*) &(mesh.id_texture));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_texture);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 2, mesh.tex_coords, GL_STATIC_DRAW);
	}

}

void BaseObject::Init_float3()
{
	//Vertices Buffer
	glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, mesh.vertices3, GL_STATIC_DRAW);

	//Indices Buffer
	glGenBuffers(1, (GLuint*) &(mesh.id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);
}

void BaseObject::Draw()
{
	if (mesh.num_vertices > 0 && mesh.num_indices > 0)
	{
		//Set Wireframe
		if (wireFrame)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		if (mesh.tex_coords == nullptr)
		{
			glColor4f(color.r, color.g, color.b, color.a);
		}

		else
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_2D, mesh.id_image);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.id_texture);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		//Disable Wireframe -> only this object will be wireframed
		if (wireFrame)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	else
	{
		LOG("Cannot draw the object %i", id);
	}
}
