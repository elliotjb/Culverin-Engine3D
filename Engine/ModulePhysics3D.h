#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "Primitive.h"
#include "p2DynArray.h"
#include "parson.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

enum Direction
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

class ModuleScene;
class DebugDrawer;
struct PhysBody3D;
struct PhysVehicle3D;
struct VehicleInfo;
struct WindmillInfo;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(bool start_enabled = true);
	~ModulePhysics3D();

	bool Init(JSON_Object* node);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	update_status UpdateConfig(float dt);
	bool CleanUp();

	PhysBody3D* AddBody(const Sphere_p& sphere, float mass = 1.0f);
	PhysBody3D* AddBox(const Cube_p& cube, float mass = 1.0f);
	PhysBody3D* AddCylinder(const Cylinder_p& cylinder, float mass = 1.0f);
	PhysVehicle3D* AddVehicle(const VehicleInfo& info);

	void AddConstraintP2P(PhysBody3D* bodyA, PhysBody3D* bodyB, const vec3& anchorA, const vec3& anchorB);
	void AddConstraintHinge(PhysBody3D* bodyA, PhysBody3D* bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisS, const vec3& axisB, bool disable_collision = false);

	//Functionts for Create Map
	//PhysBody3D* CreateStraight(Cube_p& cube, float lenght,float width, float height, Direction type, bool iskinematic, ModuleScene* scene);
	//void CreateUPER(Cube_p& cube, Cube_p& cube_1, Cube_p& cube_2, float lenght, float width, float height, Direction type, ModuleScene* scene);
	//void CreateDOWNER(Cube_p& cube, Cube_p& cube_1, Cube_p& cube_2, float lenght, float width, float height, Direction type, ModuleScene* scene);
	//void CreateCurve(Cube_p& cube, Cube_p& cube_1, Cube_p& cube_2, float lenght, float width, float height, Direction type, Direction type_of_come, ModuleScene* scene);
	

	btDiscreteDynamicsWorld*			world;
	p2List<PhysVehicle3D*> vehicles;
	bool debug;

private:

	btDefaultCollisionConfiguration*	collision_conf;
	btCollisionDispatcher*				dispatcher;
	btBroadphaseInterface*				broad_phase;
	btSequentialImpulseConstraintSolver* solver;

	btDefaultVehicleRaycaster*			vehicle_raycaster;
	DebugDrawer*						debug_draw;

	p2List<btCollisionShape*> shapes;
	p2List<PhysBody3D*> bodies;
	p2List<btDefaultMotionState*> motions;
	p2List<btTypedConstraint*> constraints;

};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0,0,0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	Line_p line;
	Primitive point;
};