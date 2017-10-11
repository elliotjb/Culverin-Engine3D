#include "_Model.h"
#include "Globals.h"
#include "Application.h"
#include "BaseGeometry.h"
#include "ModuleTextures.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

_Model::_Model(const char * path, Prim_Type type, bool wire) :BaseGeometry(type, wire)
{
	LoadModel(path);
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
}

void _Model::Clear()
{
	
	//CLEAR MESHES OF THE MODEL
	for (uint i = 0; i < meshes.size(); i++)
	{
		meshes[i].vertices.clear();
		meshes[i].indices.clear();
		meshes[i].textures.clear();
		meshes[i].face_centers.clear();
	}

	loaded_tex.clear();
}

void _Model::LoadModel(const char * path)
{
	bool ret = false;
	aiMesh* new_mesh = nullptr;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		ProcessNode(scene->mRootNode, scene);
		aiReleaseImport(scene);
	}

	else
	{
		LOG("ASSIMP Error: scene %s cannot be loaded");
	}
}

void _Model::ProcessNode(aiNode * node, const aiScene * scene)
{
	// Process all the Node's MESHES
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// Process children
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh _Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
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

		vertices.push_back(vertex); /*Push Vertex into the Array*/
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
		std::vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	
	return Mesh(vertices, indices, textures, mesh->HasNormals());
}

std::vector<Texture> _Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, const char * typeName)
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
			tex.id = TextureFromFile(str.C_Str(), this->path);
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
