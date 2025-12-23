#ifndef Enity_Behaviourt_H
#define Enity_Behaviourt_H

#include <iostream>
#include <stdlib.h>
#include <cstdlib>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Time.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//bullet enum
enum class Bullet
{
	SHOOT,
	NONE
};

//movement enum
enum class Movement
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	NONE
};

//Player Behaviour class
class Player
{
public:
//properties
glm::vec2 bullet_position;
glm::vec2 position;
glm::vec3 color;
glm::vec2 size;
float rotation;

//timer
Time timer[3];

//enums
Movement move_player;
Bullet   bullet;

//player status
struct
{
  bool alive;
  bool hit;
  unsigned int lives;
  
} status;

private:
//sprite
int sprite_location;
int counter;
int frames;

public:
//constructors
Player(glm::vec2 position,  glm::vec2 size,  glm::vec3 color = glm::vec3(1.0f));
Player(glm::vec2 position,  glm::vec2 size,  float rotation, glm::vec3 color = glm::vec3(1.0f));

//methods
void UpdatePlayer();
void Hit_Animation(SpriteRenderer   *sprite_renderer, float duration = 0.05f, int sprite_row = 1);
//rendering methods
void Animation(SpriteRenderer   *sprite_renderer, float duration = 0.05f, int sprite_row = 1);
void DrawPlayer(SpriteRenderer   *sprite_renderer, Texture2D &texture);

//border methods
void BorderCheck(int window_width);
void BorderSwap(int window_width);

//bullet methods
void PlayerBullet(float speed);
void BulletAnimation(SpriteRenderer   *sprite_renderer, int frames, int sprite_row, float duration = 0.05f);
};

class Enemy
{
private:
//properties
glm::vec3 enemy_positions[5][11];	//enemy coordinates + enemy draw condition
glm::vec3 bullet_pos[5];		//bullet coordinates + bullet draw condition
glm::vec2 position;			//enemy starting position
glm::vec2 distance;			//distance between enemies
glm::vec3 color;			//enemy color
glm::vec2 size;			//enemy size
float rotation;			//enemy rotation

//variables used for drawing
int sprite;
int counter;
Shader enemy_shader;

//time object used for timer
Time time;

//movement enum
Movement move[2];
//enemy speed 
struct
{
	float X_speed;
	float Y_speed;
	float acceleration;
}enemy_speed;

//function to setup enemy positions
void SetPositions();

public:
//constructors
Enemy(glm::vec2 position,  glm::vec2 size,    glm::vec2 distance, glm::vec3 color = glm::vec3(1.0f));
Enemy(glm::vec2 position,  glm::vec2 size,    glm::vec2 distance, float rotation, glm::vec3 color = glm::vec3(1.0f));

//methods
//draw enemies returns whether enemies have been defeated or not
bool DrawEnemies(SpriteRenderer   *sprite_renderer,  Texture2D &texture, int frames, int sprite_row);
//enemy animation
void EnemyAnimation(float duration = 0.3f, int frames = 2);

//Check if enemies get hit
bool EnemyHitDetection(Player *player);

//Enemy Player Collision -> check if player and enemies collide or enemies are past player
bool EnemyPlayerCollision(Player *player);

//enemy movement
void SetSpeed(float speed, float Y_speed, float acceleration);
void EnemyMovement(int border_x, int border_y, float deltaTime);

//render the enemy bullets
void Render_Bullet(ShapeRenderer *shapeshader);

//update the enemy bullets
void Update_Bullet(float speed, float deltatime, int Window_height, Player *player);
};
#endif
