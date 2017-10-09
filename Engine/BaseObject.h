#pragma once
#include "Globals.h"
#include "MathGeoLib.h"
#include "Color.h"
#include <string>
#include <vector>

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
	P_MODEL,
};

struct Vertex
{
	float3 pos;
	float3 norm;
	float2 texCoords;
};

struct Texture
{
	uint id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<uint> indices, std::vector<Texture> textures);
	~Mesh();

	void Draw();
	void SetupMesh();

public:
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	std::vector<Texture> textures;

private:

	uint VAO = 0; /*Vertex Array Object*/
	uint vertices_id = 0; /*VERTICES ID*/
	uint indices_id = 0; /*INDICES ID*/
};

/*struct Geometry_Base
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

	//NORMALS COORDS --------------
	uint id_normals = 0;
	float* normals = nullptr;

	~Geometry_Base()
	{
		delete[] indices;
		delete[] vertices;
		delete[] vertices3;
		delete[] tex_coords;
	}
};*/

class BaseObject
{
public:
	BaseObject(Prim_Type t, bool w);
	BaseObject(Prim_Type t, const float3 p, bool k, Color c, bool w);
	virtual ~BaseObject();

	virtual void Draw();
	virtual void Init();

	bool SetWireframe(bool w);

public:
	Prim_Type type = P_UNKNOWN;
	Color color;
	float3 pos;
	bool isKynematic = false;
	bool wireframe = false;
	bool Axis = false;
	uint id = 0;
};