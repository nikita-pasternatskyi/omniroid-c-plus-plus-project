#include "Input.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../time/Time.h"

//GAMEPAD STICK UP IS -1, DOWN IS 1

Input::Input(GLFWwindow* window, const char* defaultInputMapPath, const char* userInputMapPath)
	:_window(window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	Input::Device LastUsedDevice = Device::KEYBOARD;
	_s_RebindTime = 0.25f;
	_s_RebindTimer = _s_RebindTime;
	LeftStickDeadZone = 0.0f;
	RightStickDeadZone = 0.0f;
	DPADDeadZone = 0.0f;
	LeftTriggerDeadZone = 0.0f;
	RightTriggerDeadZone = 0.0f;
	MouseSensitivity = 1.0f;

	_s_UIKeyMap =
		"(button)ui_move_left[g:leftStick_left,dpad_left;k:263]\n"\
		"(button)ui_move_right[g:leftStick_right,dpad_right;k:262]\n"\
		"(button)ui_move_up[g:leftStick_up,dpad_up;k:265]\n"\
		"(button)ui_move_down[g:leftStick_down,dpad_down;k:264]\n"\
		"(button)ui_accept[g:0,7;k:257,32,mouse_left]\n"\
		"(button)ui_cancel[g:1;k:256]\n";

	_s_SpecialKeysTable = std::unordered_map <std::string, int>
	{
		{ "leftStick_left",  GAMEPAD_LEFT_STICK_LEFT },			{ "leftStick_right",	GAMEPAD_LEFT_STICK_RIGHT },
		{ "leftStick_up",    GAMEPAD_LEFT_STICK_UP },			{ "leftStick_down",		GAMEPAD_LEFT_STICK_DOWN },
		{ "rightStick_left", GAMEPAD_RIGHT_STICK_LEFT },		{ "rightStick_right",	GAMEPAD_RIGHT_STICK_RIGHT },
		{ "rightStick_up",   GAMEPAD_RIGHT_STICK_UP },			{ "rightStick_down",	GAMEPAD_RIGHT_STICK_DOWN },
		{ "dpad_left",		 GLFW_GAMEPAD_BUTTON_DPAD_LEFT },	{ "dpad_right",			GLFW_GAMEPAD_BUTTON_DPAD_RIGHT },
		{ "dpad_up",		 GLFW_GAMEPAD_BUTTON_DPAD_UP },		{ "dpad_down",			GLFW_GAMEPAD_BUTTON_DPAD_DOWN },
		{ "mouse_left",		 MOUSE_LEFT },						{ "mouse_right",		MOUSE_RIGHT },
		{ "leftTrigger",	 GAMEPAD_LEFT_TRIGGER },			{ "rightTrigger",		GAMEPAD_RIGHT_TRIGGER }
	};

	_s_SettingsTable = std::unordered_map<std::string, float*>
	{
		{ "leftStick_deadZone",		 &LeftStickDeadZone}, { "rightStick_deadZone",			&RightStickDeadZone },
		{ "leftTrigger_deadZone",    &LeftTriggerDeadZone }, { "rightTrigger_deadZone",		&RightTriggerDeadZone },
		{ "dpad_deadZone",			 &DPADDeadZone }, { "mouse_sensitivity",				&MouseSensitivity }
	};

	_s_DefaultInputMapPath = defaultInputMapPath;
	std::ifstream inputFile(defaultInputMapPath);
	std::ifstream userInputFile(userInputMapPath);

	bool loadDefault = false;

	if (userInputFile.fail())
	{
		LOG_WARNING("[INPUT]::User Input file{" << userInputMapPath <<"}failed to load");
		loadDefault = true;
	}

	std::string line;

	std::istringstream uiKeys(_s_UIKeyMap);
	while (getline(uiKeys, line))
	{
		ProcessLine(line, false);
	}
	if (loadDefault) 
	{
		LOG("[INPUT]::Loading From Default File..");
		while (getline(inputFile, line))
		{
			ProcessLine(line, true);
		}
	}
	else
	{
		LOG("[INPUT]::Loading From User File..");
		while (getline(userInputFile, line))
		{
			ProcessLine(line, true);
		}
	}
}

void Input::SaveCustomInputMappings()
{
	std::ofstream outfile("data/user/input.inputMap");

	if (outfile.is_open()) 
	{
		for (auto& settingsPairs : _s_SettingsTable)
		{
			outfile << settingsPairs.first << '[' << *settingsPairs.second << ']' << std::endl;
		}
		for (auto& keyValuePair : _s_ButtonStates)
		{
			std::string name = keyValuePair.first;
			Button* button = &keyValuePair.second;

			if (button->Rebindable == false)
				continue;
			//save settings

			outfile << "(button)";
			outfile << name << "[g:";

			int gamePad = button->GamepadKeys[0];
			//save buttons https://stackoverflow.com/questions/4263640/find-mapped-value-of-map
			auto result = std::find_if(
				_s_SpecialKeysTable.begin(),
				_s_SpecialKeysTable.end(),
				[gamePad](const auto& mo) {return mo.second == gamePad; });
			std::string gamePadString;
			if (result != _s_SpecialKeysTable.end())
			{
				outfile << result->first;
			}
			else
			{
				outfile << button->GamepadKeys[0];
			}
			outfile << ";k:";

			int keyboard = button->KeyboardKeys[0];
			auto keyboardResult = std::find_if(
				_s_SpecialKeysTable.begin(),
				_s_SpecialKeysTable.end(),
				[keyboard](const auto& mo) {return mo.second == keyboard; });
			std::string keyboardString;
			if (keyboardResult != _s_SpecialKeysTable.end())
			{
				outfile << keyboardResult->first;
			}
			else
			{
				outfile << button->KeyboardKeys[0];
			}
			outfile << ']' << std::endl;
		}
		outfile.close();
	}
	LOG("[INPUT]::Custom Input Mappings were successfully saved under {" << "data/user/input.inputMap" << "}");
}

void Input::ResetInputMapToDefault()
{
	std::ifstream inputFile(_s_DefaultInputMapPath);
	std::string line;
	while (getline(inputFile, line))
	{
		ProcessLine(line, true);
	}
	LOG("[INPUT]::Input map was succesfully restore to default!");
}

void Input::Rebind(const char* buttonName)
{
	LOG("[INPUT]::Rebinding of {" << buttonName <<" } has begun!");
	if (_s_ButtonStates.at(buttonName).Rebindable == false)
		return;
	_s_RebindTimer = _s_RebindTime;
	_s_ButtonToRebind = buttonName;
	_s_Rebinding = true;
}

void Input::Update()
{
	glfwGetCursorPos(_window, &MouseX, &MouseY);
	//if becomes a bottleneck, can be optimized
	for (int key = GLFW_KEY_SPACE; key < GLFW_KEY_LAST; key++)
	{
		int state = glfwGetKey(_window, key);
		switch (state)
		{
			case GLFW_PRESS:
				ChangeDevice(Device::KEYBOARD, key);
				_s_PressedKeyboardButtons[key] = true;
				break;
			case GLFW_RELEASE:
				_s_PressedKeyboardButtons[key] = false;
				break;
		}
	}

	for (int mouseKey = GLFW_MOUSE_BUTTON_1; mouseKey < GLFW_MOUSE_BUTTON_LAST; mouseKey++)
	{
		int state = glfwGetMouseButton(_window, mouseKey);
		switch (state)
		{
		case GLFW_PRESS:
			ChangeDevice(Device::KEYBOARD, CONVERT_MOUSE_TO_GLFW_MOUSE(mouseKey));
			_s_PressedKeyboardButtons[CONVERT_MOUSE_TO_GLFW_MOUSE(mouseKey)] = true;
			break;
		case GLFW_RELEASE:
			_s_PressedKeyboardButtons[CONVERT_MOUSE_TO_GLFW_MOUSE(mouseKey)] = false;
			break;
		}
	}

	if (_s_RebindTimer >= 0 && _s_Rebinding)
	{
		_s_RebindTimer -= Time::DeltaTime;
		if (_s_RebindTimer < 0)
			_s_RebindTimer = 0;
	}

	GLFWgamepadstate state;

	if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
	{
		for (int i = 0; i < GLFW_GAMEPAD_LAST_BUTTON; i++)
		{
			if (state.buttons[i] == GLFW_PRESS) 
			{
				ChangeDevice(Device::GAMEPAD, i);
				break;
			}
		}

		for (int axis = 0; axis < 5; axis++)
		{
			bool change = axis != 4 ? abs(state.axes[axis]) >= 0.2 : abs(state.axes[axis]) < 0.9;
			if (change)
			{
				int idx = 0;
				switch (axis)
				{
					case 0:
						if (state.axes[axis] < 0)
							idx = GAMEPAD_LEFT_STICK_LEFT;
						else if (state.axes[axis] > 0)
							idx = GAMEPAD_LEFT_STICK_RIGHT;
						break;	
					case 1:
						if (state.axes[axis] < 0)
							idx = GAMEPAD_LEFT_STICK_UP;
						else if (state.axes[axis] > 0)
							idx = GAMEPAD_LEFT_STICK_DOWN;
						break;
					case 2:
						if (state.axes[axis] < 0)
							idx = GAMEPAD_RIGHT_STICK_LEFT;
						else if (state.axes[axis] > 0)
							idx = GAMEPAD_RIGHT_STICK_RIGHT;
						break;
					case 3:
						if (state.axes[axis] < 0)
							idx = GAMEPAD_RIGHT_STICK_UP;
						else if (state.axes[axis] > 0)
							idx = GAMEPAD_RIGHT_STICK_DOWN;
						break;
					case 4:
						idx = GAMEPAD_LEFT_TRIGGER;
						break;
					case 5:
						idx = GAMEPAD_RIGHT_TRIGGER;
						break;
				}
				ChangeDevice(Device::GAMEPAD, idx);
				break;
			}
		}
	}

	for (auto& keyValue : _s_ButtonStates)
	{
		Button* button = &keyValue.second;
		if (button->Pressed)
			button->JustPressed = false;
		else if (!button->Pressed)
			button->JustReleased = false;
	}

	for (auto& keyValuePair : _s_ButtonStates)
	{
		Button* button = &keyValuePair.second;
		bool keyboard = false;
		bool gamepad = false;
		for (auto& keyCode : button->KeyboardKeys)
		{
			if (_s_PressedKeyboardButtons.find(keyCode) == _s_PressedKeyboardButtons.end())
			{
				continue;
			}
			if (_s_PressedKeyboardButtons.at(keyCode) == true)
			{
				keyboard = true;
				break;
			}
		}

		if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
		{
			for (auto& gamePadKey : button->GamepadKeys)
			{
				switch (gamePadKey)
				{
				default:
					switch (state.buttons[gamePadKey])
					{
					case GLFW_PRESS:
						gamepad = true;
						break;
					case GLFW_RELEASE:
						break;
					}
					break;
				case GAMEPAD_LEFT_STICK_DOWN:
					gamepad = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] >= LeftStickDeadZone ? true : gamepad;
					break;
				case GAMEPAD_LEFT_STICK_LEFT:
					gamepad = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] >= LeftStickDeadZone ? true : gamepad;
					break;
				case GAMEPAD_LEFT_STICK_RIGHT:
					gamepad = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] >= LeftStickDeadZone ? true : gamepad;
					break;
				case GAMEPAD_LEFT_STICK_UP:
					gamepad = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] >= LeftStickDeadZone ? true : gamepad;
					break;
				case GAMEPAD_RIGHT_STICK_DOWN:
					gamepad = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] >= RightStickDeadZone ? true : gamepad;
					break;
				case GAMEPAD_RIGHT_STICK_LEFT:
					gamepad = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] >= RightStickDeadZone ? true : gamepad;
					break;
				case GAMEPAD_RIGHT_STICK_RIGHT:
					gamepad = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] >= RightStickDeadZone ? true : gamepad;
					break;
				case GAMEPAD_RIGHT_STICK_UP:
					gamepad = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] >= RightStickDeadZone ? true : gamepad;
					break;
				case GAMEPAD_RIGHT_TRIGGER:
					gamepad = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] >= 0 ? true : gamepad;
					break;
				case GAMEPAD_LEFT_TRIGGER:
					gamepad = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] >= 0 ? true : gamepad;
					break;
				}
				if (gamepad)
					break;
			}
		}
		else 
		{
			gamepad = keyboard;
		}

		if (gamepad || keyboard)
			ProcessButtonPress(button);

		if (gamepad == false && keyboard == false)
			ProcessButtonRelease(button);
	}
}

void Input::ChangeDevice(Device newDevice, int cause)
{
	if (_s_RebindTimer == 0 && _s_Rebinding)
	{
		if (newDevice == Device::KEYBOARD)
			_s_ButtonStates[_s_ButtonToRebind].KeyboardKeys[0] = cause;
		else if(newDevice == Device::GAMEPAD)
			_s_ButtonStates[_s_ButtonToRebind].GamepadKeys[0] = cause;
		_s_Rebinding = false;
		LOG("[INPUT]::{" << _s_ButtonToRebind << "} was rebinded to {" << cause << "} Successfully!");
	}
	if (newDevice == LastUsedDevice)
		return;
	LastUsedDevice = newDevice;
}

void Input::ProcessButtonPress(Button* button)
{
	if (button->Pressed)
		return;
	button->Pressed = true;
	button->JustPressed = true;
}

void Input::ProcessButtonRelease(Button* button)
{
	if (!button->Pressed)
		return;
	button->JustReleased = true;
	button->Pressed = false;
}

float Input::GetSettingValueFromStringLine(std::string& line)
{
	int readStart = line.find_first_of('[') + 1;
	return std::stof(line.substr(readStart, line.size() - readStart - 1));
}

void Input::ProcessLine(const std::string& line, bool rebindable)
{
	auto npos = std::string::npos;
	int nameEndIndex = line.find_first_of('[');
	int nameStartIndex = line.find_first_of(')');

	// all input settings (mouse sensitivity, deadzones)
	if (nameStartIndex == npos && nameEndIndex != npos)
	{
		std::string parameterName = line.substr(0, nameEndIndex);
		int valueEndIndex = line.find_last_of(']');
		*_s_SettingsTable.at(parameterName) = stof(line.substr(nameEndIndex + 1, valueEndIndex - nameEndIndex - 1));
		return;
	}

	// add buttons
	if (nameEndIndex != npos && nameStartIndex != npos)
	{
		std::vector<int> gamePadKeys;
		std::vector<int> keyboardKeys;
		std::string type = line.substr(0, nameStartIndex + 1);
		std::string name = line.substr(nameStartIndex + 1, nameEndIndex - nameStartIndex - 1);
		Button::ButtonType buttonType;

		if (type.find("value") != npos)
			buttonType = Button::ButtonType::VALUE;
		else if (type.find("button") != npos)
			buttonType = Button::ButtonType::BUTTON;

		int start = line.find("g:") + 2;
		std::stringstream gamePadKeysString = std::stringstream(line.substr(start, line.find_first_of(";") - start));

		int keyboardStart = line.find("k:") + 2;
		std::stringstream keyboardKeysString = std::stringstream(line.substr(keyboardStart, line.find_last_of(']') - keyboardStart));

		std::string currentKey;
		while (getline(gamePadKeysString, currentKey, ','))
		{
			if (_s_SpecialKeysTable.find(currentKey) != _s_SpecialKeysTable.end())
			{
				gamePadKeys.push_back(_s_SpecialKeysTable.at(currentKey));
				continue;
			}
			gamePadKeys.push_back(stof(currentKey));

		}
		while (getline(keyboardKeysString, currentKey, ','))
		{
			if (_s_SpecialKeysTable.find(currentKey) != _s_SpecialKeysTable.end())
			{
				keyboardKeys.push_back(_s_SpecialKeysTable.at(currentKey));
				continue;
			}
			keyboardKeys.push_back(stof(currentKey));
		}
		_s_ButtonStates[name] = { buttonType, name, keyboardKeys, gamePadKeys, rebindable, false,false,false };
	}
}