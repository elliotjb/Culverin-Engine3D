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

struct FaceCenter
{
	float3 pos;
	float3 norm;
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
	Mesh(std::vector<Vertex> vertices, std::vector<uint> indices, std::vector<Texture> textures, bool normals, uint faces, const char* name, float3 pos, float3 rot, float3 scale);
	~Mesh();

	void Clear();
	void ClearTex();
	void ClearTexture(int t);

	void Draw();
	void SetupMesh();
	void SetTex(uint texID); /*Same texture to all textures structs*/
	void SetTexture(int t_index, uint texID); /*Only change one texture*/

public:
	float3 pos;
	float3 rot;
	float3 scale;
	Color color;

	std::string name;
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	std::vector<Texture> textures;
	std::vector<FaceCenter> face_centers;
	std::vector<float3> vertices_normals;

	bool hasNormals = false;
	uint numFaces = 0;

private:

	uint VAO = 0; /*Vertex Array Object*/
	uint vertices_id = 0; /*VERTICES ID*/
	uint indices_id = 0; /*INDICES ID*/
	uint vertices_norm_id = 0; /*NORMALS OF VERTICES ID*/
};

class BaseGeometry
{
public:
	BaseGeometry(Prim_Type t, bool w);
	BaseGeometry(Prim_Type t, const float3 p, bool k, Color c, bool w);
	virtual ~BaseGeometry();

	virtual void Draw();
	virtual void Init();
	virtual void Clear();
	virtual void ClearTextures();
	virtual void GenFaceNormals(float3* centers);

	void Enable(bool active);
	bool SetWireframe(bool w);
	bool isActive() const;

public:
	Prim_Type type = P_UNKNOWN;
	Color color;
	float3 pos;
	bool isKynematic = false;
	bool wireframe = false;
	bool Axis = false;
	uint id = 0;

	bool active = true;
};