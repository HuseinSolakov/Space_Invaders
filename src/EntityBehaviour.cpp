#include "EntityBehaviour.h"

//PLayer

//Constructors
Player::Player(glm::vec2 position, glm::vec2 size, glm::vec3 color)
{
	this->position = position;
	this->size = size;

	this->color = color;
	
	move_player = Movement::NONE;
	bullet = Bullet::NONE;
}

Player::Player(glm::vec2 position,  glm::vec2 size, float rotation, glm::vec3 color)
{
	this->position = position;
	this->size = size;
	this->rotation = rotation;

	this->color = color;
	
	move_player = Movement::NONE;
	bullet = Bullet::NONE;
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

void Player::PlayerBullet(float speed)
{
	if(bullet == Bullet::SHOOT)
	{
		//reset bullet coordinates to player coordinates
		if(bullet_position.y > position.y)
			bullet_position.y = position.y;

		//reset bullet position once it reaches the end of the window
		if(bullet_position.y < 0)
			bullet = Bullet::NONE;
		
		//move bullet	
		bullet_position.y -= speed;
	}
	
	if(bullet == Bullet::NONE)
	{
		bullet_position.y = position.y + 100;
		bullet_position.x = position.x + (size.x/2);
	}	
}
