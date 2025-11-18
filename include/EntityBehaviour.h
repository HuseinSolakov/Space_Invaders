#ifndef Enity_Behaviourt_H
#define Enity_Behaviourt_H
#include <iostream>
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
glm::vec2 position;
glm::vec2 size;
glm::vec3 color;

glm::vec2 bullet_position;

float rotation;

Movement move_player;
Bullet   bullet;

//constructors
Player(glm::vec2 position,  glm::vec2 size,    glm::vec3 color = glm::vec3(1.0f));
Player(glm::vec2 position,  glm::vec2 size,  float rotation, glm::vec3 color = glm::vec3(1.0f));

//methods
void BorderCheck(int window_width);
void BorderSwap(int window_width);

void PlayerBullet(float speed);

};
#endif
