#ifndef TIME_H
#define TIME_H

#include<GLFW/glfw3.h>

class Time
{
public:
	float GetDeltaTime();
	bool Timer(float time);
	
private:
	//delta time
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	//timer
	bool done = true;
	float time;
};

#endif
