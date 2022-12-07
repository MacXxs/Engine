#include "Application.h"
#include "Globals.h"

#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEngineCamera.h"
#include "ModuleTexture.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <GL/glew.h>

#include <shellapi.h>

static bool cameraOpened = false;
static bool configOpened = false;
static bool consoleOpened = false;
static bool aboutOpened = false;
static bool propertiesOpened = false;

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

	ImGui::SetWindowSize("Console log", ImVec2(500, 250));

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
	update_status status = UPDATE_CONTINUE;

	//ImGui::ShowDemoWindow();

	if (ImGui::BeginMainMenuBar())
	{		 
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Configuration", NULL, &configOpened);
			ImGui::MenuItem("Camera", NULL, &cameraOpened);

			ImGui::EndMenu();
		}

		ImGui::MenuItem("Properties", NULL, &propertiesOpened);

		ImGui::MenuItem("Console", NULL, &consoleOpened);

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Github page"))
				ShellExecute(0, 0, "https://github.com/MacXxs/Engine", 0, 0, SW_SHOW);

			ImGui::MenuItem("About", NULL, &aboutOpened);

			ImGui::EndMenu();
		}
		
		if (ImGui::MenuItem("Quit")) status = UPDATE_STOP;

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
		}

		ImGui::End();
	}

	if (configOpened)
	{
		if (ImGui::Begin("Configuration", &configOpened, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
			{
				char title[25];
				int maxFPS = App->GetMaxFrameRate();

				ImGui::Text("Engine name: "); 
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), TITLE);
				
				if (ImGui::SliderInt("Max FPS", &maxFPS, 0, 100))
					App->SetMaxFrameRate(maxFPS);

				sprintf_s(title, 25, "Framerate %.1f", App->fpsLog[App->fpsLog.size() - 1]);
				ImGui::PlotHistogram("##framerate", &App->fpsLog[0], App->fpsLog.size(), 0, 
					title, 0.0f, 100.0f, ImVec2(310, 100));
				sprintf_s(title, 25, "Milliseconds %0.1f", App->msLog[App->msLog.size() - 1]);
				ImGui::PlotHistogram("##milliseconds", &App->msLog[0], App->msLog.size(), 0,
					title, 0.0f, 40.0f, ImVec2(310, 100));
			}

			if (ImGui::CollapsingHeader("Window", ImGuiTreeNodeFlags_DefaultOpen))
			{
				std::pair<int, int> windowDimensions;
				windowDimensions = App->window->GetWindowSize();

				bool fullscreen = App->window->GetFullscreen();
				bool borderless = App->window->GetBorderless();
				bool resizable = App->window->GetResizable();
				bool fullscreenDsktp = App->window->GetFulscreenDesktop();
				bool modified = false;

				float brightness = App->window->GetBrightness();

				SDL_DisplayMode DM;
				SDL_GetCurrentDisplayMode(0, &DM);

				if (ImGui::SliderInt("Width", &windowDimensions.first, 320, DM.w))
					App->window->SetWindowSize(windowDimensions.first, windowDimensions.second);
				if (ImGui::SliderInt("Height", &windowDimensions.second, 160, DM.h))
					App->window->SetWindowSize(windowDimensions.first, windowDimensions.second);

				if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
					App->window->SetBrightness(brightness);

				if (ImGui::BeginTable("WindowTable1", 2))
				{
					ImGui::TableNextColumn(); 
					if (ImGui::Checkbox("Fullscreen", &fullscreen)) modified = true;
					ImGui::TableNextColumn();
					if (ImGui::Checkbox("Borderless", &borderless)) modified = true;
					ImGui::TableNextColumn();
					if (ImGui::Checkbox("Resizable", &resizable)) modified = true;
					ImGui::TableNextColumn();
					if (ImGui::Checkbox("Full Desktop", &fullscreenDsktp)) modified = true;

					ImGui::EndTable();
				}
				
				if (modified)
				{
					App->window->SetWindowType(
						fullscreen, borderless, resizable, fullscreenDsktp
					);
				}
			}

			if (ImGui::CollapsingHeader("Hardware", ImGuiTreeNodeFlags_DefaultOpen))
			{
				SDL_version version = App->renderer->GetSDLVersion();

				ImGui::Text("SDL Version: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u.%u.%u", 
					version.major, version.minor, version.patch);
				ImGui::Text("OpenGL version: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VERSION));
				ImGui::Text("GLSL: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", 
					glGetString(GL_SHADING_LANGUAGE_VERSION));

				ImGui::Separator();

				ImGui::Text("CPUs: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i (Cache: %iKB)",
					App->renderer->GetCPUCount(), App->renderer->GetCacheLineSize());

				ImGui::Text("System RAM: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%0.2f GB", App->renderer->GetRamGB());

				ImGui::Text("Caps: "); ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
				ImGui::TextWrapped(App->renderer->GetCaps());
				ImGui::PopStyleColor(1);

				ImGui::Separator();

				ImGui::Text("GPU: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VENDOR));
				ImGui::Text("Brand: "); ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
				ImGui::TextWrapped("%s", glGetString(GL_RENDERER));
				ImGui::PopStyleColor(1);
				
				GLint ramBudget, ramUsage, ramAvailable, ramReserved;
				glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &ramBudget);
				glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &ramAvailable);
				glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &ramReserved);
				ramUsage = ramBudget - ramAvailable;

				ImGui::Text("VRAM Budget: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%0.2f MB", ramBudget / 1024.0f);
				ImGui::Text("VRAM Usage: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%0.2f MB", ramUsage / 1024.0f);
				ImGui::Text("VRAM Available: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%0.2f MB", ramAvailable / 1024.0f);
				ImGui::Text("VRAM Reserved: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%0.2f MB", ramReserved / 1024.0f);
			}
		}

		ImGui::End();
	}

	if (consoleOpened)
	{
		if (ImGui::Begin("Console log", &consoleOpened))
		{
			while (!engineLog->logLines.empty())
			{
				lines.push_back(engineLog->logLines.front().c_str());
					engineLog->logLines.pop();
			}

			for (std::string line : lines)
			{
				ImGui::TextUnformatted(&line[0]);
			}

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);
		}

		ImGui::End();
	}

	if (aboutOpened)
	{
		if (ImGui::Begin("About", &aboutOpened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
		{
			ImGui::SetWindowSize("About", ImVec2(300, 200));

			ImGui::TextWrapped("%s created by Miquel Prieto Moline for \
the Masters in AAA videogame development first assignment at UPC School.", TITLE);

			ImGui::End();
		}
	}

	if (propertiesOpened)
	{
		if (ImGui::Begin("Properties", &propertiesOpened, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (App->renderer->AnyModelLoaded())
			{
				Model* model = App->renderer->GetModel(0);
				ImGui::Text("TRANSFORMATION");
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				ImGui::TextWrapped("At the moment, no transformations are applied to any loaded model.");
				
				ImGui::Separator();
				
				ImGui::Text("GEOMETRY");
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				ImGui::Text("Number of vertices: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", model->GetNumVertices());
				ImGui::Text("Number of triangles: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", model->GetNumTriangles());

				ImGui::Separator();
				
				ImGui::Text("TEXTURE");
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				ImGui::Text("Height: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", App->textures->GetHeight());
				ImGui::Text("Width: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", App->textures->GetWidth());
			}

			else ImGui::Text("There's no model loaded in scene.");

			ImGui::End();
		}
	}

	return status;
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
