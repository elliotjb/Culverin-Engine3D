#pragma once
#include "BaseGeometry.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include <vector>
#include <string>

class _Model :public BaseGeometry
{
public:
	_Model(const char* path, Prim_Type type, bool wire);
	~_Model();

	void Draw();
	void Clear();

	void LoadModel(const char* path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const char *typeName);
	uint TextureFromFile(const char *path, const std::string& dir);

	uint GetTotalMeshes() const;
	uint GetTotalVertex() const;
	uint GetTotalFaces() const;
	uint GetTexID() const;

	float3 GetPosition() const;
	float3 GetRotation() const;
	float3 GetSize() const;

private:
	std::string path;
	std::vector<Texture> loaded_tex;
	std::vector<Mesh> meshes;

	//TRANSFORM INFO
	float3 position;
	float3 rotation;
	float3 size;

	//GEOMETRY INFO
	uint total_meshes = 0;
	uint total_vertex = 0;
	uint total_faces = 0;

};