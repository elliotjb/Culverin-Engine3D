#pragma once
#include "Component.h"
#include "Math/float3.h"
#include "Math/float2.h"
#include <vector>

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


class CompMesh: public Component
{
public:
	CompMesh(Comp_Type t);
	~CompMesh();

	void InitRanges(uint num_vertices, uint num_indices, uint num_normals);
	void Init(const float3* vertices, const uint* indices, const float3* vert_normals, const float2* texCoord);
	void SetupMesh();
	
	void ShowInspectorInfo();
	void Draw();
	void Update();

public:
	char* name = "MESH NAME";
	bool hasNormals = false;
	uint num_vertices = 0;
	uint num_indices = 0;
	std::vector<_Vertex> vertices;
	std::vector<uint> indices;
	std::vector<float3> vertices_normals;
	//std::vector<_FaceCenter> face_centers;

private:
	uint VAO = 0; /*Vertex Array Object*/
	uint vertices_id = 0; /*VERTICES ID*/
	uint indices_id = 0; /*INDICES ID*/
	uint vertices_norm_id = 0; /*NORMALS OF VERTICES ID*/
};