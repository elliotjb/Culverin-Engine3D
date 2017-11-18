#pragma once
#include "Component.h"
#include "Math/float3.h"
#include "Math/float2.h"
#include <vector>

class ResourceMesh;
class GameObject;
class CompMaterial;
struct Vertex;

struct FaceCenter
{
	float3 pos;
	float3 norm;
};

enum Type_Primitive
{
	TYPE_NON,
	TYPE_SPHERE,
	TYPE_CUBE
};


class CompMesh: public Component
{
public:
	CompMesh(Comp_Type t, GameObject* parent);
	CompMesh(const CompMesh& copy, GameObject* parent);
	~CompMesh();
	
	void ShowOptions();
	void ShowInspectorInfo();
	void Draw();
	void preUpdate(float dt);
	void Update(float dt);
	void Render(bool render);
	bool isRendering() const;

	void LinkMaterial(CompMaterial* mat);
	void SetResource(ResourceMesh* resourse_mesh);

	void Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;
	void Load(const JSON_Object* object, std::string name);

public:
	char* name = "MESH NAME";
	bool hasNormals = false;

	ResourceMesh* resourceMesh = nullptr;

private:
	bool render = true;
	bool SelectMesh = false;
	CompMaterial* material = nullptr;

};