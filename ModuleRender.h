#pragma once
#include "Module.h"
#include "Globals.h"

#include "Libraries/MathGeoLib/src/Math/float4.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	bool CleanUp();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	void WindowResized(unsigned width, unsigned height);

	void SetBackgroundColor(float4 color);

	float4 GetBackgroundColor() const;

private:
	void * context;
	float4 backgroundColor;

	friend class ModuleEditor;
};
