#include "glad.h"
#include<GLFW/glfw3.h>

#include <iostream>
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Time.h"
#include "EntityBehaviour.h"

using namespace std;

class Game
{
private:

	//VARIABLES
	
	//animation stuff
		Time time;
		int sprite_location = 1;
	
	//delta time
		float deltaTime = 0.0f;
	
	//window related variables
		GLuint Window_width = 800; 
		GLuint Window_height = 600;
		GLFWwindow* window = 0;
	
	//game running condition variable
		bool running = true;
	
	//Sprite renderer object
		SpriteRenderer  *Renderer;
		
	//player
		Player* player = new Player(glm::vec2(350.0f, 500.0f), glm::vec2(70.0f, 70.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	
	//METHODS
	//input 
	void ProcessInput()
	{
	  //Game Exit
		if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window,true);
	  //Player input	
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				player->move_player = RIGHT;

		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				player->move_player = LEFT;
	}
	
	//update 
	void Update()
	{
	//player animation
		  //timer
		  bool animation = time.Timer(0.1);
		  //if the player is moving
		  if(player->move_player != NONE)
		  {
		  	if(animation)
		  	{
		  		Renderer->SetSpriteLocation(sprite_location, 1);
		  		sprite_location ++;
		  
			if(sprite_location > 6)
				sprite_location = 1;
		  	}
		  }
		  //if the player is not moving
		  else
		  	Renderer->SetSpriteLocation(1, 1);

	//get deltatime
		deltaTime = time.GetDeltaTime();
		
	//check if window is closing 
		if(glfwWindowShouldClose(window))
				running = false;

	//Player movement
		float speed = 200.0 * deltaTime;
		
		switch(player->move_player)
		{
			case(RIGHT): player->position.x += speed; player->move_player = NONE;	break;
			case(LEFT):  player->position.x -= speed; player->move_player = NONE;	break;
		}
	
	//Checking if player hits window border
		//player->BorderCheck(Window_width);	//player stays at border
		player->BorderSwap(Window_width);	//player swaps borders
	}
	
	//render
	void Render()
	{
		//background color
		glClearColor(0.1f,0.5f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw player sprite	
		Renderer->DrawSprite(ResourceManager::GetTexture("face"), player->position, player->size, 0.0f, player->rotation);
			
		//swap buffers poll events
		glfwSwapBuffers(window);
		glfwPollEvents();	
	}
	
public:

	//Constructor to Initialize game
	Game()
	{
		//set error callback
		glfwSetErrorCallback([](int error, const char* description){fprintf(stderr, "Error: %s\n", description); });
		
		//initializing glfw
		if(!glfwInit())
			running = false;
		
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
		Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), 64, 0, 1, 1, imgSize[0], imgSize[1]);	
		
		//viewport framebuffer callback
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });
	}
	
	//run game
	void Run()
	{
		//game loop
		while(running == true)
		{		
			ProcessInput();
			Update();
			Render();
		}
		//clear resources
		ResourceManager::Clear();
		//terminate glfw
		glfwTerminate();
	}
};

int main()
{
	Game game;
	game.Run();
	return 0;
}
