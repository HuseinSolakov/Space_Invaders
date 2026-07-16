//imgui GUI Widgets
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//OpenGL and GLFW
#include "glad.h"
#include<GLFW/glfw3.h>

//custom libraries
#include "ResourceManager.h"
#include "Texture.h"
#include "stb_image.h"
#include "Time.h"

//cpp libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

enum class MenuButtons
{
	BUTTON_1, BUTTON_2, BUTTON_3, NONE
};

class Menu
{
	public:
	const char* score_contents = "empty";
	
	int button_size_x[3] = {250, 250, 250};
	int button_size_y[3] = {100, 100, 100};
	
	int button_pos_x[3] = {300,300,300};
	int button_pos_y[3] = {100,250,400};
	
	ImFont* pFont, *pixelFont;
	MenuButtons mm_buttons;
	
	Menu(GLFWwindow *window);

	void Start_Menu();
	bool Leaderboard_menu();
	void SetButtonTitles(const char* b1, const char* b2, const char* b3);
	void Name_Menu();

	private:
	const char* button_titles[3];
	GLFWwindow *window;
};