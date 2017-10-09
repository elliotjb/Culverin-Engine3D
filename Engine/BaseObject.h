#pragma once
#include "Globals.h"
#include "MathGeoLib.h"
#include "Color.h"

enum Prim_Type
{
	P_UNKNOWN = -1,
	P_SPHERE,
	P_CUBE,
	P_CYLINDER,
	P_ARROW,
	P_AXIS,
	P_RAY,
	P_PLANE,
	P_MESH,
};

struct Geometry_Base
{
	// INDICES -----------------
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	// VERTICES ---------------------
	uint id_vertices = 0;
	uint num_vertices = 0;
	//We can alternate between float or float3
	float* vertices = nullptr;
	float3* vertices3 = nullptr;

	// TEXTUTE COORDS -----------------
	uint id_image = 0;
	uint id_texture = 0;
	float* tex_coords = nullptr;
	char* tex_path = nullptr;

	~Geometry_Base()
	{
		delete[] indices;
		delete[] vertices;
		delete[] vertices3;
		delete[] tex_coords;
	}
};

class BaseObject
{
public:
	BaseObject(Prim_Type t, bool w);
	BaseObject(Prim_Type t, const float3 p, bool k, Color c, bool w);
	virtual ~BaseObject();

	void Init();
	void Draw();

	bool SetWireframe(bool w);

public:
	Prim_Type type = P_UNKNOWN;
	Geometry_Base mesh;
	Color color;
	float3 pos;
	bool isKynematic = false;
	bool wireframe = false;
	bool Axis = false;
	uint id = 0;
};