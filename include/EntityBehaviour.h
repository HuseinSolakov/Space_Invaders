#ifndef Enity_Behaviourt_H
#define Enity_Behaviourt_H
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Movement
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
glm::vec3 rotation;

Movement move_player;

//constructors
Player(glm::vec2 position,  glm::vec2 size);
Player(glm::vec2 position,  glm::vec2 size,  glm::vec3 rotation);

//methods
void BorderCheck(int window_width);
void BorderSwap(int window_width);

};
#endif
