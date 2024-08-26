#pragma once
class Time
{

public:
	static float DeltaTime;
	static float TimeSinceStartup;
	static void Update();
	
private:
	static float LastTimeStep;
};