#pragma once
#include "Globals.h"
#include "ImGui\imgui.h"
#include <string>
//#include "parson.c"

struct json_object_t;
typedef struct json_object_t JSON_Object;

class GameObject;

enum Comp_Type 
{	
	C_UNKNOWN = -1,
	C_TRANSFORM,
	C_MESH,
	C_MATERIAL,
	C_CAMERA
};

class Component
{
public:
	Component(Comp_Type t, GameObject* parent);
	virtual ~Component();

	virtual bool Enable();
	virtual bool Disable();
	virtual void preUpdate(float dt);
	virtual void Update(float dt);
	virtual void Draw();

	virtual void ShowInspectorInfo();

	Comp_Type GetType() const;
	bool isActive() const;

	virtual void Save(JSON_Object* object, std::string name) const;
	virtual void Load(const JSON_Object* object, std::string name);

private:
	Comp_Type type = C_UNKNOWN;
	bool active = false;


protected:
	GameObject* parent = nullptr;
	uint uid = 0;
};