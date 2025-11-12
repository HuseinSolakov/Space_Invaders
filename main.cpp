#include "glad.h"
#include<GLFW/glfw3.h>

#include <iostream>
#include "ResourceManager.h"
#include "SpriteRenderer.h"

using namespace std;

class Game
{
private:
	//global variables
	
	//window related variables
	GLuint Window_width = 800;
	GLuint Window_height = 600;
	GLFWwindow* window = 0;
	
	//game running condition variable
	bool running = true;
	
	//Sprite renderer object
	SpriteRenderer  *Renderer;
	
	//callbacks
	//framebuffer for viewport 
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
	    glViewport(0, 0, width, height);
	}
	//glfw error callback
	static void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}
	
public:

//player
glm::vec2 player_pos = glm::vec2(350.0f, 500.0f);


//bool move_player = false;
enum Movement 
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	NONE
};
Movement move_player = NONE;

	//Constructor to Initialize game
	Game()
	{
		//set error callback
		glfwSetErrorCallback(error_callback);
		
		//initializing glfw
		if(!glfwInit())
		{
			running = false;
		}
		


		//setting window hints
		//glfw version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//opnegl profile
		glfwWindowHint(GLFW_OPENGL_PROFILE ,GLFW_OPENGL_CORE_PROFILE);
		
		#ifdef __APPLE__
		    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif
//		glfwWindowHint(GLFW_RESIZABLE, false);
		// creating the window 
		window = glfwCreateWindow(Window_width, Window_height, "Space_Invaders",NULL,NULL);
		// check if window was created
		if(!window)
		{
			glfwTerminate();
			running = false;
		}
		glfwMakeContextCurrent(window);
		
		//load opengl functions
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			printf("Failed to initialize glad\n");
			return;
		}
		
		//initialize viewport
		glViewport(0,0, Window_width, Window_height);
		//enable texture transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//sprite
		//load shaders into shader program
   		ResourceManager::LoadShader("../shaders/sprite.vs", "../shaders/sprite.frag", nullptr, "sprite_sheet");
		// configure shaders
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Window_width), static_cast<float>(Window_height), 0.0f, -1.0f, 1.0f);
		ResourceManager::GetShader("sprite_sheet").Use().SetInteger("image", 0);
		ResourceManager::GetShader("sprite_sheet").SetMatrix4("projection", projection);
	
		// load sprite source
		ResourceManager::LoadTexture("../Textures/player_test.png", true, "face");

		//get image dimensions
		int* imgSize = new int[2];
		imgSize[0] = ResourceManager::GetTexture("face").img_width;
		imgSize[1] = ResourceManager::GetTexture("face").img_height;
		// set render-specific controls
		Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), 64, 0, 2, 1, imgSize[0], imgSize[1]);

		
	}
	
	//input 
	void ProcessInput()
	{
			if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
					glfwSetWindowShouldClose(window,true);
					
			if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
					move_player = RIGHT;

			if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
					move_player = LEFT;

			if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
					move_player = UP;

			if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
					move_player = DOWN;
	}
	
	//update function
	void Update()
	{
		//check if window is closing 
		if(glfwWindowShouldClose(window))
				running = false;
		//Player movement
		switch(move_player)
		{
			case(RIGHT): player_pos.x += 2.0; move_player = NONE;	break;
			case(LEFT):  player_pos.x -= 2.0; move_player = NONE;	break;
			case(UP):    player_pos.y -= 2.0; move_player = NONE;	break;
			case(DOWN):  player_pos.y += 2.0; move_player = NONE;	break;
		}
			
		//setup viewport framebuffer
		glfwSetFramebufferSizeCallback(window, glfwSetFramebufferSizeCallback(window, framebuffer_size_callback));//[](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }); 
	}
	
	//render
	void Render()
	{
		glClearColor(0.1f,0.5f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//					texsture 	    position 			size			  rotate 	color
		Renderer->DrawSprite(ResourceManager::GetTexture("face"), player_pos, glm::vec2(70.0f, 70.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
			
		glfwSwapBuffers(window);
		glfwPollEvents();	
	}

	//run game
	void Run()
	{
		while(running == true)
		{
			ProcessInput();
			Update();
			Render();
		}
		
		ResourceManager::Clear();
		glfwTerminate();
	}
};


int main()
{
	Game game;
	game.Run();

//cout<<"Hello World !"<<endl;

return 0;
}
