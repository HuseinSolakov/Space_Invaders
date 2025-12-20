#include "glad.h"
#include<GLFW/glfw3.h>

#include <iostream>
#include <time.h>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Time.h"
#include "EntityBehaviour.h"

using namespace std;

//key callback
bool shoot = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	  if(key == GLFW_KEY_X && action == GLFW_PRESS)
	  {
	  		shoot = true;
	  }
	  
	  else if(key == GLFW_KEY_X && action == GLFW_RELEASE || GLFW_REPEAT)
	  {
	  		shoot = false;
	  }
}

//game class
class Game
{
private:
	//VARIABLES
		
	//delta time
	Time my_time;
	float deltaTime = 0.0f;

	//game running condition variable
	bool running = true;
	
	//window related variables
	GLuint Window_width = 800; 
	GLuint Window_height = 600;
	GLFWwindow* window = 0;
	
	//player renderers
	SpriteRenderer  *player_renderer;
	SpriteRenderer  *bullet_renderer;
	
	//enemy renderers
	SpriteRenderer  *enemy_renderer;
	ShapeRenderer   *shape_renderer;
	
	//player object
	Player* player = new Player(glm::vec2(350.0f, 525.0f), glm::vec2(60.0f, 60.0f));
	
	//enemy object
	Enemy* enemy =  new Enemy(glm::vec2(160.0f, 80.0f),  glm::vec2(25.0f,25.0f),    glm::vec2(0.0f, 0.0f));
	
	//METHODS
	
	//input 
	void ProcessInput()
	{
	  //Game Exit => ESC
	  if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window,true);
	  //Player input => arrow keys
	  if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			player->move_player = Movement::RIGHT;

	  if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			player->move_player = Movement::LEFT;
	  //Player bullet => X
	  if(shoot)
  		player->bullet = Bullet::SHOOT;
	  
	  //keycallback
	  glfwSetKeyCallback(window, key_callback);
	}
	
	//update 
	void Update()
	{
	//check if window is closing 
	if(glfwWindowShouldClose(window))
		running = false;
		
	//get deltatime
	deltaTime = my_time.GetDeltaTime();
		
	//Player movement
	float speed = 200.0 * deltaTime;
	
	//update player position after input	
	switch(player->move_player)
	{
		case(Movement::RIGHT): player->position.x += speed; player->move_player = Movement::NONE;	break;
		case(Movement::LEFT):  player->position.x -= speed; player->move_player = Movement::NONE;	break;
	}
	
	//Checking if player hits window border
	player->BorderSwap(Window_width);
	
	//player update
	player->UpdatePlayer();
	
	//Player bullet
	float bullet_speed = 400.0 * deltaTime;
	player->PlayerBullet(bullet_speed);

	//Enemy 
	enemy->EnemyMovement(Window_width, Window_height, deltaTime);
	enemy->Update_Bullet(100.0f, deltaTime, Window_height, player);
	
	//hit detection
	enemy->EnemyHitDetection(player);
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
		
		//player animations
		player->Animation(player_renderer, 0.1f);
		player->Hit_Animation(player_renderer, 0.1f);
		player->BulletAnimation(bullet_renderer, 9, 4);

		//draw enemies
		enemy->DrawEnemies(enemy_renderer,  ResourceManager::GetTexture("sprites"), 2, 3);
		enemy->Render_Bullet(shape_renderer);

		//enemy animation
		enemy->EnemyAnimation(0.8f, 2);
		
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
   		ResourceManager::LoadShader("../shaders/default.vert", "../shaders/default.frag", nullptr, "default");

		// configure shaders
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Window_width), static_cast<float>(Window_height), 0.0f, -1.0f, 1.0f);
		ResourceManager::GetShader("sprite_sheet").Use().SetInteger("image", 0);
		ResourceManager::GetShader("sprite_sheet").Use().SetMatrix4("projection", projection);
	
		ResourceManager::GetShader("default").Use().SetMatrix4("projection", projection);
		
		//sprites
		ResourceManager::LoadTexture("../Textures/sprites.png", true, "sprites");

		//get sprite image dimensions
		int* imgSize = new int[2];
		imgSize[0] = ResourceManager::GetTexture("sprites").img_width;
		imgSize[1] = ResourceManager::GetTexture("sprites").img_height;
		
		shape_renderer = new ShapeRenderer(ResourceManager::GetShader("default"), Shape::Line, GL_DYNAMIC_DRAW);
		//set render-specific controls
		player_renderer = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), 32, glm::vec2(0.0f, 10.0f), 1,1, 288,158);
		enemy_renderer  = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), 32, glm::vec2(0.0f, 10.0f), 1,1, 288,158);
		bullet_renderer = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), 32, glm::vec2(0.0f, 10.0f), 1,1, 288,158);
		
		//viewport framebuffer callback
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });
		
		//set enemy speed
		enemy->SetSpeed(30.0f, 5.0f, 5.0f);
	}
	
	//destructor
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
		delete shape_renderer;
		
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
			Render();
			Update();

		}
	}
};

int main()
{
	srand((unsigned) time(NULL));

	Game game;
	game.Run();
	return 0;
}
