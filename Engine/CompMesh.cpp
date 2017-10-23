#include "Application.h"
#include "Globals.h"
#include "Component.h"
#include "CompMesh.h"
#include "CompMaterial.h"
#include "ModuleRenderer3D.h"
#include "Color.h"
#include "GameObject.h"
#include <vector>

CompMesh::CompMesh(Comp_Type t, GameObject* parent) : Component(t, parent)
{
}

CompMesh::~CompMesh()
{
}



//void CompMesh::Init(std::vector<_Vertex> v, std::vector<uint> i)
//{
//	vertices = v;
//	indices = i;
//
//	float3 vert_origin;
//	float3 vert_norm;
//
//	SetupMesh();
//}

void CompMesh::InitRanges(uint num_vert, uint num_ind, uint num_normals)
{
	num_vertices = num_vert;
	num_indices = num_ind;

	if (num_normals > 0)
	{
		hasNormals = true;
	}
}

void CompMesh::Init(const float3* vert, const uint* ind, const float3* vert_normals, const float2* texCoord)
{

	// SET VERTEX DATA -------------------------------
	for (uint i = 0; i < num_vertices; i++)
	{
		_Vertex ver;
		// Vertex Positions ------------------
		ver.pos = vert[i];

		// Vertex Normals --------------------
		if (hasNormals)
		{
			ver.norm = vert_normals[i];
		}
		else
		{
			ver.norm.Set(0, 0, 0);
		}

		// Vertex Tex Coords ------------------
		ver.texCoords = texCoord[i];

		vertices.push_back(ver);
	}

	// SET INDEX DATA -----------------------------------------
	for (uint i = 0; i < num_indices; i++)
	{
		indices.push_back(ind[i]);
	}

	//NORMALS ARRAY ---------
	for (int i = 0; i < num_vertices; i++)
	{
		vertices_normals.push_back(float3(vertices[i].pos.x, vertices[i].pos.y, vertices[i].pos.z));
		vertices_normals.push_back(float3(vertices[i].pos.x + vertices[i].norm.x, vertices[i].pos.y + vertices[i].norm.y, vertices[i].pos.z + vertices[i].norm.z));
	}


	//// SET MATERIAL DATA -----------------------------------------
	//if (mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	//	std::vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//	// For the moment, we can only see textures on the diffuse channel, but we can load the specular ones
	//	std::vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//}

	////TRANSFORM DATA ---------------------------
	//aiQuaternion rot_quat;
	//aiVector3D pos, rot, scal;
	//float3 pos_vec, rot_vec, scal_vec;

	//transform.Decompose(scal, rot_quat, pos);
	//rot = rot_quat.GetEuler();

	//pos_vec.Set(pos.x, pos.y, pos.z);
	//rot_vec.Set(rot.x, rot.y, rot.z);
	//scal_vec.Set(scal.x, scal.y, scal.z);
	//------------------------------------------
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


void CompMesh::ShowInspectorInfo()
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
	if (ImGui::TreeNodeEx("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{

		ImGui::PopStyleColor();
		ImGui::Text("Name:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%s", name);
		ImGui::Text("Vertices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", num_vertices);
		ImGui::Text("Indices:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "%i", num_indices);
		
		ImGui::Checkbox("Render", &render);

		ImGui::TreePop();
	}
	else
	{
		ImGui::PopStyleColor();
	}
}

void CompMesh::Draw()
{
	if (vertices.size() > 0 && indices.size() > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);

		//Set Wireframe
		if (App->renderer3D->wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		//Set Color
		if (material != nullptr)
		{
			glColor4f(material->GetColor().r, material->GetColor().g, material->GetColor().b, material->GetColor().a);
		}
		else
		{
			glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
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

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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

void CompMesh::Update()
{
	if (render)
	{
		Draw();
	}
}

void CompMesh::Render(bool render)
{
	this->render = render;
}

void CompMesh::LinkMaterial(CompMaterial * mat)
{
	if (mat != nullptr)
	{
		material = mat;
		LOG("MATERIAL linked to MESH %s", name);
	}
}
