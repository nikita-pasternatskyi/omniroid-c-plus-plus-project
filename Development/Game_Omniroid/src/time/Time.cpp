#include "Time.h"
#include <GLFW/glfw3.h>

float Time::LastTimeStep;
float Time::TimeSinceStartup;
float Time::DeltaTime;

void Time::Update()
{
	DeltaTime = glfwGetTime() - LastTimeStep;
	LastTimeStep = glfwGetTime();
	TimeSinceStartup = glfwGetTime();
}
