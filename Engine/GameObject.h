#pragma once
#include "Globals.h"
#include "Component.h"
#include "MathGeoLib.h"
#include <string>
#include <vector>

class CompTransform;

class GameObject
{
public:
	GameObject();
	GameObject(char* nameGameObject, uint uuid);
	GameObject(const GameObject& copy);
	~GameObject();

	void preUpdate(float dt);
	void Update(float dt);
	void Draw();
	bool Enable();
	bool Disable();

	void SetName(char* name);
	const char* GetName() const;

	void ShowHierarchy();
	void ShowInspectorInfo();

	void SetVisible(bool visible);

	bool isActive() const;
	bool isVisible() const;
	bool isStatic() const;

	// Components -----------------------------
	Component* FindComponentByType(Comp_Type type) const;
	Component* AddComponent(Comp_Type type);
	void AddComponent_(Comp_Type type);
	int GetNumComponents() const;
	CompTransform* GetComponentTransform() const;
	CompMesh* GetComponentMesh() const;
	CompMaterial* GetComponentMaterial() const;
	Component* GetComponentbyIndex(uint i) const;
	void SaveComponents(JSON_Object* object, std::string name) const;
	void LoadComponents(const JSON_Object * object, std::string name, uint numComponents);

	// Childs ---------------------------------
	uint GetNumChilds() const;
	GameObject* GetChildbyIndex(uint pos_inVec) const;
	std::vector<GameObject*> GetChildsVec() const;
	void AddChildGameObject_Copy(GameObject* child);
	void AddChildGameObject_Load(GameObject* child);
	void AddChildGameObject_Replace(GameObject* child);

	// Tranform Modifications -----------------
	void UpdateMatrixRecursive(float4x4 transform, bool modificate);

	GameObject* GetParent() const;

	// Bounding Box -----------------------
	AABB* bounding_box = nullptr;
	AABB  box_fixed;
	void DrawBoundingBox();

	uint GetUUID() const;

private:
	char* name = "CHANGE THIS";
	bool active = false;
	bool visible = false;
	bool static_obj = false;

	std::vector<Component*> components;
	std::vector<GameObject*> childs;
	
	GameObject* parent = nullptr;

	bool bb_active = false;
	uint uid = 0;
};