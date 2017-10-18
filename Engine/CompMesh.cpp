#include "Application.h"
#include "Globals.h"
#include "Component.h"
#include "CompMesh.h"
#include "ModuleRenderer3D.h"
#include <vector>

CompMesh::CompMesh(Comp_Type t): Component(t)
{
}

CompMesh::CompMesh(Comp_Type t, std::vector<_Vertex> v, std::vector<uint> i): Component(t)
{
	vertices = v;
	indices = i;

	float3 vert_origin;
	float3 vert_norm;

	SetupMesh();
}


CompMesh::~CompMesh()
{
}

void CompMesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertices_id);
	glGenBuffers(1, &indices_id);
	glGenBuffers(1, &vertices_norm_id);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(_Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

	if (hasNormals)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices_norm_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices_normals.size() * sizeof(float3), &vertices_normals[0], GL_STATIC_DRAW);
	}

	// Vertex Positions -------------
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_Vertex), (void*)0);

	// Vertex Normals ----------------
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(_Vertex), (void*)offsetof(_Vertex, norm));

	glBindVertexArray(0);
}


void CompMesh::Draw()
{
	if (vertices.size() > 0 && indices.size() > 0)
	{
		//Set Wireframe
		if (App->renderer3D->wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);


		glBindBuffer(GL_ARRAY_BUFFER, vertices_id); //VERTEX ID
		glVertexPointer(3, GL_FLOAT, sizeof(_Vertex), NULL);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id); // INDICES ID
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);

		// NORMALS ----------------------------------
		if (App->renderer3D->normals && hasNormals)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertices_norm_id);
			glVertexPointer(3, GL_FLOAT, sizeof(float3), NULL);
			glDrawArrays(GL_LINES, 0, vertices.size() * 2);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);

		//Disable Wireframe -> only this object will be wireframed
		if (App->renderer3D->wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	else
	{
		LOG("Cannot draw the mesh");
	}
}