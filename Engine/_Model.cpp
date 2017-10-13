#include "_Model.h"
#include "Globals.h"
#include "Application.h"
#include "BaseGeometry.h"
#include "ModuleTextures.h"
#include "ModuleInspector.h"
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

	if (App->renderer3D->bounding_box)
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
		info.total_meshes = meshes.size();
		for (uint i = 0; i < info.total_meshes; i++)
		{
			info.total_vertex += meshes[i].vertices.size();
			info.total_faces += meshes[i].numFaces;
		}

		aiQuaternion rot_quat;
		aiVector3D pos;
		aiVector3D rot;
		aiVector3D scal;

		scene->mRootNode->mTransformation.Decompose(scal, rot_quat, pos);
		rot = rot_quat.GetEuler();

		info.position.Set(pos.x, pos.y, pos.z);
		info.rotation.Set(rot.x, rot.y, rot.z);
		info.scale.Set(scal.x, scal.y, scal.z);

		/*Set Bounding Box Min & Max Vertex*/
		bounding_box.minPoint = min;
		bounding_box.maxPoint = max;
		//-----------------------------------

		aiReleaseImport(scene);
	}

	else
	{
		LOG("ASSIMP Error: scene %s cannot be loaded");
	}
}

void _Model::ProcessNode(aiNode * node, const aiScene * scene, float3 * min, float3 * max)
{
	// Process all the Node's MESHES
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, min, max));
	}

	// Process children
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, min, max);
	}
}

Mesh _Model::ProcessMesh(aiMesh * mesh, const aiScene * scene, float3 * min, float3 * max)
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
		std::vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures, mesh->HasNormals(), mesh->mNumFaces);
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

void _Model::SetTexture(const char * path)
{
	if (path != nullptr)
	{
		//At the moment we can only load a single texture
		for (uint i = 0; i < meshes.size(); i++)
		{
			meshes[i].textures[0].id = App->textures->LoadTexture(path);
		}
		((Inspector*)App->gui->winManager[INSPECTOR])->SetTexInfo(meshes[0].textures[0].id);
	}
}

void _Model::SetBoundingBox()
{
	bounding_box;
}

uint _Model::GetTotalMeshes() const
{
	return info.total_meshes;
}

uint _Model::GetTotalVertex() const
{
	return info.total_vertex;
}

uint _Model::GetTotalFaces() const
{
	return info.total_faces;
}

uint _Model::GetTexID() const
{
	/* Only one texture loaded by now*/ // TODO -< MODIFY THIS!!! URGENT!!!!
	return meshes[0].textures[0].id;
}

float3 _Model::GetPosition() const
{
	return info.position;
}

float3 _Model::GetRotation() const
{
	return info.rotation;
}

float3 _Model::GetScale() const
{
	return info.scale;
}
