#pragma once
#include "Math\float3.h"
#include "Globals.h"

enum SkyBoxSide
{
	SKY_LEFT = 0,
	SKY_BACK,
	SKY_RIGHT,
	SKY_FRONT,
	SKY_TOP,
	SKY_BOTTOM
};

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

