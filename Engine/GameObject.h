#pragma once
#include "Globals.h"
#include "Component.h"
#include "MathGeoLib.h"
#include <string>
#include <vector>

#define Width_AddComponent 204

class CompTransform;
class CompMesh;
class CompMaterial;

class GameObject
{
public:
	GameObject(GameObject* parent);
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

	// EDITOR METHODS ------------------
	void ShowHierarchy();
	void ShowGameObjectOptions();
	void ShowInspectorInfo();
	void FreezeTransforms(bool freeze, bool change_childs);
	void ShowFreezeChildsWindow(bool freeze, bool& active);
	// ----------------------------------

	void SetVisible(bool visible);

	bool isActive() const;
	bool isVisible() const;
	bool isStatic() const;

	// Components -----------------------------
	Component* AddComponent(Comp_Type type);
	void AddComponent_(Comp_Type type);
	int GetNumComponents() const;
	Component* FindComponentByType(Comp_Type type) const;
	CompTransform* GetComponentTransform() const;
	CompMesh* GetComponentMesh() const;
	CompMaterial* GetComponentMaterial() const;
	Component* GetComponentbyIndex(uint i) const;
	void DeleteAllComponents();
	void DeleteComponent(Component* component);

	void SaveComponents(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;
	void LoadComponents(const JSON_Object * object, std::string name, uint numComponents);

	// Childs ---------------------------------
	uint GetNumChilds() const;
	GameObject* GetChildbyIndex(uint pos_inVec) const;
	GameObject* GetChildbyName(const char* name) const;
	uint GetIndexChildbyName(const char* name) const;
	void RemoveChildbyIndex(uint index);
	std::vector<GameObject*> GetChildsVec() const;
	std::vector<GameObject*>* GetChildsPtr();
	void AddChildGameObject_Copy(const GameObject* child);
	void AddChildGameObject_Load(GameObject* child);
	void AddChildGameObject_Replace(GameObject* child);

	// Transform Modifications -----------------
	void UpdateChildsMatrices();

	// Parent ----------------
	GameObject* GetParent() const;
	bool HaveParent();

	// Bounding Box -----------------------
	void AddBoundingBox(const ResourceMesh* mesh);
	AABB* bounding_box = nullptr;
	AABB  box_fixed;
	void DrawBoundingBox();

	uint GetUUID() const;
	void SetUUID(uint uuid);
	void SetUUIDRandom();

	// Delete -----------------
	bool WanttoDelete() const;
	void SettoDelete();


private:
	char* name = "CHANGE THIS";
	bool active = false;
	bool visible = false;
	bool static_obj = false;
	bool toDelete = false; 

	GameObject* parent = nullptr;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;
	
	bool bb_active = false;
	uint uid = 0;
};