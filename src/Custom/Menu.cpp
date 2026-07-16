#include "Menu.h"

Time timer_1[2];
bool show_leaderboard = false;
bool switch_texture = true;

//Menu constructor
Menu::Menu(GLFWwindow *window)
{
	this->window = window;

	//button
	mm_buttons = MenuButtons::NONE;
	button_titles[0] = " "; button_titles[1] = " "; button_titles[2] = " ";

	//setup IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io  = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//fonts
	pFont = io.Fonts->AddFontFromFileTTF("../Fonts/Freedom-10eM.ttf", 32.0f); 
	pixelFont = io.Fonts->AddFontFromFileTTF("../Fonts/Pixellettersfull-BnJ5.ttf", 32.0f);
}

//Method for setting up the button tittles
void Menu::SetButtonTitles(const char* b1, const char* b2, const char* b3)
{
	button_titles[0] = b1;
	button_titles[1] = b2;
	button_titles[2] = b3;
}

//Main menu
void Menu::Start_Menu()
{
	//Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiStyle& style = ImGui::GetStyle();

	//flags
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration;

	//push font
	ImGui::PushFont(this->pFont);

	//Window Attributes	
	ImGui::SetNextWindowSize(ImVec2(1000,1000));
	ImGui::SetNextWindowPos(ImVec2(0,0));	

	//window
	ImGui::Begin("Game_Menu",nullptr,flags);

	//button color
	style.Colors[ImGuiCol_Button] = ImVec4(0.0,0.0,0.0,0.0);         //Default
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9,1.0,1.0,0.1);  //Hovered
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.9,1.0,1.0,0.5);   //Clicked

	//Button Position
	ImGui::SetCursorPos(ImVec2(300-(button_size_x[0]-250),100-(button_size_y[0]-100)));

	//Texture
	Texture2D texture_crab_1, texture_crab_2, texture_jelly_1, texture_jelly_2;
	int nrChannels;

	//Play
	if(ImGui::Button(button_titles[0],ImVec2(button_size_x[0], button_size_y[0]))) 
	mm_buttons = MenuButtons::BUTTON_1;
	
	//checks latest hovered item 
	if(ImGui::IsItemHovered(0))
	{
		texture_crab_1.setFormat(GL_RGBA);

		if(timer_1[0].Timer(0.8f))
		{
			if(switch_texture == true) switch_texture = false;
			else if(switch_texture == false) switch_texture = true;
		}
	
		if(switch_texture)
		   texture_crab_1.Generate(texture_crab_1.img_width, texture_crab_1.img_height, stbi_load("../Textures/Crab.png", &texture_crab_1.img_width, &texture_crab_1.img_height, &nrChannels, 0));
		else if(!switch_texture)
		   texture_crab_1.Generate(texture_crab_1.img_width, texture_crab_1.img_height, stbi_load("../Textures/Crab_Blink.png", &texture_crab_1.img_width, &texture_crab_1.img_height, &nrChannels, 0));

		ImGui::SetCursorPos(ImVec2(button_pos_x[0]*1.88,button_pos_y[0]));
		ImGui::Image(texture_crab_1.ID, ImVec2(100, 100));
	
		ImGui::SetCursorPos(ImVec2(button_pos_x[0]/1.6,button_pos_y[0]));
		ImGui::Image(texture_crab_1.ID,ImVec2(100, 100));

		if(this->button_size_x[0] < 250+10)
		  {this->button_size_x[0] +=10; this->button_size_y[0] +=10;}
	}
	else if(!ImGui::IsItemHovered(0) && this->button_size_x[0] > 250) 
		{this->button_size_x[0] -=2; this->button_size_y[0] -=2;}
	
	//Scoreboard button
	ImGui::SetCursorPos(ImVec2(300,250));
	if(ImGui::Button(button_titles[1],ImVec2(250, 100))){mm_buttons = MenuButtons::BUTTON_2; show_leaderboard = true;}
	if(ImGui::IsItemHovered(0))
	{
		texture_jelly_1.setFormat(GL_RGBA);

		if(timer_1[0].Timer(0.8f))
		{
			if(switch_texture == true) switch_texture = false;
			else if(switch_texture == false) switch_texture = true;
		}

		if(switch_texture)
		   texture_jelly_1.Generate(texture_jelly_1.img_width, texture_jelly_1.img_height, stbi_load("../Textures/jellyfish.png", &texture_jelly_1.img_width, &texture_jelly_1.img_height, &nrChannels, 0));
	
		else if(!switch_texture)
		   texture_jelly_1.Generate(texture_jelly_1.img_width, texture_jelly_1.img_height, stbi_load("../Textures/jellyfish2.png", &texture_jelly_1.img_width, &texture_jelly_1.img_height, &nrChannels, 0));
	
		ImGui::SetCursorPos(ImVec2(button_pos_x[0]*1.88,button_pos_y[1]));
		ImGui::Image(texture_jelly_1.ID, ImVec2(100, 100));
	
		ImGui::SetCursorPos(ImVec2(button_pos_x[0]/1.6,button_pos_y[1]));
		ImGui::Image(texture_jelly_1.ID,ImVec2(100, 100));
	}

	//Exit
	ImGui::SetCursorPos(ImVec2(button_pos_x[2],button_pos_y[2]));
	//get mouse position
	ImVec2 mouse_pos = ImGui::GetMousePos();
	//Exit Button
	if(ImGui::Button(button_titles[2],ImVec2(250, 100)))
	   mm_buttons = MenuButtons::BUTTON_3;

	//if mouse is to the left move right
	if( (mouse_pos.x >= 300 && mouse_pos.y >= 400) && (mouse_pos.x <= 400 && mouse_pos.y <= 500) )
	{
		if(button_pos_x[2] < 350)
		   button_pos_x[2] += 10;

		if(button_pos_y[2] > 300 && button_pos_y[2] < 450)
		   button_pos_y[2] += 10;
	}
	//if mouse is to the right move left
	else if( (mouse_pos.x >= 400 && mouse_pos.y >= 400) && (mouse_pos.x <= 550 && mouse_pos.y <= 500) )
	{
		if(button_pos_x[2] > 250)
		   button_pos_x[2] -= 10;

		if(button_pos_y[2] > 370 )
		   button_pos_y[2] -= 10;
	}
	//if mouse is outside go back to original position
	else
	{
		if(button_pos_x[2] != 300)
		{
			if(button_pos_x[2] < 300)
			   button_pos_x[2] += 10;
			
			else if(button_pos_x[2] > 300)
			   button_pos_x[2] -= 10;
		}

		if(button_pos_y[2] != 400)
		{
			if(button_pos_y[2] < 400)
			   button_pos_y[2] += 10;
			
			else if(button_pos_y[2] > 400)
			   button_pos_y[2] -= 10;
		}
	}

	if(show_leaderboard) 
	   show_leaderboard = Leaderboard_menu();

	//Stop
	ImGui::PopFont();
	ImGui::End();
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//Child menu showing leaderboard
bool Menu::Leaderboard_menu()
{
	//variables
	//main string
	string mytext;
	
	//sub strings
	const char* text_head;
	const char* text_tail;
	
	//file write_score.open("../scoreboard_info.txt",ios::out | ios::app | ios::ate);
	ifstream file("../scoreboard_info.txt");
	
	ImGuiStyle& style = ImGui::GetStyle();
	//Window Attributes	
	ImGui::SetNextWindowSize(ImVec2(600,600));
	ImGui::SetNextWindowPos(ImVec2(150,20));
	ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(1.0,1.0,1.0,1.0));
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.3,0.3,0.3,1.0);

	//window
	ImGui::BeginChild("Game_Menu", ImVec2(600, 440), 0); //, ImVec2(600, 440), 0, flags
	
	ImGui::SetCursorPos(ImVec2(100,20));
	ImGui::Text("--- Leaderboard 23123---");
	
	ImGui::SetCursorPos(ImVec2(560,20));
	if(ImGui::Button("X", ImVec2(30,40)))
	{
		ImGui::PopStyleColor();
		ImGui::EndChild();
		return false;
	}

	ImGui::PushFont(this->pixelFont);
	std::vector<std::pair<int, string>> leaderboard;

	int position = 100;
	int index_temp = 0;

	while(getline(file, mytext))
	{
		//get the string after symbol
		text_tail = strchr(mytext.c_str(),' ');
	
		//getting the position of symbol
		int index = text_tail - mytext.c_str();

		//get the string before symbol
		text_head = (mytext.substr(0,index)).c_str();
		leaderboard.push_back(std::make_pair(atoi(text_tail),text_head));
	}
	
	//checking if it stores them
	int size = leaderboard.size();
	std::sort(leaderboard.begin(), leaderboard.end());

	for(int i = size-1; i>=0; i--)
	{
	  ImGui::SetCursorPos(ImVec2(10,position));
	  ImGui::LabelText(to_string(leaderboard.at(i).first).c_str(), leaderboard.at(i).second.c_str());
	  position += 50;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor();	
	ImGui::EndChild();
	
	//closing the file
	file.close();
	
	return true;
}