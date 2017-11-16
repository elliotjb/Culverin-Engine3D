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

class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint uid);
	virtual ~ResourceMesh();

	void Init(const float3* vert, const uint* ind, const float3* vert_normals, const float2* texCoord);
	void InitRanges(uint num_vert, uint num_ind, uint num_normals);
	void InitInfo(uint uuid, const char* name);

	bool LoadToMemory();
	bool IsLoadedToMemory();

public:
	bool hasNormals = false;
	uint num_vertices = 0;
	uint num_indices = 0;
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	std::vector<float3> vertices_normals;
	//std::vector<FaceCenter> face_centers;

	uint VAO = 0;				/* Vertex Array Object */
	uint vertices_id = 0;		/* VERTICES ID */
	uint indices_id = 0;		/* INDICES ID */
	uint vertices_norm_id = 0;	/* NORMALS OF VERTICES ID */

	const char* directory_mesh = nullptr; //Only use in save and load

private:
	bool isLoaded = false;

};

