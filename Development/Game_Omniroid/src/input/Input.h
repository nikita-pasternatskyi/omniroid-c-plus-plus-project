#pragma once
#include "../debug/DebugLogger.h"
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>
#include "Button.h"

#define GLFW_GAMEPAD_LAST_BUTTON	GLFW_GAMEPAD_BUTTON_DPAD_LEFT  
#define GAMEPAD_LEFT_STICK_LEFT		15
#define GAMEPAD_LEFT_STICK_RIGHT	GAMEPAD_LEFT_STICK_LEFT + 1
#define GAMEPAD_LEFT_STICK_UP		GAMEPAD_LEFT_STICK_RIGHT + 1
#define GAMEPAD_LEFT_STICK_DOWN		GAMEPAD_LEFT_STICK_UP + 1
#define GAMEPAD_RIGHT_STICK_LEFT	GAMEPAD_LEFT_STICK_DOWN + 1
#define GAMEPAD_RIGHT_STICK_RIGHT	GAMEPAD_RIGHT_STICK_LEFT + 1
#define GAMEPAD_RIGHT_STICK_UP		GAMEPAD_RIGHT_STICK_RIGHT + 1
#define GAMEPAD_RIGHT_STICK_DOWN	GAMEPAD_RIGHT_STICK_UP + 1
#define GAMEPAD_RIGHT_TRIGGER		GAMEPAD_RIGHT_STICK_DOWN + 1
#define GAMEPAD_LEFT_TRIGGER		GAMEPAD_RIGHT_TRIGGER + 1
#define MOUSE_LEFT					-(GLFW_MOUSE_BUTTON_LEFT+1)
#define MOUSE_RIGHT					-(GLFW_MOUSE_BUTTON_RIGHT+1)
#define CONVERT_MOUSE_TO_GLFW_MOUSE(x) -(x+1)

class Input
{
public:
	Input(GLFWwindow* window, const char* defaultInputMapPath, const char* userInputMapPath);

	enum Device
	{
		GAMEPAD, KEYBOARD
	};
	Device LastUsedDevice;
	double MouseX;
	double MouseY;
	float MouseSensitivity;
	float LeftStickDeadZone;
	float RightStickDeadZone;
	float DPADDeadZone;
	float LeftTriggerDeadZone;
	float RightTriggerDeadZone;
	void SaveCustomInputMappings();
	void ResetInputMapToDefault();
	void Rebind(const char* buttonName);
	void Update();
	inline Button* GetButtonState(const char* name) { return &_s_ButtonStates.at(name); }
private:
	GLFWwindow* _window;
	std::string _s_DefaultInputMapPath;
	std::string _s_UIKeyMap;
	std::unordered_map<std::string, std::string> _s_RebindedButtons;
	std::unordered_map<std::string, Button> _s_ButtonStates;
	std::unordered_map<std::string, int> _s_SpecialKeysTable;
	std::unordered_map<std::string, float*> _s_SettingsTable;
	std::unordered_map<int, bool> _s_PressedKeyboardButtons;
	std::string _s_ButtonToRebind;
	bool _s_Rebinding;
	float _s_RebindTime;
	float _s_RebindTimer;
	void ChangeDevice(Device newDevice, int cause);
	void ProcessButtonPress(Button* button);
	void ProcessButtonRelease(Button* button);
	float GetSettingValueFromStringLine(std::string& line);
	void ProcessLine(const std::string& line, bool rebindable);
};