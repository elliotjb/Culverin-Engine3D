#pragma once
#include "Component.h"
#include "Math/float3.h"
#include "Math/float2.h"
#include <vector>

class ResourceMesh;
class GameObject;
class CompMaterial;

struct _FaceCenter
{
	float3 pos;
	float3 norm;
};

struct _Vertex
{
	float3 pos;
	float3 norm;
	float2 texCoords;
};

enum Type_Primitive
{
	TYPE_NON,
	TYPE_SPHERE,
	TYPE_CUBE
};


class CompMesh: public Component
{
public:
	CompMesh(Comp_Type t, GameObject* parent);
	~CompMesh();

	void InitRanges(uint num_vertices, uint num_indices, uint num_normals);
	void Init(const float3* vertices, const uint* indices, const float3* vert_normals, const float2* texCoord);
	void SetupMesh();
	
	void ShowInspectorInfo();
	void Draw();
	void Update(float dt);
	void Render(bool render);
	bool isRendering() const;

	void LinkMaterial(CompMaterial* mat);
	void SetUUIDMesh(uint uuid);

	void Save(JSON_Object* object, std::string name) const;
	void Load(const JSON_Object* object, std::string name);

public:
	char* name = "MESH NAME";
	bool hasNormals = false;
	uint num_vertices = 0;
	uint num_indices = 0;
	std::vector<_Vertex> vertices;
	std::vector<uint> indices;
	std::vector<float3> vertices_normals;
	//std::vector<_FaceCenter> face_centers;

	bool isPrimitive = false;
	int TypePrimitive = 0;//0 nothing / 1-Sphere / 2-Cube / 3-etc...

private:
	bool render = true;
	uint VAO = 0; /*Vertex Array Object*/
	uint vertices_id = 0; /*VERTICES ID*/
	uint indices_id = 0; /*INDICES ID*/
	uint vertices_norm_id = 0; /*NORMALS OF VERTICES ID*/
	uint uuid_mesh = 0;

	const char* directory_mesh = nullptr; //Only use in save and load (isPrimitive dont use never!)
	//GameObject* parent = nullptr;

	CompMaterial* material = nullptr;
	ResourceMesh* resourceMesh = nullptr;
};