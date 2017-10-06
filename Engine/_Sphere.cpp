#include "_Sphere.h"

_Sphere::_Sphere(Prim_Type t, const float3 p, const float r, bool k, Color c, uint def) :BaseObject(t, p, k, c)
{
	sphere = new Sphere(p, r);
	radius = r;
	SetDefinition(def);
	
	vertex_array.reserve(def);
	sphere->Triangulate(vertex_array.data(), NULL, NULL, mesh.num_vertices, false);
}

_Sphere::~_Sphere()
{
}

void _Sphere::SetDefinition(uint def)
{
	switch (def)
	{
	case 1:
		mesh.num_vertices = 24;
		break;
	case 2:
		mesh.num_vertices = 96;
		break;
	case 3:
		mesh.num_vertices = 384;
		break;
	case 4:
		mesh.num_vertices = 1536;
		break;
	case 5:
		mesh.num_vertices = 6144;
		break;
	default:
		break;
	}
}