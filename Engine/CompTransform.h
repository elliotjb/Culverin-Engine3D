#pragma once
#include "Component.h"
#include "MathGeoLib.h"
#include <vector>
#include "Assimp/include/matrix4x4.h"

class GameObject;

struct Axis
{
	float3 x = { 1, 0, 0 };
	float3 y = { 0, 1, 0 };
	float3 z = { 0, 0, 1 };
};

class CompTransform :public Component
{
public:
	CompTransform(Comp_Type t, GameObject* parent);
	~CompTransform();

	void Init(float3 p, float3 r, float3 s);
	void Update();
	void ShowInspectorInfo();

	void SetPos(float3 pos);
	void SetRot(float3 rot);
	void SetScale(float3 scale);
	void SetZero();
	void SetTransformation(aiMatrix4x4 tranformation);
	void SetLocalTransform();
	void SetGlobalTransform(float4x4 transform);

	void UpdateMatrix();

	float3 GetPos() const;
	float3 GetRot() const;
	float3 GetScale() const;
	float4x4 GetLocalTransform() const;
	float4x4 GetGlobalTransform() const;
	float4x4 GetInheritedTransform() const;
	const float* GetMultMatrixForOpenGL() const;

	void Save(JSON_Object* object, std::string name) const;
	void Load(const JSON_Object* object, std::string name);

private:
	Axis axis;

	// Output Values -----------------
	float3 position = { 0, 0, 0 };
	float3 rotation = { 0, 0, 0 };
	float3 scale = { 0, 0, 0 };
	Quat rotation_new;
	// -------------------------------

	float3 rot_angle = { 0, 0, 0 };
	Quat rot_quat = { 1, 0, 0, 0 };

	float4x4 local_transform;
	float4x4 global_transform;

	float4x4 inherited_transform;
	float3 inherited_pos = { 0, 0, 0 };
};