#pragma once
#include "../audio/SoundEffect.h"
#include "entt/entt.hpp"
#include "Screen.h"
#include "../json/JSONReader.h"
#include "../gameWorld/systems/ISystem.h"
#include "UIScreen.h"
#include "LevelCompletionStatus.h"

class Input;
class Renderer;
class Game;
class Audio;
class Material;
struct TransformComponent;


class Demo
{

public:

	Demo(Game& game, Renderer& renderer, Audio& audio, Input& input);

	template<typename Type, typename... Args>
	void AddSystem(Args &&...args)
	{
		auto system = new Type(std::forward<Args>(args)...);
		_systems.push_back((ISystem*)system);
		LOG("[WORLD]::System addition complete!");
	};

	entt::entity CreateEntity();

	template<typename Type, typename... Args>
	decltype(auto) AddComponent(const entt::entity& entity, Args &&...args)
	{
		return _registry.emplace<Type>(entity, args...);
	}

	LevelCompletionStatus Status;
	void Update();

	//=================MAIN MENU=================//

	entt::entity CreateUISprite(const Renderer& renderer, Material* material, glm::vec3 position);


	//===================LEVEL====================//

	void ReturnToMainMenu();
	entt::entity CreateMonster(int hp, float x, float y, const char* animation);


protected:
	entt::entity CreateCamera(const glm::vec2& aspect, float orthoScale);
	std::vector<ISystem*>		_systems;
	entt::registry				_registry;

private:
	Game& _game;
	Renderer& _renderer;
	Audio& _audio;
	Input& _input;
	Screen _currentScreen;

	void ChangeScreen(Screen newScreen);

	//=================MAIN MENU=================//

	void CreateMainScreen();
	SoundEffect* _uiClick;
	SoundEffect* _uiMove;
	SoundEffect* _bgMusic;
	TransformComponent* _handTransform;
	Material* _transitionMaterial;
	Material* _handMaterial;
	Material* _tabletOverlayMaterial;
	Material* _tabletMaterial;
	Material* _overlayMaterial;
	Material* _exitButtonMaterial;
	Material* _optionsButtonMaterial;
	Material* _playButtonMaterial;
	Material* _titleMaterial;
	Material* _backgroundLinesMaterial;
	Material* _backgroundMaterial;
	bool _loadLevel;


	//===================LEVEL====================//

	UIScreen _currentUIScreen = UIScreen::NONE;
	entt::entity _pauseMenu;
	entt::entity _camera;


	void CreateLevel();
	nlohmann::json_abi_v3_11_3::basic_json<nlohmann::json_abi_v3_11_3::ordered_map> _levelData;


	Material* _thankYouMaterial;
	Material* _gameOverMaterial;
	Material* _pauseMaterial;

	Material* _playerMaterial;
	Material* _crateMaterial;
	Material* _monsterMaterial;
	Material* _monsterCapsuleMaterial;
	Material* _dustMaterial;

	Material* _cityMaterial;
	Material* _foreGroundMaterial;
	Material* _pipesMaterial;
	Material* _pipes2Material;
	Material* _levelBackgroundMaterial;
	Material* _middleGroundMaterial;
	void CreateCollisionRect(Renderer& renderer, int x, int y, int pixelWidth, int pixelHeight);
	void CreateLight(glm::vec3 position, glm::vec3 color, float intensity, bool castRays, float sineInfluence);
	Material* CreateDecalMaterial(const std::string& name, const glm::vec4& tint, float lightAbsorption);
	void CreateDecal(Renderer& renderer, Material* material, const glm::vec3& position);

};
