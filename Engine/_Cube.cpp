#include "_Cube.h"
#include "BaseGeometry.h"
#include "MathGeoLib.h"
#include "GL3W\include\glew.h""
#include <vector>

_Cube::_Cube(Prim_Type t, const float3 p, const float3 s, bool k, Color c, bool w) :BaseGeometry(t, p, k, c, w)
{
	size = s;

	boxed = new OBB();
	boxed->pos = pos;
	boxed->r = size;
	boxed->axis[0] = float3(1, 0, 0);
	boxed->axis[1] = float3(0, 1, 0);
	boxed->axis[2] = float3(0, 0, 1);

	AABB boxer(*boxed);
	box = &boxer;

	vertex_array.reserve(36);
	//face_centers.reserve(6);
	box->Triangulate(1, 1, 1, vertex_array.data(), NULL, NULL, false);
}

_Cube::~_Cube()
{
}

void _Cube::Init()
{
	cube_mesh.SetupMesh();
}

void _Cube::Draw()
{
	glColor4f(color.r, color.g, color.b, color.a);
	cube_mesh.Draw();
}

void _Cube::GenFaceNormals(float3* centers)
{
	if (box != nullptr)
	{
		for (int i = 0; i < box->NumFaces(); i++)
		{
			FaceCenter center;
			center.pos = centers[i];
			center.norm = box->FaceNormal(i);

			cube_mesh.face_centers.push_back(center);
		}
	}
}

