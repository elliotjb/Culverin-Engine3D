#pragma once
#include "Resource_.h"
#include "Math/float3.h"
#include "Math/float2.h"

struct Vertex
{
	float3 pos;
	float3 norm;
	float2 texCoords;
};

class ResourceMesh : Resource
{
public:
	ResourceMesh(uint uid);
	virtual ~ResourceMesh();

	bool LoadToMemory();
	bool IsLoadedToMemory();


public:
	char* name = "MESH NAME";
	bool hasNormals = false;
	uint num_vertices = 0;
	uint num_indices = 0;
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	std::vector<float3> vertices_normals;
	//std::vector<FaceCenter> face_centers;

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
};

