#include "EntityBehaviour.h"

//Player
//constructors
Player::Player(glm::vec2 position, glm::vec2 size, glm::vec3 color)
{
	this->position = position;
	this->rotation = 0.0f;
	this->color = color;
	this->size = size;
	
	move_player = Movement::NONE;
	bullet = Bullet::NONE;
	
	sprite_location = 1;
	frames = 1;		//temporary solution
}

Player::Player(glm::vec2 position,  glm::vec2 size, float rotation, glm::vec3 color)
{
	this->position = position;
	this->rotation = rotation;
	this->color = color;
	this->size = size;

	move_player = Movement::NONE;
	bullet = Bullet::NONE;
	
	sprite_location = 1;
	frames = 1;		//temporary solution
}

//methods
//Player Animation
void Player::Animation(SpriteRenderer   *sprite_renderer, float duration, int sprite_row)
{
	//timer
	bool animation = time.Timer(duration);
	//if the player is moving
	if(move_player != Movement::NONE)
	{
	
		if(animation)
		{
			//std::cout<<"check"<<std::endl;  	  
	  		sprite_renderer->SetSpriteLocation(sprite_location, sprite_row);
	  		sprite_location ++;
  
			if(sprite_location > 6)
				sprite_location = 2;
		}
	}
	//if the player is not moving
	else
		sprite_renderer->SetSpriteLocation(1, sprite_row);
}

//Draw Player
void Player::DrawPlayer(SpriteRenderer   *sprite_renderer, Texture2D &texture)
{
	sprite_renderer->DrawSprite(texture, position, size, rotation, color);
}

//Check if player is at window's border
void Player::BorderCheck(int window_width)
{
	//if player hits right  side
	if(position.x > window_width - size.x)
		position.x = window_width - size.x;
		
	//if player hits left side	
	if(position.x < 0)
		position.x = 0;
}
//swap borders if player at window's border
void Player::BorderSwap(int window_width)
{
	//if player hits right  side
	if(position.x > window_width)
		position.x = -size.x;
		
	//if player hits left side	
	if(position.x < -size.x)
		position.x = window_width;
}

//Shoot player bullet
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
		bullet_position.x = position.x + (size.x/3);
	}	
}

//Bullet Animation
void Player::BulletAnimation(SpriteRenderer   *sprite_renderer, int frames, int sprite_row, float duration)
{
	//timer
	bool animation = time.Timer(duration);
	if(bullet == Bullet::SHOOT)
	{
		if(animation && this->frames <= frames)
		{
		sprite_renderer->SetSpriteLocation(frames, sprite_row);
		frames++;
		}	
	}
	else
	{
		sprite_renderer->SetSpriteLocation(1, sprite_row);
		frames = 1;
	}
}

//Enemy
//constructors
Enemy::Enemy(glm::vec2 position,  glm::vec2 size,    glm::vec2 distance, glm::vec3 color)
{	
	this->position = position;
	this->distance = distance;
	this->rotation = 0.0f;
	this->color = color;
	this->size = size;
	
	this->sprite = 1;
	//setup enemy positions
	SetPositions();
}

Enemy::Enemy(glm::vec2 position,  glm::vec2 size,    glm::vec2 distance, float rotation, glm::vec3 color)
{
	this->position = position;
	this->distance = distance;
	this->rotation = rotation;
	this->color = color;
	this->size = size;

	this->sprite = 1;
	//setup enemy positions
	SetPositions();
}

//Draw enemies
void Enemy::DrawEnemies(SpriteRenderer   *sprite_renderer, Texture2D &texture, int frames, int sprite_row)
{
	int enemy = sprite;
	//draw enemies
	for(int x = 0; x < 5 ; x++)
	{
		for(int y = 0; y < 11; y++)
		{
			sprite_renderer->SetSpriteLocation(enemy, sprite_row);
			if(render_enemy[x][y])
				sprite_renderer->DrawSprite(texture, enemy_positions[x][y], this->size, rotation, color);
		}
		
		if(x == 0 || x == 2)
				enemy += frames;
	}
}

//Enemy animation
void Enemy::EnemyAnimation(float duration, int frames)
{	
	bool animation = time.Timer(duration);
	if(animation)
	{
	     if(sprite < frames)
	     {
	     	sprite++;
	     }
	     else
	     {
	     	sprite = 1;
	     }
	}	
}

//Check if enemies get hit
void Enemy::EnemyHitDetection(Player *player)
{
  for(int i=0; i<5; i++)			
    for(int j=0; j<11; j++)	
    {
      if(render_enemy[i][j])
         if(player->bullet_position.x > enemy_positions[i][j].x && player->bullet_position.x < enemy_positions[i][j].x + this->size.x)
	     if(player->bullet_position.y > enemy_positions[i][j].y && player->bullet_position.y < enemy_positions[i][j].y + this->size.y)
	  	{
			render_enemy[i][j] = false;
			player->bullet = Bullet::NONE;
	  	}
    }
}

//setup enemy positions
void Enemy::SetPositions()
{
	for(int i=0; i<5; i++)			
	{
		for(int j=0; j<11; j++)	
		{
			enemy_positions[i][j] = glm::vec2(position.x + distance.x, position.y + distance.y);
			this->render_enemy[i][j] = true;
			distance.x += 45.0f;
		}
		
		distance.x = 0.0f;
		distance.y += 45.0f;
	}
	
	distance.y = 0.0f;
}
