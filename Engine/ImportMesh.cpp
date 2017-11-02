#include "ImportMesh.h"
#include "ModuleFS.h"
#include <filesystem>
#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include "GameObject.h"

ImportMesh::ImportMesh()
{
}


ImportMesh::~ImportMesh()
{
}

bool ImportMesh::Import(aiScene* scene, std::string & output_file)
{

	if (scene != nullptr && scene->HasMeshes())
	{
		scene->mRootNode;

	}


	return true;
}

bool ImportMesh::Load(const char * exported_file, Texture * resource)
{
	return false;
}

bool ImportMesh::Import(aiMesh* mesh, const char* name)
{
	bool ret = true;

	if (mesh != nullptr)
	{
		LOG("Importing Mesh %s", name);
		
		// SET VERTEX DATA -------------------------------
		num_vertices = mesh->mNumVertices;
		vertices = new float3[num_vertices];
		memcpy(vertices, mesh->mVertices, sizeof(float3) * num_vertices);
		LOG("- Imported all vertex from data, total vertex: %i", num_vertices);

		// SET INDEX DATA -----------------------------------------
		if (mesh->HasFaces())
		{
			num_indices = mesh->mNumFaces * 3;
			indices = new uint[num_indices];
			for (uint i = 0; i < mesh->mNumFaces; i++)
			{
				if (mesh->mFaces[i].mNumIndices != 3)
				{
					LOG("WARNING, Geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&indices[i * 3], mesh->mFaces[i].mIndices, sizeof(uint) * 3);
				}
			}
			LOG("- Imported all index from data, total indices: %i", num_indices);
		}
		else
		{
			LOG("- Mesh %s hasn't got Faces", mesh->mName.C_Str());
		}

		// SET NORMAL DATA -------------------------------
		if (mesh->HasNormals())
		{
			num_normals = num_vertices;
			vert_normals = new float3[num_normals];
			memcpy(vert_normals, mesh->mNormals, sizeof(float3) * num_normals);
			LOG("- Imported all Normals from data");
		}
		else
		{
			num_normals = 0;
			LOG("- Mesh %s hasn't got Normals", mesh->mName.C_Str());
		}

		// SET TEX COORD DATA -------------------------------
		//num_tex_coords = num_vertices;
		tex_coords = new float2[num_vertices];
		if (mesh->mTextureCoords[0])
		{
			for (uint i = 0; i < num_vertices; i++)
			{
				tex_coords[i].x = mesh->mTextureCoords[0][i].x;
				tex_coords[i].y = mesh->mTextureCoords[0][i].y;
			}
			//memcpy(tex_coords, mesh->mTextureCoords[0], sizeof(float2) * num_tex_coords);
			LOG("- Imported all Tex Coords from data");
		}
		else
		{
			for (uint i = 0; i < num_vertices; i++)
			{
				tex_coords[i] = float2(0, 0);
			}

			LOG("- Mesh %s hasn't got Tex Coords", mesh->mName.C_Str());
		}
		LOG("Imported all data");
	}
	else
	{
		LOG("Can't Import Mesh");
		ret = false;
	}

	
	//// SET MATERIAL DATA -----------------------------------------
	//if (mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial* mat = mesh->mMaterials[mesh->mMaterialIndex];
	//	std::vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//	// For the moment, we can only see textures on the diffuse channel, but we can load the specular ones
	//	std::vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//}
	
	//TRANSFORM DATA ---------------------------
	//aiQuaternion rot_quat;
	//aiVector3D pos, rot, scal;
	//float3 pos_vec, rot_vec, scal_vec;

	//transform.Decompose(scal, rot_quat, pos);
	//rot = rot_quat.GetEuler();

	//pos_vec.Set(pos.x, pos.y, pos.z);
	//rot_vec.Set(rot.x, rot.y, rot.z);
	//scal_vec.Set(scal.x, scal.y, scal.z);
	//------------------------------------------
	

	// ALLOCATING DATA INTO BUFFER ------------------------
	uint ranges[3] = { num_vertices, num_indices, num_normals }; //,num_tex_coords };

	uint size = sizeof(ranges) + sizeof(float3) *  num_vertices + sizeof(uint) * num_indices + sizeof(float3) *  num_normals + sizeof(float2) *  num_vertices;

	// Allocating all data 
	char* data = new char[size];
	char* cursor = data;

	// Storing Ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	// Storing Vertices
	cursor += bytes;
	bytes = sizeof(float3) * num_vertices;
	memcpy(cursor, vertices, bytes);

	// Storing Indices
	cursor += bytes;
	bytes = sizeof(uint) * num_indices;
	memcpy(cursor, indices, bytes);

	// Storing Normals
	cursor += bytes;
	bytes = sizeof(float3) * num_normals;
	memcpy(cursor, vert_normals, bytes);

	// Storing Tex Coords
	cursor += bytes;
	bytes = sizeof(float2) * num_vertices; //num_tex_coords;
	memcpy(cursor, tex_coords, bytes);

	//// Release all pointers
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vert_normals);
	RELEASE_ARRAY(tex_coords);

	std::string fileName = name;
	fileName += ".rin";

	//Save Mesh
	App->fs->SaveFile(data, fileName, size, IMPORT_DIRECTORY_LIBRARY_MESHES);

	RELEASE_ARRAY(data);


	return ret;
}


bool ImportMesh::Load(const char* file)
{

	char* buffer = nullptr;

	// Loading File
	uint size = App->fs->LoadFile(file, &buffer);

	if (buffer != nullptr && size > 0)
	{
		char* cursor = buffer;

		// Amount vertices, amount indices, amount normals
		uint ranges[3];
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		// Set Amounts
		num_vertices = ranges[0];
		num_indices = ranges[1];
		num_normals = ranges[2];
		//num_tex_coords = ranges[3];
		
		//Load Vertices
		cursor += bytes;
		bytes = sizeof(float3) * num_vertices;
		vertices = new float3[num_vertices];
		memcpy(vertices, cursor, bytes);
		
		//Load Indices
		cursor += bytes;
		bytes = sizeof(uint) * num_indices;
		indices = new uint[num_indices];
		memcpy(indices, cursor, bytes);

		//Load Normals
		cursor += bytes;
		bytes = sizeof(float3) * num_normals;
		vert_normals = new float3[num_normals];
		memcpy(vert_normals, cursor, bytes);

		//Load Tex Coords
		cursor += bytes;
		bytes = sizeof(float2) * num_vertices; //num_tex_coords;
		tex_coords = new float2[num_vertices];
		memcpy(tex_coords, cursor, bytes);
		
		GameObject* gameobject = new GameObject();
		CompMesh* mesh = (CompMesh*)gameobject->AddComponent(C_MESH);
		
		mesh->InitRanges(num_vertices, num_indices, num_normals);
		mesh->Init(vertices, indices, vert_normals, tex_coords);
		mesh->SetupMesh();
		mesh->Enable();
		mesh->SetDirecotryMesh(file);

		App->scene->gameobjects.push_back(gameobject);
		
	}

	return true;
}