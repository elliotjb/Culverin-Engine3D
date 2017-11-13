#pragma once
#include "Math\float3.h"
#include "Globals.h"

class SkyBox
{
public:
	SkyBox();
	virtual ~SkyBox();

	void InitSkybox();
	void DrawSkybox(float size, float3 pos);
	void DeleteSkyboxTex();

private:
	uint skybox[6];
};

