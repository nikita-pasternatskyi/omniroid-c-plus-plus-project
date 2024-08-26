#pragma once

struct Button
{
public:
	enum ButtonType
	{
		BUTTON, VALUE
	};
	ButtonType Type;
	std::string Name;
	std::vector<int> KeyboardKeys;
	std::vector<int> GamepadKeys;
	bool Rebindable;
	bool Pressed;
	bool JustPressed;
	bool JustReleased;
};