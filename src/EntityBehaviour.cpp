#include "EntityBehaviour.h"


//PLayer

//Constructors
Player::Player(glm::vec2 position, glm::vec2 size)
{
	this->position = position;
	this->size = size;
	move_player = NONE;
}

Player::Player(glm::vec2 position,  glm::vec2 size, glm::vec3 rotation)
{
	this->position = position;
	this->size = size;
	this->rotation = rotation;
	move_player = NONE;
}

//methods
//check if player is at window's border
void Player::BorderCheck(int window_width)
{
	//if player hits right  side
	if(position.x > window_width - size.x)
		position.x = window_width - size.x;
		
	//if player hits left side	
	if(position.x < 0)
		position.x = 0;
}

void Player::BorderSwap(int window_width)
{
	//if player hits right  side
	if(position.x > window_width)
		position.x = -size.x;
		
	//if player hits left side	
	if(position.x < -size.x)
		position.x = window_width;
}

