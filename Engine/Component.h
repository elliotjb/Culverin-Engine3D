#pragma once
#include "Globals.h"

enum Comp_Type 
{	
	C_UNKNOWN = -1,
	C_TRANSFORM,
	C_MESH,
	C_MATERIAL
};

class Component
{
public:
	Component(Comp_Type t);
	virtual ~Component();

	virtual bool Enable();
	virtual bool Disable();
	virtual void Update();

	Comp_Type GetType() const;
	bool isActive() const;

private:
	Comp_Type type = C_UNKNOWN;
	bool active = false;
};