#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"

#include <MathGeoLib/src/Math/float4.h>

#include <list>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	bool Start();
	bool CleanUp();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	void WindowResized(unsigned width, unsigned height);

	void SetBackgroundColor(float4 color);

	float4 GetBackgroundColor() const;
	Model* GetModel(unsigned pos) const;
	
	bool LoadModel(const char* path);
	bool AnyModelLoaded();
	bool IsSupportedPath(const std::string& modelPath);


private:
	void* context;
	float4 backgroundColor;

	unsigned vbo;
	std::vector<Model*> models;

	friend class ModuleEditor;

	const std::vector<std::string> modelTypes = { "FBX" };
};
