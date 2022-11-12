#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEngineCamera.h"

#include "Libraries/ImGui/imgui.h"
#include "Libraries/ImGui/imgui_impl_sdl.h"
#include "Libraries/ImGui/imgui_impl_opengl3.h"

ModuleEditor::ModuleEditor() {}

ModuleEditor::~ModuleEditor() {}

bool ModuleEditor::Init()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	return true;
}

bool ModuleEditor::Start()
{
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);

	return true;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Open")) {}
			if (ImGui::MenuItem("Save")) {}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	
	int hfov = App->engineCamera->GetHFOV();
	int vfov = App->engineCamera->GetVFOV();

	if (ImGui::Begin("Camera Settings"))
	{
		if (ImGui::SliderInt("Horizontal FOV", &hfov, 60, 120))
		{
			App->engineCamera->SetHFOV(math::DegToRad(hfov));
		}
		if (ImGui::SliderInt("Vertical FOV", &vfov, 60, 120))
		{
			App->engineCamera->SetVFOV(math::DegToRad(vfov));
		}

		ImGui::End();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
	SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(backup_current_window, backup_current_context);

	return UPDATE_CONTINUE;
}