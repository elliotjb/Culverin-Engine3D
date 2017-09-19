
#pragma once
#include "glmath.h"
#include "Color.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube_p : public Primitive
{
public :
	Cube_p();
	Cube_p(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
};

// ============================================
class Sphere_p : public Primitive
{
public:
	Sphere_p();
	Sphere_p(float radius);
	void InnerRender() const;
public:
	float radius;
};

// ============================================
class Cylinder_p : public Primitive
{
public:
	Cylinder_p();
	Cylinder_p(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Line_p : public Primitive
{
public:
	Line_p();
	Line_p(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class Plane_p : public Primitive
{
public:
	Plane_p();
	Plane_p(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};