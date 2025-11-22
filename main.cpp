#include "glad.h"
#include<GLFW/glfw3.h>

#include <iostream>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Time.h"
#include "EntityBehaviour.h"

using namespace std;

class Game
{
private:

	//VARIABLES
	
	//delta time
	Time time;
	float deltaTime = 0.0f;
	
	//window related variables
	GLuint Window_width = 800; 
	GLuint Window_height = 600;
	GLFWwindow* window = 0;
	
	//game running condition variable
	bool running = true;
	
	//Sprite renderer object
	SpriteRenderer  *player_renderer;
	SpriteRenderer  *enemy_renderer;
	SpriteRenderer  *bullet_renderer;
	
	//player object
	Player* player = new Player(glm::vec2(350.0f, 525.0f), glm::vec2(60.0f, 60.0f));
	
	//enemy object
	Enemy* enemy =  new Enemy(glm::vec2(160.0f, 80.0f),  glm::vec2(25.0f,25.0f),    glm::vec2(0.0f, 0.0f));
	
	//METHODS
	//input 
	void ProcessInput()
	{
	  //Game Exit
	  if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window,true);
	  //Player input	
	  if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			player->move_player = Movement::RIGHT;

	  if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			player->move_player = Movement::LEFT;
	  //Player bullet
	  if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	  		player->bullet = Bullet::SHOOT;
	}
	
	//update 
	void Update()
	{
	//check if window is closing 
	if(glfwWindowShouldClose(window))
		running = false;
				
	//player animation
	player->Animation(player_renderer);
	
	//enemy animation
	enemy->EnemyAnimation(0.8f, 2);
	
	//hit detection
	enemy->EnemyHitDetection(player);
		
	//get deltatime
		deltaTime = time.GetDeltaTime();
		
	//Player movement
		float speed = 200.0 * deltaTime;
		
		switch(player->move_player)
		{
			case(Movement::RIGHT): player->position.x += speed; player->move_player = Movement::NONE;	break;
			case(Movement::LEFT):  player->position.x -= speed; player->move_player = Movement::NONE;	break;
		}
	
	//Checking if player hits window border
		player->BorderSwap(Window_width);
		
	//Player Bullet
		float bullet_speed = 400.0 * deltaTime;
		player->PlayerBullet(bullet_speed);
		player->BulletAnimation(bullet_renderer, 9, 3);
	}
	
	//render
	void Render()
	{
		//background color
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw player
		player->DrawPlayer(player_renderer, ResourceManager::GetTexture("sprites"));
		
		//draw player bullet
		if(player->bullet == Bullet::SHOOT)
		bullet_renderer->DrawSprite(ResourceManager::GetTexture("sprites"), player->bullet_position, glm::vec2(20.0f,20.0f));
		
		//draw enemies
		enemy->DrawEnemies(enemy_renderer, ResourceManager::GetTexture("sprites"), 2, 2);
		
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
		
		//shaders
		//load shaders into shader program
   		ResourceManager::LoadShader("../shaders/sprite.vert", "../shaders/sprite.frag", nullptr, "sprite_sheet");

		// configure shaders
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Window_width), static_cast<float>(Window_height), 0.0f, -1.0f, 1.0f);
		ResourceManager::GetShader("sprite_sheet").Use().SetInteger("image", 0);
		ResourceManager::GetShader("sprite_sheet").Use().SetMatrix4("projection", projection);
	
		//sprites
		ResourceManager::LoadTexture("../Textures/sprites.png", true, "sprites");

		//get sprite image dimensions
		int* imgSize = new int[2];
		imgSize[0] = ResourceManager::GetTexture("sprites").img_width;
		imgSize[1] = ResourceManager::GetTexture("sprites").img_height;
		
		//set render-specific controls
		player_renderer = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), 32, glm::vec2(0.0f,0.0f),  1,1, 288, 116);
		enemy_renderer  = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), 32, glm::vec2(0.0f, 10.0f), 1,1, 288,116);
		bullet_renderer = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), 32, glm::vec2(0.0f, 10.0f), 1,1, 288,116);
			
		//viewport framebuffer callback
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });
	}
	
	~Game()
	{
		//clear resources
		ResourceManager::Clear();

		//delete objects
		delete player;
		delete enemy;
		delete enemy_renderer;
		delete player_renderer;
		delete bullet_renderer;
		
		//terminate glfw after deleting objects
		glfwTerminate();
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
	}
};

int main()
{
	Game game;
	game.Run();
	return 0;
}
