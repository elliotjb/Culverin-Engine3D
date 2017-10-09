#include "BaseObject.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "GL3W\include\glew.h"



BaseObject::BaseObject(Prim_Type t, bool w):type(t), wireframe(w)
{
}

BaseObject::BaseObject(Prim_Type t, const float3 p, bool k, Color c, bool w) :type(t), pos(p), isKynematic(k), color(c), wireframe(w)
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
	if (mesh.vertices != nullptr)
	{
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, mesh.vertices, GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, mesh.vertices3, GL_STATIC_DRAW);
	}
	
	//Indices Buffer
	glGenBuffers(1, (GLuint*) &(mesh.id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

	//Texture  Coords Buffer
	if (mesh.tex_coords != nullptr) // If the mesh has a texture, save it in the buffer
	{
		glGenBuffers(1, (GLuint*) &(mesh.id_texture));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_texture);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 2, mesh.tex_coords, GL_STATIC_DRAW);
	}

}

void BaseObject::Draw()
{
	if (mesh.num_vertices > 0 && mesh.num_indices > 0)
	{
		//Set Wireframe
		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		if (mesh.tex_coords == nullptr)
		{
			glColor4f(color.r, color.g, color.b, color.a);
		}

		else if(App->renderer3D->texture_2d)
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

		if (mesh.normals!= nullptr)
		{
			if (App->renderer3D->normals)
			{
				for (uint i = 0; i < mesh.num_vertices; i++)
				{
					glLineWidth(1.0f);
					glColor3f(1.0f, 1.0f, 1.0f);

					glBegin(GL_LINES);
					glVertex3f(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);
					glVertex3f(mesh.vertices[i] + mesh.normals[i], mesh.vertices[i + 1] + mesh.normals[i + 1], mesh.vertices[i + 2] + mesh.normals[i + 2]);
					glEnd();
				}
			}
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		//Disable Wireframe -> only this object will be wireframed
		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	else
	{
		LOG("Cannot draw the object %i", id);
	}
}

bool BaseObject::SetWireframe(bool w)
{
	wireframe = w;
	return wireframe;
}
