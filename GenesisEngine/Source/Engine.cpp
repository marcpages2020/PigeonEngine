#include "Engine.h"
#include "Time.h"

GnEngine::GnEngine() : versionMajor(0), versionMinor(1), maxFPS(120)
{
	SetMaxFPS(maxFPS);

	hardware = new ModuleHardware(this);
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(hardware);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(editor);

	// Renderer last!
	AddModule(renderer3D);
}

GnEngine::~GnEngine()
{
	std::vector<Module*>::reverse_iterator item = modulesVector.rbegin();

	while(item != modulesVector.rend())
	{
		delete *item;
		++item ;
	}
}

bool GnEngine::Init()
{
	bool ret = true;
	Time::Init();
	Time::realClock.deltaTimer.Start();

	// Call Init() in all modules
	for (int i = 0; i < modulesVector.size() && ret == true; i++)
	{
		ret = modulesVector[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (int i = 0; i < modulesVector.size() && ret == true; i++)
	{
		ret = modulesVector[i]->Start();
	}
	
	return ret;
}

// ---------------------------------------------
void GnEngine::PrepareUpdate()
{
	deltaTime = (float)Time::realClock.deltaTimer.Read() / 1000;

	Time::realClock.Step();
	Time::gameClock.Step();
}

// ---------------------------------------------
void GnEngine::FinishUpdate()
{
	Uint32 last_frame_ms = Time::realClock.deltaTimer.Read();
	if (last_frame_ms < minFrameMS)
	{
		SDL_Delay(minFrameMS - last_frame_ms);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status GnEngine::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (int i = 0; i < modulesVector.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = modulesVector[i]->PreUpdate(deltaTime);
	}

	for (int i = 0; i < modulesVector.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = modulesVector[i]->Update(deltaTime);
	}

	for (int i = 0; i < modulesVector.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = modulesVector[i]->PostUpdate(deltaTime);
	}

	FinishUpdate();
	return ret;
}

bool GnEngine::CleanUp()
{
	bool ret = true;
	for (int i = modulesVector.size() -1; i > 0; i--)
	{
		modulesVector[i]->CleanUp();
	}

	return ret;
}

void GnEngine::AddModule(Module* mod)
{
	modulesVector.push_back(mod);
}

int GnEngine::GetMaxFPS() const
{
	return maxFPS;
}

void GnEngine::SetMaxFPS(int newMaxFPS)
{
	maxFPS = newMaxFPS;
	minFrameMS = 1000.0f / maxFPS;
}

float GnEngine::GetLastDeltaTime() const
{
	return deltaTime;
}

float GnEngine::GetCurrentFPS() const
{
	return 1.0f / deltaTime;
}
