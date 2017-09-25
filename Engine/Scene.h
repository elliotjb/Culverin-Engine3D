#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "imgui.h"
#include "MathGeoLib.h"


class Scene : public Module
{
public:
	Scene(bool start_enabled = false);
	virtual ~Scene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:



};