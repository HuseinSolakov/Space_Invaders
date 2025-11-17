#include "Time.h"

//Get Delta time 
float Time::GetDeltaTime()
{	
	//get deltatime
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	return deltaTime;
}

//Create a timer
bool Time::Timer(float time)
{
	if(done)
	{
	  this->time = glfwGetTime();
	  done = false;
	}
	
	float current_time = glfwGetTime();
	
	if(current_time > this->time + time)
	{
		done = true;
		return true;
	}
	else 
		return false;
}
