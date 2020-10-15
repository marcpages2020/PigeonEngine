#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "parson/parson.h"
#include "Mesh.h"

//#include "glew/include/glew.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	name = "scene";
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));

	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

bool ModuleSceneIntro::Init(JSON_Object* object)
{
	show_grid = json_object_get_boolean(object, "show_grid");
	return true;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	bool wired = (App->renderer3D->GetDisplayMode() == WIREFRAME);

	if (show_grid) 
	{
		Grid grid(10);
		grid.Render();
	}

	//Cube cube;
	//cube.Render();

	//Pyramid pyramid;
	//pyramid.Render();

	//Plane plane;
	//plane.Render();

	//Sphere sphere;
	//sphere.Render();

	//Cylinder cylinder(1, 2, 32);
	//cylinder.Render();

	Cone cone(1, 1.5, 12);
	cone.Render();

	return UPDATE_CONTINUE;
}


