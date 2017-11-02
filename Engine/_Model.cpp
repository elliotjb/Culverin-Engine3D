#include "_Model.h"
#include "Globals.h"
#include "Application.h"
#include "BaseGeometry.h"
#include "ModuleTextures.h"
#include "WindowInspector.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "CompMaterial.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

_Model::_Model(const char* path, Prim_Type type, bool wire) :BaseGeometry(type, wire)
{
	color = White;
	LoadModel(path);
	SetColor(color);
}

_Model::~_Model()
{
}

void _Model::Draw()
{
	for (uint i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw();
	}

	if (bb_active)
	{
		for (uint i = 0; i < 12; i++)
		{
			glBegin(GL_LINES);
			glLineWidth(1.0f);
			glVertex3f(bounding_box.Edge(i).a.x, bounding_box.Edge(i).a.y, bounding_box.Edge(i).a.z);
			glVertex3f(bounding_box.Edge(i).b.x, bounding_box.Edge(i).b.y, bounding_box.Edge(i).b.z);
			glEnd();
		}	
	}
}

void _Model::Clear()
{
	LOG("DELETING MODEL %s.", base_info.name.c_str());

	//CLEAR MESHES OF THE MODEL
	for (uint i = 0; i < meshes.size(); i++)
	{
		meshes[i].Clear();
	}

	loaded_tex.clear();
}

void _Model::ClearTextures()
{
	//CLEAR TEXTURES OF EACH MESH
	for (uint i = 0; i < meshes.size(); i++)
	{
		meshes[i].ClearTex();
	}
}

void _Model::ClearTexture(int m, int t, int t_id)
{
	if (m >= 0 && m < meshes.size())
	{
		meshes[m].ClearTexture(t);
	}
	else
	{
		LOG("Mesh index is wrong: %i", m);
	}
}

void _Model::LoadModel(const char * path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		float3 min;
		float3 max;
		
		// INIT THE BOUNDING BOX COORDS ---------
		if (scene->mMeshes[0]->mNumVertices > 0)
		{
			min.x = scene->mMeshes[0]->mVertices[0].x;
			min.y = scene->mMeshes[0]->mVertices[0].y;
			min.z = scene->mMeshes[0]->mVertices[0].z;

			max.x = scene->mMeshes[0]->mVertices[0].x;
			max.y = scene->mMeshes[0]->mVertices[0].y;
			max.z = scene->mMeshes[0]->mVertices[0].z;
		}

		// --------------------------------------
		ProcessNode(scene->mRootNode, scene, &min, &max);

		//Set Base Info --------------------
		base_info.total_meshes = meshes.size();
		for (uint i = 0; i < base_info.total_meshes; i++)
		{
			base_info.total_vertex += meshes[i].vertices.size();
			base_info.total_faces += meshes[i].numFaces;
		}

		/*Set the base info of the Root Node*/
		aiQuaternion rot_quat;
		aiVector3D pos;
		aiVector3D rot;
		aiVector3D scal;

		scene->mRootNode->mTransformation.Decompose(scal, rot_quat, pos);
		rot = rot_quat.GetEuler();

		base_info.position.Set(pos.x, pos.y, pos.z);
		base_info.rotation.Set(rot.x, rot.y, rot.z);
		base_info.scale.Set(scal.x, scal.y, scal.z);

		/*Set Bounding Box Min & Max Vertex*/
		bounding_box.minPoint = min;
		bounding_box.maxPoint = max;

		/*Set path and name of the model*/
		SetName(path);

		//-----------------------------------

		aiReleaseImport(scene);
		loaded_success = true;
	}

	else
	{
		LOG("ASSIMP Error: scene cannot be loaded");
		loaded_success = false;
	}
}

void _Model::ProcessNode(aiNode* node, const aiScene* scene, float3* min, float3* max)
{
	// Process all the Node's MESHES
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, min, max, node->mTransformation, node->mName.C_Str()));	
	}
	
	// Process children
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, min, max);
	}
}

Mesh _Model::ProcessMesh(aiMesh * mesh, const aiScene * scene, float3 * min, float3 * max, aiMatrix4x4 transform, const char* name)
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	std::vector<Texture> textures;
	float3 vec;
	float2 vec2;

	// SET VERTEX DATA -------------------------------
	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// Vertex Positions ------------------
		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		vertex.pos = vec;

		/*Update Bounding Box coords*/
		*min = min->Min(vec);
		*max = max->Max(vec);

		// Vertex Normals ------------------
		if (mesh->HasNormals())
		{
			vec.x = mesh->mNormals[i].x;
			vec.y = mesh->mNormals[i].y;
			vec.z = mesh->mNormals[i].z;
			vertex.norm = vec;
		}

		// Texture Coords ------------------
		if (mesh->mTextureCoords[0])
		{
			vec2.x = mesh->mTextureCoords[0][i].x;
			vec2.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec2;
		}
		else
		{
			vertex.texCoords = float2(0, 0);
		}

		/*Push Vertex into the Array*/
		vertices.push_back(vertex);
	}

	// SET INDEX DATA -----------------------------------------
	if (mesh->HasFaces())
	{
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
	}

	// SET MATERIAL DATA -----------------------------------------
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// For the moment, we can only see textures on the diffuse channel, but we can load the specular ones
		std::vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	//TRANSFORM DATA ---------------------------
	aiQuaternion rot_quat;
	aiVector3D pos, rot, scal;
	float3 pos_vec, rot_vec, scal_vec;

	transform.Decompose(scal, rot_quat, pos);
	rot = rot_quat.GetEuler();

	pos_vec.Set(pos.x, pos.y, pos.z);
	rot_vec.Set(rot.x, rot.y, rot.z);
	scal_vec.Set(scal.x, scal.y, scal.z);
	//------------------------------------------

	return Mesh(vertices, indices, textures,
		mesh->HasNormals(), mesh->mNumFaces, name, pos_vec, rot_vec, scal_vec);
}

std::vector<Texture> _Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, const char* typeName)
{
	std::vector<Texture> textures;
	bool skip = false;
	for (uint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		for (uint j = 0; j < loaded_tex.size(); j++)
		{
			if (std::strcmp(loaded_tex[j].path.c_str(), str.C_Str()) == 0)
			{
				textures.push_back(loaded_tex[j]);
				skip = true;
				break;
			}
		}

		if (skip == false)
		{
			Texture tex;
			tex.id = TextureFromFile(str.C_Str(), base_info.path);
			tex.type = typeName;
			tex.path = str.C_Str();
			textures.push_back(tex);
		}
	}

	return textures;
}

uint _Model::TextureFromFile(const char * path, const std::string&  dir)
{
	std::string filename = std::string(path);
	filename = dir + "textures/" + filename;
	return App->textures->LoadTexture(filename.c_str());
}

void _Model::SetColor(Color col)
{
	for (uint i = 0; i < meshes.size(); i++)
	{
		meshes[i].color = col;
	}
}

void _Model::SetTexture(const char * path)
{
	if (path != nullptr)
	{
		uint texture_id = App->textures->LoadTexture(path);

		//At the moment we can only load a single texture
		for (uint i = 0; i < meshes.size(); i++)
		{
			meshes[i].SetTex(texture_id);
		}
	}
}

void _Model::SetTexture(const char * path, int m, int t, int t_id)
{
	if (path != nullptr)
	{
		if (m >= 0 && m < meshes.size())
		{
			uint texture_id = App->textures->LoadTexture(path);
			
			meshes[m].SetTexture(t, texture_id);
		}
		else
		{
			LOG("SETTING TEXTURE ERROR - Mesh index is wrong: %i", m)
		}
	}
}

void _Model::SetName(const char* filepath)
{
	base_info.path = filepath;

	size_t BeginName = base_info.path.find_last_of("\\/");
	size_t EndName = base_info.path.find_last_of(".");
	base_info.name = base_info.path.substr(BeginName + 1);
	size_t namesize = base_info.name.find_last_of(".");
	
	base_info.name = base_info.name.substr(0, namesize);
}

void _Model::EnableBoundingBox(bool active)
{
	bb_active = active;
}

AABB _Model::GetBoundingBox() const
{
	return bounding_box;
}