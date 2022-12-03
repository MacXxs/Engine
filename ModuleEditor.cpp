#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEngineCamera.h"

#include "Libraries/ImGui/imgui.h"
#include "Libraries/ImGui/imgui_impl_sdl.h"
#include "Libraries/ImGui/imgui_impl_opengl3.h"

static bool cameraOpened = false;
static bool meshOpened = false;

ModuleEditor::ModuleEditor() {}

ModuleEditor::~ModuleEditor() {}

bool ModuleEditor::Init()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

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
	ImGui_ImplOpenGL3_Shutdown();
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
		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Camera")) cameraOpened = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Mesh")) meshOpened = true;

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (cameraOpened)
	{
		if (ImGui::Begin("Camera Settings", &cameraOpened, ImGuiWindowFlags_AlwaysAutoResize))
		{
			int hfov = App->engineCamera->GetHFOV();
			int vfov = App->engineCamera->GetVFOV();
			float znear = App->engineCamera->GetZNear();
			float zfar = App->engineCamera->GetZFar();

			if (ImGui::CollapsingHeader("Frustum", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::SliderInt("Horizontal FOV", &hfov, MIN_HFOV, MAX_HFOV))
					App->engineCamera->SetHFOV(math::DegToRad(hfov));
				if (ImGui::SliderInt("Vertical FOV", &vfov, MIN_VFOV, MAX_VFOV))
					App->engineCamera->SetVFOV(math::DegToRad(vfov));

				if (ImGui::InputFloat("Z near", &znear, 0.5f, 0.0f))
					App->engineCamera->SetPlaneDistance(znear, zfar);
				if (ImGui::InputFloat("Z far", &zfar, 0.5f, 0.0f))
					App->engineCamera->SetPlaneDistance(znear, zfar);
			}

			float movementSpeed = App->engineCamera->GetMoveSpeed();
			float rotationSpeed = App->engineCamera->GetRotationSpeed();

			if (ImGui::CollapsingHeader("Movement", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::SliderFloat("Movement Speed", &movementSpeed, 
										DEFAULT_MOVE_SPEED, DEFAULT_MOVE_SPEED * 10.f))
					App->engineCamera->SetMoveSpeed(movementSpeed);
				if (ImGui::SliderFloat("Rotation Speed (keyboard)", &rotationSpeed, 
										DEFAULT_ROTATION_SPEED, DEFAULT_ROTATION_SPEED * 10.f))
					App->engineCamera->SetRotationSpeed(rotationSpeed);
			}

			static float4 color = App->renderer->GetBackgroundColor();

			if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("Background Color");
				if (ImGui::ColorEdit3("MyColor##1", (float*)&color))
					App->renderer->SetBackgroundColor(color);
			}

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}

	if (meshOpened)
	{
		if (ImGui::Begin("Mesh Information", &meshOpened))
		{
			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
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
