#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleFramebuffers.h"
#include "CompMesh.h"
#include "Quadtree.h"
#include <vector>

class GameObject;
class SkyBox;

class Scene : public Module
{
public:
	Scene(bool start_enabled = false);
	virtual ~Scene();

	//bool Init(JSON_Object* node);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	//update_status PostUpdate(float dt);
	update_status UpdateConfig(float dt);
	bool CleanUp();

	void DrawPlane();
	void DrawCube(float size);

	void FillStaticObjectsVector();

	//OBJECTS CREATION / DELETION ---------------------
	void Init_IndexVertex(float3* vertex_triangulate, uint num_index, CompMesh* mesh);
	GameObject* CreateGameObject(GameObject* parent = nullptr);
	GameObject* CreateCube(GameObject* parent = nullptr);
	GameObject* CreateSphere(GameObject* parent = nullptr);
	GameObject* CreateMainCamera(GameObject* parent = nullptr);
	void DeleteGameObjects(std::vector<GameObject*>& gameobjects, bool isMain = true);
	void DeleteGameObject(GameObject* gameobjects);
	// -------------------------------------

public:
	//GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	//_Cube* test = nullptr;

	//GLuint tex;
	//GLuint Lenna;

	//Drawing Scene & Game inside a Dock
	FrameBuffer* sceneBuff = nullptr;
	FrameBuffer* gameBuff = nullptr;

	//Container Vector of all Game Objects
	std::vector<GameObject*> gameobjects;

	//Container Vector of Static Objects
	std::vector<GameObject*> static_objects;

	Quadtree quadtree;
	bool quadtree_draw = false;
	GameObject* drag = nullptr;
	GLuint icon_options_transform;
	SkyBox* skybox = nullptr;

private:
	int size_plane = 0;
	float size_quadtree = 0;

};