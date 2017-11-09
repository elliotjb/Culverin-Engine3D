#include "BaseGeometry.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "GL3W\include\glew.h"


BaseGeometry::BaseGeometry(Prim_Type t, bool w):type(t), wireframe(w)
{
}

BaseGeometry::BaseGeometry(Prim_Type t, const float3 p, bool k, Color c, bool w) :type(t), pos(p), isKynematic(k), color(c), wireframe(w)
{
}

BaseGeometry::~BaseGeometry()
{
}

void BaseGeometry::Init()
{
//	//Vertices Buffer
//	glGenBuffers(1, (GLuint*) &(mesh.id_vertices));
//	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
//	if (mesh.vertices != nullptr)
//	{
//		
//		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, mesh.vertices, GL_STATIC_DRAW);
//	}
//	else
//	{
//		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 3, mesh.vertices3, GL_STATIC_DRAW);
//	}
//	
//	//Indices Buffer
//	glGenBuffers(1, (GLuint*) &(mesh.id_indices));
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);
//
////Texture  Coords Buffer
//if (mesh.tex_coords != nullptr) // If the mesh has a texture, save it in the buffer
//{
//	glGenBuffers(1, (GLuint*) &(mesh.id_texture));
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_texture);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.num_vertices * 2, mesh.tex_coords, GL_STATIC_DRAW);
//}
}

void BaseGeometry::Clear()
{
}

void BaseGeometry::ClearTextures()
{
}

void BaseGeometry::GenFaceNormals(float3* centers)
{
}

void BaseGeometry::Draw()
{
	//if (mesh.num_vertices > 0 && mesh.num_indices > 0)
	//{
	//	//Set Wireframe
	//	if (wireframe)
	//	{
	//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	}

	//	if (mesh.tex_coords == nullptr)
	//	{
	//		glColor4f(color.r, color.g, color.b, color.a);
	//	}

	//	else if (App->renderer3D->texture_2d)
	//	{
	//		glEnable(GL_TEXTURE_2D);
	//		glBindTexture(GL_TEXTURE_2D, 0);
	//		glBindTexture(GL_TEXTURE_2D, mesh.id_image);

	//		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_texture);
	//		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	//	}

	//	glEnableClientState(GL_VERTEX_ARRAY);
	//	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
	//	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//	glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, NULL);
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//	if (mesh.normals != nullptr)
	//	{
	//		if (App->renderer3D->normals)
	//		{
	//			for (uint i = 0; i < mesh.num_vertices; i++)
	//			{
	//				glLineWidth(1.0f);
	//				glColor3f(1.0f, 1.0f, 1.0f);

	//				glBegin(GL_LINES);
	//				glVertex3f(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);
	//				glVertex3f(mesh.vertices[i] + mesh.normals[i], mesh.vertices[i + 1] + mesh.normals[i + 1], mesh.vertices[i + 2] + mesh.normals[i + 2]);
	//				glEnd();
	//			}
	//		}
	//	}

	//	glDisableClientState(GL_VERTEX_ARRAY);
	//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//	//Disable Wireframe -> only this object will be wireframed
	//	if (wireframe)
	//	{
	//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	}
	//}

	//else
	//{
	//	LOG("Cannot draw the object %i", id);
	//}
}


void BaseGeometry::Enable(bool active)
{
	this->active = active;
}

bool BaseGeometry::SetWireframe(bool w)
{
	wireframe = w;
	return wireframe;
}

bool BaseGeometry::isActive() const
{
	return active;
}

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<VertexOld> vert, std::vector<uint> ids, std::vector<Texture> tex, bool normals, uint faces, const char* name, float3 pos, float3 rot, float3 scale)
{
	this->name = name;
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;

	hasNormals = normals;
	numFaces = faces;

	vertices = vert;
	indices = ids;
	textures = tex;

	float3 vert_origin;
	float3 vert_norm;

	for (uint i = 0; i < vertices.size(); i++)
	{
		vert_origin.Set(vertices[i].pos.x, vertices[i].pos.y, vertices[i].pos.z);
		vert_norm.Set(vertices[i].pos.x + vertices[i].norm.x, vertices[i].pos.y + vertices[i].norm.y, vertices[i].pos.z + vertices[i].norm.z);
		vertices_normals.push_back(vert_origin);
		vertices_normals.push_back(vert_norm);
	}

	SetupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::Clear()
{
	LOG(" - Deleting mesh: %s.", name.c_str());

	//Free Buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &vertices_id);
	glDeleteBuffers(1, &indices_id);
	glDeleteBuffers(1, &vertices_norm_id);

	//Free Textures
	for (uint i = 0; i < textures.size(); i++)
	{
		glDeleteTextures(1, &textures[i].id);
	}

	vertices.clear();
	indices.clear();
	textures.clear();
	face_centers.clear();
	vertices_normals.clear();

}

void Mesh::ClearTex()
{
	//Free Textures
	for (uint i = 0; i < textures.size(); i++)
	{
		glDeleteTextures(1, &textures[i].id);
	}
}

void Mesh::ClearTexture(int t)
{
	if (t >= 0 && t < textures.size())
	{
		glDeleteTextures(1, &textures[t].id);
	}

	else
	{
		LOG("Texture index is wrong: %i", t);
	}
}

void Mesh::Draw()
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
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);



		if (App->renderer3D->texture_2d)
		{
			glEnable(GL_TEXTURE_2D);
			for (uint i = 0; i < textures.size(); i++)
			{
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}
		}

		glColor4f(color.r, color.g, color.b, color.a);

		glBindBuffer(GL_ARRAY_BUFFER, vertices_id); //VERTEX ID
		glVertexPointer(3, GL_FLOAT, sizeof(VertexOld), NULL);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VertexOld), (void*)offsetof(VertexOld, texCoords));
		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id); // INDICES ID
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
		if (App->renderer3D->normals && hasNormals)
		{
			// VERTEX NORMALS ----------------------------------
			glBindBuffer(GL_ARRAY_BUFFER, vertices_norm_id);
			glVertexPointer(3, GL_FLOAT, sizeof(float3), NULL);
			glDrawArrays(GL_LINES, 0, vertices.size() * 2);
			glBindBuffer(GL_ARRAY_BUFFER, 0);


			// FACE NORMALS ------------------------------------------
			/*for (uint i = 0; i < face_centers.size(); i++)
			{
				glBegin(GL_LINES);
				glLineWidth(1.0f);
				glColor3f(1.0f, 1.0f, 0.0f);
				glVertex3f(face_centers[i].pos.x, face_centers[i].pos.y, face_centers[i].pos.z);
				glVertex3f(face_centers[i].pos.x + face_centers[i].norm.x, face_centers[i].pos.y + face_centers[i].norm.y, face_centers[i].pos.z + face_centers[i].norm.z);
				glEnd();
			}*/
		}
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		//Disable Wireframe -> only this object will be wireframed
		if (App->renderer3D->wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	else
	{
		LOG("Cannot draw the object");
	}
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertices_id);
	glGenBuffers(1, &indices_id);
	glGenBuffers(1, &vertices_norm_id);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexOld), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

	if (hasNormals)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices_norm_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices_normals.size() * sizeof(float3), &vertices_normals[0], GL_STATIC_DRAW);
	}

	// Vertex Positions -------------
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexOld), (void*)0);

	// Vertex Normals ----------------
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexOld), (void*)offsetof(VertexOld, norm));

	// Vertex texture coords -----------
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexOld), (void*)offsetof(VertexOld, texCoords));

	glBindVertexArray(0);
}

void Mesh::SetTex(uint texID)
{
	for (uint i = 0; i < textures.size(); i++)
	{
		textures[i].id = texID;
	}
}

void Mesh::SetTexture(int t_index, uint texID)
{
	if (t_index >= 0 && t_index < textures.size())
	{
		textures[t_index].id = texID;
	}
	else
	{
		LOG("SETTING TEXTURE ERROR - texture index is wrong: %i", t_index);
	}
}
