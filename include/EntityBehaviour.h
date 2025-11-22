#ifndef Enity_Behaviourt_H
#define Enity_Behaviourt_H

#include <iostream>
#include "Renderer.h"
#include "Time.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Bullet
{
	SHOOT,
	NONE
};

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

Time time;
Movement move_player;
Bullet   bullet;

float rotation;
int sprite_location;
int frames;		//temporary solution

//constructors
Player(glm::vec2 position,  glm::vec2 size,  glm::vec3 color = glm::vec3(1.0f));
Player(glm::vec2 position,  glm::vec2 size,  float rotation, glm::vec3 color = glm::vec3(1.0f));

//methods
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
int sprite;
bool render_enemy[5][11];
glm::vec2 enemy_positions[5][11];

//setup enemy positions
void SetPositions();

public:
//properties
glm::vec2 position;
glm::vec2 distance;
glm::vec3 color;
glm::vec2 size;

Time time;
float rotation;

//constructors
Enemy(glm::vec2 position,  glm::vec2 size,    glm::vec2 distance, glm::vec3 color = glm::vec3(1.0f));
Enemy(glm::vec2 position,  glm::vec2 size,    glm::vec2 distance, float rotation, glm::vec3 color = glm::vec3(1.0f));

//methods
//rendering methods
void DrawEnemies(SpriteRenderer   *sprite_renderer, Texture2D &texture, int frames, int sprite_row);
void EnemyAnimation(float duration = 0.3f, int frames = 2);

//Check if enemies get hit
void EnemyHitDetection(Player *player);
};
#endif
