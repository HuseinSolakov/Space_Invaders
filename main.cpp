#define GLT_IMPLEMENTATION
//gui menu
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//custom menu library -> needs to be included before glad and glfw
#include "Menu.h"

//opengl functions
#include "glad.h"

//text
#include "gltext.h"

//window
#include<GLFW/glfw3.h>

//cpp libs
#include <iostream>
#include <fstream>

//custom libs
#include "ResourceManager.h"
#include "Renderer.h"
#include "Time.h"
#include "EntityBehaviour.h"
#include <time.h>

//image loading library
#include "stb_image.h"

using namespace std;

//key callback

// booleans to represent keys
bool shoot = false;
bool right_key = false;
bool left_key = false;
bool enter_key = false;

//key callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// X key press
	  if(key == GLFW_KEY_X && action == GLFW_PRESS)
	  {
	  		shoot = true;
	  }
	  
	  else if(key == GLFW_KEY_X && action == GLFW_RELEASE || GLFW_REPEAT)
	  {
	  		shoot = false;
	  }
	  
	  //arrow keys
	  if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	  {left_key = true;}
	  
	  else if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE || GLFW_REPEAT)
	  {left_key = false;}
	  
	  if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	  {right_key = true;}
	  
	  else if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE || GLFW_REPEAT)
	  {right_key = false;}
	  
	  
	  //ENTER
	  if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	  {enter_key = true;}
	  
	  else if(key == GLFW_KEY_ENTER && action == GLFW_RELEASE || GLFW_REPEAT)
	  {enter_key = false;}
	  
}

//game class
class Game
{
private:
	//	VARIABLES	//

	//naming menu variables
	double title_size[2];
	double title_position[2];

	double selector_positions[2];
	int selector_count[2] = {0,0};
	float selector_shift[2] = {0.0,0.0};
	const char* selector_texture;

	string player_name = " ";

	//winning condition is true if player has killed all enemies
	bool winning_condition = false;
	
	//for fading effect at the end of the game
	float fade = 0.0;

	//file
	fstream write_score;

	//menu
	Menu *menu;

	//delta time
	Time my_time;
	float deltaTime = 0.0f;

	//game running condition variable
	bool running = true;
	
	//window related variables
	GLuint Window_width = 800; 
	GLuint Window_height = 600;
	GLFWwindow* window = 0;
	
	//Sprite renderer
	//player renderers
	SpriteRenderer  *player_renderer;
	SpriteRenderer  *bullet_renderer;
	
	//enemy renderers
	SpriteRenderer  *enemy_renderer;
	ShapeRenderer   *shape_renderer;
	
	//UI
	int score = 0;
	SpriteRenderer *UI;
	GLTtext *Score_text;
	GLTtext *EndGame_text;
	
	//name screen
	SpriteRenderer *InputName;
	GLTtext *Player_Name_text;
	
	//player object
	Player* player = new Player(glm::vec2(350.0f, 525.0f), glm::vec2(60.0f, 60.0f));
	
	//enemy object
	Enemy* enemy =  new Enemy(glm::vec2(160.0f, 80.0f),  glm::vec2(25.0f,25.0f),    glm::vec2(0.0f, 0.0f));
	
	//	METHODS		//
	
	//1) INPUT METHOD
	void ProcessInput()
	{
	  //Game Exit => ESC
	  if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window,true);

	//game input
	  if (menu->mm_buttons == MenuButtons::BUTTON_1)		
	  {
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
	}
	
	//2) UPDATE METHOD
	void Update()
	{
	  //main menu scoreboard button
	  if(menu->mm_buttons == MenuButtons::BUTTON_2)
	     menu->mm_buttons = MenuButtons::NONE;

	  //check if window is closing 
	  if(glfwWindowShouldClose(window) || menu->mm_buttons == MenuButtons::BUTTON_3)
	     running = false;

	  //game update => main menu play button
	  if(menu->mm_buttons == MenuButtons::BUTTON_1 && fade <= 1.0)
	  {
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
	     if(enemy->EnemyHitDetection(player) == true)
	     {
	       //if enemy is hit add 1 to the score and then display it
	       score++;
	       string str = to_string(score);
	       gltSetText(Score_text, str.c_str());
	     }
	  }
	
	// The naming screen
	//after game is over and fading effect is done
	  if(fade >= 1.0)
	  {
	    if(left_key)
	    {
	      if(selector_count[0] > 0)	{ selector_shift[0] -= 50; selector_count[0]--; }
	      else if(selector_count[0] <= 0 && selector_count[1] > 0)	{ selector_shift[0] += 50*7; selector_shift[1] -= 50; selector_count[0] = 7; --selector_count[1];}
	    }

	    if(right_key)
	    {
	      if(selector_count[0] < 7)
	      {
	        if(selector_count[1] < 5)  { selector_shift[0] += 50; selector_count[0]++;}
	        if(selector_count[1] == 5) { if(selector_count[0] < 3) { selector_shift[0] += 50; selector_count[0]++;} }
	      }
	      
	      else if(selector_count[0] >= 7 && selector_count[1] < 5)
	      { selector_shift[0] -= 50*7; selector_shift[1] += 50; selector_count[0] = 0; ++selector_count[1]; }
	    }

	    if(right_key) right_key = false;
	    if(left_key) left_key = false;

	    if(enter_key)
	    {
	      //switch between the coordinated
	      //y coordinate
	      switch(selector_count[1])
	      {
	        case 0:
	          //x coordinate
	          switch(selector_count[0])
	          {
	            case 0: player_name += 'A'; break;
	            case 1: player_name += 'B'; break;
	            case 2: player_name += 'C'; break;
	            case 3: player_name += 'D'; break;
	            case 4: player_name += 'E'; break;
	            case 5: player_name += 'F'; break;
	            case 6: player_name += 'G'; break;
	            case 7: player_name += 'H'; break;
	          }
	          
	        break;
	        
	        case 1:
	          //x coordinate
	          switch(selector_count[0])
	          {
	            case 0: player_name += 'I'; break;
	            case 1: player_name += 'J'; break;
	            case 2: player_name += 'K'; break;
	            case 3: player_name += 'L'; break;
	            case 4: player_name += 'M'; break;
	            case 5: player_name += 'N'; break;
	            case 6: player_name += 'O'; break;
	            case 7: player_name += 'P'; break;
	          }
	          
	        break;
	        
	        case 2:
	          //x coordinate
	          switch(selector_count[0])
	          {
	            case 0: player_name += 'Q'; break;
	            case 1: player_name += 'R'; break;
	            case 2: player_name += 'S'; break;
	            case 3: player_name += 'T'; break;
	            case 4: player_name += 'U'; break;
	            case 5: player_name += 'V'; break;
	            case 6: player_name += 'W'; break;
	            case 7: player_name += 'X'; break;
	          }
	          
	        break;
	        
	        case 3:
	          //x coordinate
	          switch(selector_count[0])
	          {
	            case 0: player_name += 'Y'; break;
	            case 1: player_name += 'Z'; break;
	            case 2: player_name += '1'; break;
	            case 3: player_name += '2'; break;
	            case 4: player_name += '3'; break;
	            case 5: player_name += '4'; break;
	            case 6: player_name += '5'; break;
	            case 7: player_name += '6'; break;
	          }
	          
	        break;
	        
	        case 4:
	          //x coordinate
	          switch(selector_count[0])
	          {
	            case 0: player_name += '7'; break;
	            case 1: player_name += '8'; break;
	            case 2: player_name += '9'; break;
	            case 3: player_name += '!'; break;
	            case 4: player_name += '@'; break;
	            case 5: player_name += '#'; break;
	            case 6: player_name += '$'; break;
	            case 7: player_name += '%'; break;
	          }
	          
	        break;
	        
	        case 5:
	          //x coordinate
	          switch(selector_count[0])
	          {
	            case 0: player_name += '^'; break;
	            case 1: player_name += '&'; break;
	            case 2: player_name += '*'; break;
	            
	            case 3: 
	              if(player_name.length() == 1) { write_score<<"NO_NAME"<<" "<<score<<"\n"; }
	              
	              else if(player_name.length() > 1) 
	              {
	                 player_name = player_name.substr(1, player_name.length()-1); 
	                 write_score<<player_name<<" "<<score<<"\n";
	              }
	              
	              write_score.close(); running = false;
	            break;
	          }
	        break;
	      }
	    } enter_key = false;
	  }
	}
	
	//Drawing stuff on screen
	void Render()
	{
	  if(menu->mm_buttons == MenuButtons::NONE)
	     menu->Start_Menu();
	
	  //game
	  else if (menu->mm_buttons == MenuButtons::BUTTON_1 && fade <= 1.0)
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

	    //draw enemies / win screen
	    if(enemy->DrawEnemies(enemy_renderer,  ResourceManager::GetTexture("sprites"), 2, 3))
	       winning_condition = true;

	    //draw enemy bullets
	    enemy->Render_Bullet(shape_renderer);

	    //enemy animation
	    enemy->EnemyAnimation(0.8f, 2);

	    //UI
	      //Score
	      UI->SetSpriteLocation(1, 5);
	      UI->DrawSprite(ResourceManager::GetTexture("UI_elements") ,glm::vec2(Window_width - (Window_width - 10), Window_height - (Window_height - 20)), glm::vec2(120.0, 30.0));

	      //Score text
	      gltBeginDraw();
	        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
	        gltDrawText2D(Score_text, 130, 13, 3);
	      gltEndDraw();

	      //Lives
	      switch(player->status.lives)
	      {
	        case 0:  UI->SetSpriteLocation(1, 4); break;
	        case 1:  UI->SetSpriteLocation(1, 3); break;
	        case 2:  UI->SetSpriteLocation(1, 2); break;
	        case 3:  UI->SetSpriteLocation(1, 1); break;
	      }
	      UI->DrawSprite(ResourceManager::GetTexture("UI_elements"), glm::vec2(Window_width - 160, Window_height - (Window_height - 20)), glm::vec2(120.0, 30.0));
	  } // end of game

	  //lose screen
	  if(!player->status.alive)
	  {
	    gltBeginDraw();
	      gltSetText(EndGame_text, "YOU LOSE !");
	      gltDrawText2D(EndGame_text, 50, 200, 9);
	    gltEndDraw();
	
	    //draw a square that fades in slowely
	    shape_renderer->DrawShape(glm::vec2(0.0, 0.0), glm::vec2(Window_width, Window_height), 0.0, glm::vec4(0.0,0.0,0.0,fade));
	    if(fade < 1.0)
	       fade += 0.005;
	  }

	  //win screen
	  if(winning_condition)
	  {
	
	    gltBeginDraw();
	      gltSetText(EndGame_text, "YOU WON !");
	      gltDrawText2D(EndGame_text, 50, 200, 9);
	    gltEndDraw();
	
	     //draw a square that fades in slowely
	     shape_renderer->DrawShape(glm::vec2(0.0, 0.0), glm::vec2(Window_width, Window_height), 0.0, glm::vec4(0.0,0.0,0.0,fade));
	     if(fade < 1.0)
	        fade += 0.005;
	  }

	  //naming screen/menu
	  if(fade >= 1.0)
	  {
	    title_size[0] = Window_width*0.6; title_size[1] = Window_height*0.6;
	    title_position[0] = Window_width*0.2; title_position[1] = Window_height*0.3;
	    selector_positions[0] = (title_position[0] + title_position[0]*0.2)+selector_shift[0]; selector_positions[1] = (title_position[1] + title_position[1]*0.2)+selector_shift[1];
	    selector_texture = "score_selector";
	    
	    if(selector_count[0] == 3 && selector_count[1] == 5)
	       selector_texture = "score_arrow";
	    
	    InputName->DrawSprite(ResourceManager::GetTexture("score_title"),glm::vec2(title_position[0],title_position[1]), glm::vec2(title_size[0],title_size[1]));
	    InputName->DrawSprite(ResourceManager::GetTexture(selector_texture),glm::vec2(selector_positions[0],selector_positions[1]), glm::vec2(title_size[0]*0.10,title_size[1]*0.1));

	    gltBeginDraw();
	      gltSetText(Player_Name_text, player_name.c_str());
	      gltDrawText2D(Player_Name_text, 80, 50, 9);
	    gltEndDraw();
	  }
	
	  //swap buffers poll events
	  glfwSwapBuffers(window);
	  glfwPollEvents();
	}

public:
	//Constructor to Initialize game
	Game()
	{
	  //scoreboard information file
	  write_score.open("../scoreboard_info.txt",ios::out | ios::app | ios::ate);
	
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

	  //window icon
	  GLFWimage icon;
	  icon.pixels = stbi_load("../Textures/jellyfish.png", &icon.width, &icon.height, 0, 4);
	  
	  glfwSetWindowIcon(window, 1, &icon);
	  stbi_image_free(icon.pixels);
	
	  //load opengl functions
	  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	  {
	    printf("Failed to initialize glad\n");
	    return;
	  }

	  //menu => NOTE( needs to be after create window )
	  menu = new Menu(window);
	  menu->SetButtonTitles("PLAY", "SCOREBOARD", "EXIT");

	  //initialize viewport
	  glViewport(0,0, Window_width, Window_height);

	  //enable texture transparency
	  glEnable(GL_BLEND);
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	  // Initialize glText
	  gltInit();

	  //Setting up gltText objects
	  EndGame_text = gltCreateText();
	  Score_text = gltCreateText();
	  Player_Name_text = gltCreateText();

	  gltSetText(Score_text, "0");

	  //shaders
	  //load shaders into shader program
	  ResourceManager::LoadShader("../shaders/sprite.vert", "../shaders/sprite.frag", nullptr, "sprite_sheet");
	  ResourceManager::LoadShader("../shaders/default.vert", "../shaders/default.frag", nullptr, "default");

	  //configure shaders
	  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Window_width), static_cast<float>(Window_height), 0.0f, -1.0f, 1.0f);
	  ResourceManager::GetShader("sprite_sheet").Use().SetInteger("image", 0);
	  ResourceManager::GetShader("sprite_sheet").Use().SetMatrix4("projection", projection);
	  ResourceManager::GetShader("default").Use().SetMatrix4("projection", projection);

	  //sprites
	  //spritesheets
	  ResourceManager::LoadTexture("../Textures/sprites.png", true, "sprites");
	  ResourceManager::LoadTexture("../Textures/UI_elements.png", true, "UI_elements");
	  //single texture images
	  ResourceManager::LoadTexture("../Textures/score_title.png", true, "score_title");
	  ResourceManager::LoadTexture("../Textures/score_selector.png", true, "score_selector"); 
	  ResourceManager::LoadTexture("../Textures/arrow.png", true, "score_arrow");

	  //get sprite image dimensions
	  int* imgSize = new int[2];
	  imgSize[0] = ResourceManager::GetTexture("sprites").img_width;
	  imgSize[1] = ResourceManager::GetTexture("sprites").img_height;
	


	  //set render-specific controls
	  //shape renderer
	  shape_renderer = new ShapeRenderer(ResourceManager::GetShader("default"), GL_DYNAMIC_DRAW);
	  
	  //sprite renderer
	  InputName = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"));
	  UI = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), glm::vec2(40.0, 10.0), glm::vec2(0.0f,0.0f),1,1,40,50);
	  player_renderer = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), glm::vec2(32.0), glm::vec2(0.0f, 10.0f), 1,1, imgSize[0],imgSize[1]);
	  enemy_renderer  = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), glm::vec2(32.0), glm::vec2(0.0f, 10.0f), 1,1, imgSize[0],imgSize[1]);
	  bullet_renderer = new SpriteRenderer(ResourceManager::GetShader("sprite_sheet"), glm::vec2(32.0), glm::vec2(0.0f, 10.0f), 1,1, imgSize[0],imgSize[1]);
	
	  //viewport framebuffer callback
	  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });
	
	  //set enemy speed
	  enemy->SetSpeed(20.0f, 3.0f, 5.0f);
	}
	
	//destructor
	~Game()
	{
	  //clear resources
	  ResourceManager::Clear();
	  
	  //delete objects
	  delete menu;
	  
	  delete player;
	  delete enemy;
	  
	  delete enemy_renderer;
	  delete player_renderer;
	  delete bullet_renderer;
	  delete shape_renderer;
	  delete UI;

	  // Deleting text
	  gltDeleteText(this->Score_text);
	  gltDeleteText(this->EndGame_text); 
	  gltDeleteText(this->Player_Name_text);
	  
	  // Destroy glText
	  gltTerminate();
	  
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
  //random seed
  srand((unsigned) time(NULL));
  
  //starting the game
  Game game;
  game.Run();
  
  return 0;
}