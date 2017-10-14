#pragma once
#include "BaseGeometry.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include <vector>
#include <string>

struct ModelInfo
{
	//TRANSFORM INFO
	float3 position;
	float3 rotation;
	float3 scale;

	//GEOMETRY INFO
	uint total_meshes = 0;
	uint total_vertex = 0;
	uint total_faces = 0;

	//FILE & NAME INFO
	std::string path;
	std::string name;
};

class _Model :public BaseGeometry
{
public:
	_Model(const char* path, Prim_Type type, bool wire);
	~_Model();

	void Draw();
	void Clear();
	void ClearTextures();
	void ClearTexture(int m, int t, int t_id);

	void LoadModel(const char* path);
	void ProcessNode(aiNode* node, const aiScene* scene, float3* min, float3* max);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, float3* min, float3* max);
	
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const char *typeName);
	uint TextureFromFile(const char *path, const std::string& dir);
	
	void SetTexture(const char *path);/*Set the same texture to all meshes*/
	void SetTexture(const char *path, int m, int t, int t_id);/*Set specific texture to specific mesh*/
	void SetName(const char *path);

	void EnableBoundingBox(bool active);
	AABB GetBoundingBox() const;

	ModelInfo base_info;
	std::vector<Mesh> meshes;
	
	/*Don't load the same texture more than once*/
	std::vector<Texture> loaded_tex;

private:
	AABB bounding_box;
	bool bb_active = false;
};