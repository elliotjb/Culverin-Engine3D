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
	CompMesh(Comp_Type t, std::vector<_Vertex> vertices, std::vector<uint> indices);
	~CompMesh();

	void Draw();
	void SetupMesh();


public:
	std::vector<_Vertex> vertices;
	std::vector<uint> indices;
	std::vector<_FaceCenter> face_centers;
	std::vector<float3> vertices_normals;

private:
	bool hasNormals = false;
	uint VAO = 0; /*Vertex Array Object*/
	uint vertices_id = 0; /*VERTICES ID*/
	uint indices_id = 0; /*INDICES ID*/
	uint vertices_norm_id = 0; /*NORMALS OF VERTICES ID*/
};