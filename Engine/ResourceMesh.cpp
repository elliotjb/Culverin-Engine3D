#include "ResourceMesh.h"
#include "Application.h"
#include "Globals.h"

ResourceMesh::ResourceMesh(uint uid) : Resource(uid, Resource::Type::MESH)
{
}


ResourceMesh::~ResourceMesh()
{
}

bool ResourceMesh::LoadToMemory()
{
	//glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertices_id);
	glGenBuffers(1, &indices_id);
	glGenBuffers(1, &vertices_norm_id);

	//glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(_Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

	if (hasNormals)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices_norm_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices_normals.size() * sizeof(float3), &vertices_normals[0], GL_STATIC_DRAW);
	}

	//// Vertex Positions -------------
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_Vertex), (void*)0);

	//// Vertex Normals ----------------
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(_Vertex), (void*)offsetof(_Vertex, norm));

	//glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

bool ResourceMesh::IsLoadedToMemory()
{
	return false;
}
