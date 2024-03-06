#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <iostream>
#include <chrono>
#include <imgui_plot.h>

static std::vector<float> timingarr3{ };
static std::vector<float> timingarr2{ };
static std::vector<float> timingarr4{ };


struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
};

class GameObject3D
{
public:
	Transform transform;
	int ID;
};

class GameObject3DAlt
{
public:
	Transform transform;
	int ID = 1;
};

void draw_multi_plot(std::vector<float> arr, std::vector<float> arr2, std::vector<float> arr3) {
	////ImGui::NewFrame();
	//int buf_size{ 9 };
	//float* arr4 = new float[] {1, 2, 4, 8, 15, 32, 64, 128, 512, 1024};
	//static const float* y_data[] = { arr, arr2, arr3, arr4 };
	//static ImU32 colors[4] = { ImColor(0, 255, 0), ImColor(255, 0, 0), ImColor(0, 0, 255), ImColor(0, 255, 255) };
	//static uint32_t selection_start = 0, selection_length = 0;

	//ImGui::Begin("Example plot", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	//// Draw first plot with multiple sources
	//ImGui::PlotConfig conf{};
	//conf.values.xs = arr;
	//conf.values.count = buf_size;
	//conf.values.ys_list = y_data; // use ys_list to draw several lines simultaneously
	//conf.values.ys_count = 3;
	//conf.values.colors = colors;
	//conf.scale.min = -1;
	//conf.scale.max = 1;
	//conf.tooltip.show = true;
	//conf.grid_x.show = true;
	//conf.grid_x.size = 128;
	//conf.grid_x.subticks = 4;
	//conf.grid_y.show = true;
	//conf.grid_y.size = 0.5f;
	//conf.grid_y.subticks = 5;
	//conf.selection.show = true;
	//conf.selection.start = &selection_start;
	//conf.selection.length = &selection_length;
	//conf.frame_size = ImVec2(float(buf_size), 200);
	//ImGui::Plot("plot1", conf);

	//// Draw second plot with the selection
	//// reset previous values
	//conf.values.ys_list = nullptr;
	//conf.selection.show = false;
	//// set new ones
	//conf.values.ys = arr4;
	//conf.values.offset = selection_start;
	//conf.values.count = selection_length;
	//conf.line_thickness = 2.f;
	//ImGui::Plot("plot2", conf);

	//// Draw second plot with the selection
	//// reset previous values
	//conf.values.ys_list = nullptr;
	//conf.selection.show = false;
	//// set new ones
	//conf.values.ys = arr3;
	//conf.values.offset = selection_start;
	//conf.values.count = selection_length;
	//conf.line_thickness = 2.f;
	//ImGui::Plot("plot3", conf);

	//ImGui::End();

	ImGui::Begin("Timing plots", nullptr, ImGuiWindowFlags_None);

	ImGui::Text("Combinend plot", nullptr, ImGuiWindowFlags_None);
	int buf_size = 11;
	std::vector<float> arr4 = {1, 2, 4, 8, 15, 32, 64, 128, 512, 1024};

	const float* y_data[] = {arr.data(), arr2.data(), arr3.data() };

	std::vector<ImU32> colors = { ImColor(255, 0, 0), ImColor(0, 255, 0), ImColor(0, 255, 255) };
	uint32_t selection_start = 0, selection_length = 0;

	ImGui::PlotConfig conf;
	conf.values.xs = arr4.data();  // x-axis values
	conf.values.count = buf_size;  // number of data points
	conf.values.ys_list = y_data;  // y-axis values
	conf.values.ys_count = 3;  // number of lines to plot

	conf.values.colors = colors.data();  // line colors

	conf.scale.min = arr[arr.size() - 1];  // minimum value on y-axis
	conf.scale.max = arr[0];   // maximum value on y-axis

	conf.tooltip.show = true;  // show tooltips
	conf.grid_x.show = true;  // show x-axis grid
	conf.grid_x.size = arr[0];   // x-axis grid size

	conf.grid_y.show = true;  // show y-axis grid
	conf.grid_y.size = 1024;  // y-axis grid size

	conf.selection.show = true;  // show selection
	conf.selection.start = &selection_start;  // selection start index
	conf.selection.length = &selection_length;  // selection length

	conf.frame_size = ImVec2(400, 200);  // plot frame size
	ImGui::Plot("plot1", conf);  // draw plot
	ImGui::Text("Test1 plot", nullptr, ImGuiWindowFlags_None);

	ImGui::PlotConfig conf2;
	conf2.values.xs = arr4.data();  // x-axis values
	conf2.values.count = buf_size;  // number of data points
	conf2.values.ys= arr.data();  // y-axis values
	conf2.values.colors = colors.data();  // line colors

	conf2.scale.min = arr[arr.size() - 1];  // minimum value on y-axis
	conf2.scale.max = arr[0];   // maximum value on y-axis

	conf2.tooltip.show = true;  // show tooltips
	conf2.grid_x.show = true;  // show x-axis grid
	conf2.grid_x.size = arr[0];   // x-axis grid size

	conf2.grid_y.show = true;  // show y-axis grid
	conf2.grid_y.size = arr[0];  // y-axis grid size

	conf2.selection.show = true;  // show selection
	conf2.selection.start = &selection_start;  // selection start index
	conf2.selection.length = &selection_length;  // selection length

	conf2.frame_size = ImVec2(400, 200);  // plot frame size
	ImGui::Plot("plot2", conf2);  // draw plot
	ImGui::Text("Test2 plot", nullptr, ImGuiWindowFlags_None);

	// Draw second plot with the selection
	conf2.values.ys = arr2.data();
	conf2.scale.min = arr2[arr2.size() - 1];  // minimum value on y-axis
	conf2.scale.max = arr2[0];   // maximum value on y-axis
	conf2.grid_x.size = arr2[0];   // x-axis grid size

	ImGui::Plot("plot3", conf2);
	// Draw second plot with the selection
	ImGui::Text("Test3 plot", nullptr, ImGuiWindowFlags_None);

	conf2.values.ys = arr3.data();
	conf2.scale.min = arr3[arr3.size() - 1];  // minimum value on y-axis
	conf2.scale.max = arr3[0];   // maximum value on y-axis
	conf2.grid_x.size = arr3[0];   // x-axis grid size

	ImGui::Plot("plot4", conf2);

	ImGui::End();

}


void TrashTheCash()
{
	std::cout << "Hello World!\n";

	int size{ int(std::pow(2, 26))/10 };
	std::cout << size << "\n";
	int* arr{ new int[size] };
	GameObject3D* arr2{ new GameObject3D[size] };
	for (int i = 0; i < size; i++)
	{
		arr2[i].ID = 50;
	}
	GameObject3DAlt* arr3{ new GameObject3DAlt[size] };
	for (int i = 0; i < size; i++)
	{
		arr3[i].ID = 50;
	}


	for (int step = 1; step <= 1024; step *= 2)
	{
		std::vector<long long> timingarr;
		for (size_t i = 0; i < 11; i++)
		{
			const auto start = std::chrono::high_resolution_clock::now();
			for (int i2 = 0; i2 < size; i2 += step) arr[i2] *= 2;
			const auto end = std::chrono::high_resolution_clock::now();
			const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			timingarr.push_back(total);
		}
		timingarr.erase(std::remove(timingarr.begin(), timingarr.end(), timingarr[0]));
		timingarr.erase(std::remove(timingarr.begin(), timingarr.end(), timingarr[timingarr.size() - 1]));
		int average{ 0 };
		for (auto item : timingarr) average += int(item);
		timingarr2.push_back(float(float(average / timingarr.size())));
		std::cout << step << "; " << average / timingarr.size() << "\n";
	}


	std::cout << "\n\n";

	for (int step = 1; step <= 1024; step *= 2)
	{
		std::vector<long long> timingarr;
		for (size_t i = 0; i < 11; i++)
		{
			const auto start = std::chrono::high_resolution_clock::now();
			for (int i2 = 0; i2 < size; i2 += step) arr2[i2].ID *= 2;
			const auto end = std::chrono::high_resolution_clock::now();
			const long long total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			timingarr.push_back(static_cast<long long>(total));
		}
		timingarr.erase(std::remove(timingarr.begin(), timingarr.end(), timingarr[0]));
		timingarr.erase(std::remove(timingarr.begin(), timingarr.end(), timingarr[timingarr.size() - 1]));
		int average{ 0 };
		for (auto item : timingarr) average += int(item);
		timingarr3.push_back(float(float(average / timingarr.size())));
		std::cout << step << "; " << average / timingarr.size() << "\n";
	}

	std::cout << "\n\n";

	for (int step = 1; step <= 1024; step *= 2)
	{
		std::vector<long long> timingarr;
		for (size_t i = 0; i < 11; i++)
		{
			const auto start = std::chrono::high_resolution_clock::now();
			for (int i2 = 0; i2 < size; i2 += step) arr3[i2].ID *= 2;
			const auto end = std::chrono::high_resolution_clock::now();
			const long long total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			timingarr.push_back(static_cast<long long>(total));
		}
		timingarr.erase(std::remove(timingarr.begin(), timingarr.end(), timingarr[0]));
		timingarr.erase(std::remove(timingarr.begin(), timingarr.end(), timingarr[timingarr.size() - 1]));
		int average{ 0 };
		for (auto item : timingarr) average += int(item);
		timingarr4.push_back(float(float(average / timingarr.size())));
		std::cout << step << "; " << average / timingarr.size() << "\n";
	}

	//timingarr2.erase(std::remove(timingarr2.begin(), timingarr2.end(), timingarr2[0]));
	//timingarr2.erase(std::remove(timingarr2.begin(), timingarr2.end(), timingarr2[timingarr2.size() - 1]));
	//timingarr3.erase(std::remove(timingarr3.begin(), timingarr3.end(), timingarr3[0]));
	//timingarr3.erase(std::remove(timingarr3.begin(), timingarr3.end(), timingarr3[timingarr3.size() - 1]));
	//timingarr4.erase(std::remove(timingarr4.begin(), timingarr4.end(), timingarr4[0]));
	//timingarr4.erase(std::remove(timingarr4.begin(), timingarr4.end(), timingarr4[timingarr4.size() - 1]));

	delete[] arr;
	delete[] arr2;
	delete[] arr3;
}



int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();

	TrashTheCash();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow();
	draw_multi_plot(timingarr2, timingarr3, timingarr4);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
