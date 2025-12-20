#include "EntityBehaviour.h"

using namespace std;
//Player
//constructors
Player::Player(glm::vec2 position, glm::vec2 size, glm::vec3 color)
{
	//properties
	this->bullet_position = glm::vec2(0.0f, 0.0f);
	this->position = position;
	this->rotation = 0.0f;
	this->color = color;
	this->size = size;
	
	//enums
	move_player = Movement::NONE;
	bullet = Bullet::NONE;
	
	//sprite
	sprite_location = 1;
	counter = 0;
	frames = 1;

	//status
	status.alive = true;
	status.hit = false;
	status.lives = 3;
}

Player::Player(glm::vec2 position,  glm::vec2 size, float rotation, glm::vec3 color)
{
	this->bullet_position = glm::vec2(0.0f, 0.0f);
	this->position = position;
	this->rotation = rotation;
	this->color = color;
	this->size = size;

	

	move_player = Movement::NONE;
	bullet = Bullet::NONE;
	
	sprite_location = 1;
	counter = 0;
	frames = 1;
	
	//status
	status.alive = true;
	status.hit = false;
	status.lives = 3;
}

//methods
void Player::UpdatePlayer()
{
	if(status.lives == 0)
	   status.alive = false;	
}

void Player::Hit_Animation(SpriteRenderer   *sprite_renderer, float duration, int sprite_row)
{
	bool animation = timer[0].Timer(duration);
	
  	if(status.hit && status.alive)
	{
	  	switch(animation)
	  	{
	  	  case 1: sprite_renderer->SetSpriteLocation(1, sprite_row); counter++; break;
	  	  case 0: sprite_renderer->SetSpriteLocation(7, sprite_row); animation = true; break;
	  	}
	  	
	  	if(counter == 3)
	  	{
	    	  counter = 0;
	    	  status.hit = false;
	    	  sprite_renderer->SetSpriteLocation(1, sprite_row);
	 	}
	}
}

//Player Animation
void Player::Animation(SpriteRenderer   *sprite_renderer, float duration, int sprite_row)
{
  //timer
  bool animation = timer[1].Timer(duration);
  if(status.alive)
  {
	//if the player is moving
	if(move_player != Movement::NONE)
	{
		if(animation)
		{
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
  
  else if(!status.alive)
  {
  	if(this->frames < 10)
	{
		if(animation)
		{
		this->frames++;
		sprite_renderer->SetSpriteLocation(this->frames, sprite_row+1); //death sprites should be directly below player sprites
		}
		
	}
	else
	  sprite_renderer->SetSpriteLocation(0, 0);
  }
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
  if(status.alive) 
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
}

//Bullet Animation
void Player::BulletAnimation(SpriteRenderer   *sprite_renderer, int frames, int sprite_row, float duration)
{
	//timer
	bool animation = timer[2].Timer(duration);
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
	this->move[0] = Movement::RIGHT;
	this->move[1] = Movement::NONE;
	
	this->enemy_speed.X_speed = 1.0f;
	this->enemy_speed.Y_speed = 1.0f;
	this->enemy_speed.acceleration = 1.0f;
	
	counter = 0;

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
	this->move[0] = Movement::RIGHT;
	this->move[1] = Movement::NONE;
	
	this->enemy_speed.X_speed = 1.0f;
	this->enemy_speed.Y_speed = 1.0f;
	this->enemy_speed.acceleration = 1.0f;
	
	counter = 0;	

	//setup enemy positions
	SetPositions();
}

//Draw enemies
							///* ShapeRenderer *test,*/
void Enemy::DrawEnemies(SpriteRenderer   *sprite_renderer,Texture2D &texture, int frames, int sprite_row)
{
	int enemy = sprite;
	//draw enemies
	for(int x = 0; x < 5 ; x++)
	{
		for(int y = 0; y < 11; y++)
		{
			sprite_renderer->SetSpriteLocation(enemy, sprite_row);
			if(enemy_positions[x][y].z)
				sprite_renderer->DrawSprite(texture, enemy_positions[x][y], this->size, rotation, color);	
		}
		// draw enemies
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
      if(enemy_positions[i][j].z)
      {
      	 //enemy hit detection
         if(player->bullet_position.x > enemy_positions[i][j].x - this->size.x/2 && 
            player->bullet_position.x < enemy_positions[i][j].x + this->size.x/2)
	     if(player->bullet_position.y > enemy_positions[i][j].y && player->bullet_position.y < enemy_positions[i][j].y + this->size.y)
	  	{
	  		this->enemy_speed.X_speed += this->enemy_speed.acceleration;
			enemy_positions[i][j].z = false;
			player->bullet = Bullet::NONE;
	  	}
	  //enemy player collision
	  if(enemy_positions[i][j].x > player->position.x - player->size.x && enemy_positions[i][j].x < player->position.x + 
	  player->size.x)
	      if(enemy_positions[i][j].y > player->position.y - player->size.y / 2)
	      player->status.alive = false;
	      
	  //enemy getting past player
	  if(enemy_positions[i][j].y > player->position.y + this->size.y)
	      player->status.alive = false;
	}
    }
}

void Enemy::SetSpeed(float X_speed, float Y_speed, float acceleration)
{
	this->enemy_speed.X_speed = X_speed;
	this->enemy_speed.Y_speed = Y_speed;
	this->enemy_speed.acceleration = acceleration;
}

void Enemy::EnemyMovement(int border_x, int border_y, float deltaTime)
{
	
	float move_down = 0.0f;
			
	if(move[1] == Movement::DOWN)
	{
	  move_down = enemy_speed.Y_speed;
	  enemy_speed.X_speed += enemy_speed.acceleration;
	  move[1] = Movement::NONE;
	}

	for(int i=0; i<5; i++)
	{
		for(int j=0; j<11; j++) 
		{
		  	if(enemy_positions[i][j].z)
			//check if enemies have reached right side border
			if(enemy_positions[i][j].x >= border_x - size.x)
			{
				//set enemy movement to left and down once
				move[0] = Movement::LEFT;
				move[1] = Movement::DOWN;
			}
			
			//check if enemies have reached left side border
			else if(enemy_positions[i][j].x <= 0)
			{
				//set enemy movement to right and down once
				move[0] = Movement::RIGHT;
				move[1] = Movement::DOWN;
			}
			
			//move enemies
			switch(move[0])
			{
				case Movement::RIGHT: enemy_positions[i][j].x += enemy_speed.X_speed * deltaTime;	break;
				case Movement::LEFT:  enemy_positions[i][j].x -= enemy_speed.X_speed * deltaTime;	break;
			}
			
			enemy_positions[i][j].y += move_down;	
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
			enemy_positions[i][j] = glm::vec3(position.x + distance.x, position.y + distance.y, true);
			//bullet_pos[i][j] = glm::vec3(0.0f,0.0f, false);
			
			distance.x += 45.0f;
		}
		
		distance.x = 0.0f;
		distance.y += 45.0f;
		
		bullet_pos[i] = glm::vec3(0.0f,0.0f, false);
	}
	
	distance.y = 0.0f;
}

//Render Enemy bullet
void Enemy::Render_Bullet(ShapeRenderer *shapeshader)
{
//choose random enemy between 0 and 10
int index_x = rand() % 11;
int index_y = 4;

//only allow 3 bullets max
if(counter < 3) 
{
  //check if enemy at current row is alive
  while(enemy_positions[index_y][index_x].z != true)
  {
    if(index_y < 0 )
    break;
    
    else
    index_y--;
  }

  if(enemy_positions[index_y][index_x].z == true)
    if(bullet_pos[counter].z == false)
    {
      //random chance to shoot
      int random = rand() % 100;
      if(random == 1)
      {
        //storing it here
        bullet_pos[counter] = enemy_positions[index_y][index_x];
        bullet_pos[counter].z = true;
        counter++;
      }
    }
}

//render the bullets
for(int i = 0; i < 4; i++)
  if(bullet_pos[i].z == true)
    shapeshader->DrawLine(glm::vec2(bullet_pos[i].x+this->size.x/2, bullet_pos[i].y+this->size.y), 20);
}

//update bullet position and status
void Enemy::Update_Bullet(float speed, float deltatime, int Window_height, Player *player)
{
	for(int i = 0; i < 4; i++)
	{
	  if(bullet_pos[i].z == true)
	  {
	  	bullet_pos[i].y += speed * deltatime;
	  	if(bullet_pos[i].y > Window_height)
	  	{
	  		bullet_pos[i].z = false;
	  		counter--;
	  	}
	  	
	  	if(bullet_pos[i].x > player->position.x - player->size.x/2 && bullet_pos[i].x < player->position.x + player->size.x/2)
		  if(bullet_pos[i].y > player->position.y - player->size.y / 2)
	  	  {
		    //decrease player lifes
		    if(player->status.lives > 0)
		    {
		      player->status.lives --;
		      player->status.hit = true;
		      bullet_pos[i].z = false;
		    }
	  	  }
	  }
	}
}
