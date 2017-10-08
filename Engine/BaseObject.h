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
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0;
	uint num_vertices = 0;
	//We can alternate between float or float3
	float* vertices = nullptr;
	float3* vertices3 = nullptr;

	~Geometry_Base()
	{
		delete[] indices;
		delete[] vertices;
	}
};

class BaseObject
{
public:
	BaseObject(Prim_Type t);
	BaseObject(Prim_Type t, const float3 p, bool k, Color c);
	virtual ~BaseObject();

	virtual void Init();
	virtual void Init_float3();
	void Draw();

public:
	Prim_Type type = P_UNKNOWN;
	Geometry_Base mesh;
	Color color;
	float3 pos;
	bool isKynematic = false;
	bool wireFrame = false;
	bool Axis = false;
	uint id = 0;
};