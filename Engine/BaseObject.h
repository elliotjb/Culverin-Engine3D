#pragma once
#include "Globals.h"

enum Prim_Type
{
	P_SPHERE,
	P_CUBE,
	P_CYLINDER,
	P_ARROW,
	P_AXIS,
	P_RAY,
	P_PLANE,
	P_MESH
};

struct Geometry_Base
{
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

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
	virtual ~BaseObject();

	virtual void Init();
	void Draw();

public:
	Geometry_Base mesh;
	Prim_Type type;
};