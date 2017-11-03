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
	~GameObject();

	void preUpdate();
	void Update();
	bool Enable();
	bool Disable();

	void SetName(char* name);
	const char* GetName() const;

	void ShowHierarchy();
	void ShowInspectorInfo();

	bool isActive() const;
	bool isVisible() const;

	// Componenets
	Component* FindComponentByType(Comp_Type type) const;
	Component* AddComponent(Comp_Type type);
	void AddComponent_(Comp_Type type);
	int GetNumComponents() const;
	CompTransform* GetComponentTransform() const;
	void SaveComponents(JSON_Object* object, std::string name) const;
	void LoadComponents(const JSON_Object * object, std::string name, uint numComponents);

	// Childs
	uint GetNumChilds() const;
	GameObject* GetChildbyIndex(uint pos_inVec) const;
	void AddChildGameObject(GameObject* child);

	GameObject* GetParent() const;

	AABB* bounding_box = nullptr;
	void DrawBoundingBox();

	uint GetUUID() const;

private:
	char* name = "CHANGE THIS";
	bool active = false;

	std::vector<Component*> components;
	std::vector<GameObject*> childs;
	
	GameObject* parent = nullptr;

	bool bb_active = false;
	uint uid = 0;
};