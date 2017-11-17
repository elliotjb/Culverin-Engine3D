#pragma once
#include "parson.h"
#include "Globals.h"
#include <vector>

class GameObject;
class ResourceMaterial;
struct ReImport;

class JSONSerialization
{
public:
	JSONSerialization();
	~JSONSerialization();

	// SAVE & LOAD SCENE --------------------------
	void SaveScene();
	void SaveChildGameObject(JSON_Object* config_node, const GameObject& gameObject, uint& count, uint& countResources);
	void LoadScene();
	void LoadChilds(GameObject& parent, GameObject& child, int uuidParent);
	// --------------------------------------

	// SAVE & LOAD PREFAB --------------------------
	void SavePrefab(const GameObject& gameObject, const char* directory, const char* fileName);
	void SaveChildPrefab(JSON_Object* config_node, const GameObject& gameObject, uint& count, uint& countResources);
	void LoadPrefab(const char* prefab);
	void LoadChildLoadPrefab(GameObject& parent, GameObject& child, int uuidParent);
	// --------------------------------------

	// SAVE & LOAD MATERIAL --------------------------
	void SaveMaterial(const ResourceMaterial* material, const char* directory, const char* fileName);
	//void LoadMaterial(const char* Material); At the moment no need this
	// --------------------------------------

	// MESH & MATERIAL -------------------------------
	ReImport GetUUIDPrefab(const char* file);
	ReImport GetUUIDMaterial(const char* file);
	// -----------------------------------------------

private:
	// UUID & Name
	void ChangeUUIDs(GameObject& gameObject);
	void CheckChangeName(GameObject& gameObject);
	// Get All Names form Scene -------
	void GetAllNames(const std::vector<GameObject*>& gameobjects);

private:
	std::vector<const char*> namesScene;
};

