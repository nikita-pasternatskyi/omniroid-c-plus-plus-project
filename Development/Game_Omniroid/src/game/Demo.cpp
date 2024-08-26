#include "Demo.h"
#include "../rendering/materials/DefaultLitMaterial.h"
#include "../rendering/materials/SimpleMaterial.h"
#include "../rendering/materials/MainMenuUIScrollMaterial.h"
#include "../rendering/materials/SimpleUIMaterial.h"
#include "../gameWorld/systems/AudioListenerComponent.h"
#include "../gameWorld/systems/CameraComponent.h"
#include "../gameWorld/systems/TransformComponent.h"
#include "../gameWorld/systems/SpriteRendererComponent.h"
#include "../gameWorld/systems/ForwardRendererComponent.h"
#include "UIButton.h"
#include "../time/Time.h"
#include "../Game.h"

#include "../gameWorld/systems/LightComponent.h"
#include "../gameWorld/systems/animatedSprite/AnimatedSpriteRendererComponent.h"
#include "../gameWorld/systems/collisions/RectCollider.h"
#include "player/PlayerComponent.h"
#include "player/PlayerSystem.h"
#include "CrateComponent.h"
#include "FractionParticleSystem.h"
#include "monster/MonsterCapsuleComponent.h"


#define COMMON_LEVEL_ASSET_PATH "assets/level/"
#define COMMON_LEVEL_ASSET_ALBEDO_PATH "assets/level/albedo/"
#define COMMON_LEVEL_ASSET_NORMAL_PATH "assets/level/normal/"
#define COMMON_LEVEL_ASSET_PARAMS_PATH "assets/level/params/"

Demo::Demo(Game& game, Renderer& renderer, Audio& audio, Input& input)
    : _renderer(renderer), _input(input), _game(game), _audio(audio)
{
    //==========================MAIN MENU==========================//

    _uiMove = new SoundEffect(_audio, "event:/UI/ui_move", 0);
    _uiClick = new SoundEffect(_audio, "event:/UI/ui_accept", 0);
    _bgMusic = new SoundEffect(_audio, "event:/Music/Title", 0);

    _transitionMaterial = new SimpleMaterial(glm::vec4(0), "");
    _handMaterial = new SimpleMaterial(glm::vec4(1), "assets/ui/main_menu/hand.png");
    _tabletOverlayMaterial = new SimpleMaterial(glm::vec4(0.557, 0.91, 0.38, 1.0), "assets/ui/main_menu/tablet_overlay.png");
    _tabletMaterial = new SimpleMaterial(glm::vec4(1), "assets/ui/main_menu/tablet.png");
    _overlayMaterial = new SimpleMaterial(glm::vec4(1), "assets/ui/main_menu/global_overlay.png");
    _exitButtonMaterial = new SimpleMaterial(glm::vec4(1), "assets/ui/main_menu/EXIT.png");
    _optionsButtonMaterial = new SimpleMaterial(glm::vec4(1), "assets/ui/main_menu/OPTIONS.png");
    _playButtonMaterial = new SimpleMaterial(glm::vec4(1), "assets/ui/main_menu/PLAY.png");
    _titleMaterial = new SimpleMaterial(glm::vec4(0.659, 1, 0.698, 1.0), "assets/ui/main_menu/[OMNIROID].png");

    _backgroundLinesMaterial = new MainMenuUIScrollMaterial(glm::vec4(1), "assets/ui/main_menu/scan_lines_overlay.png", 0.25);
    _backgroundMaterial = new SimpleMaterial(glm::vec4(0, 1, 0, 1), "assets/ui/main_menu/scan_lines.png");

    //==============================GAME===========================//

    _levelData = JsonReader::ReadJson("assets/level/data.json");

    _monsterMaterial = new DefaultLitMaterial(
        "assets/monster/monster.png",
        "assets/monster/monster_normal.png",
        "assets/monster/monster_params.png",
        glm::vec4(1.0), 1.0);

    _playerMaterial = new DefaultLitMaterial(
        "assets/omniroid/omniroid.png",
        "assets/omniroid/omniroid_normal.png",
        "assets/omniroid/omniroid_params.png",
        glm::vec4(1.0), 2.0);

    _crateMaterial = new DefaultLitMaterial(
        "assets/level/crates/crate_albedo.png",
        "assets/level/crates/crate_normal.png",
        "assets/level/crates/crate_params.png",
        glm::vec4(1.0), 0.0);

    _monsterCapsuleMaterial = new DefaultLitMaterial(
        "assets/monster/monster_capsule.png",
        "",
        "assets/level/params/City.png",
        glm::vec4(1.0), 1.0);

    _dustMaterial = new MainMenuUIScrollMaterial(glm::vec4(1.0), "assets/level/albedo/dust.png", -.01);

    _thankYouMaterial = new SimpleUIMaterial(glm::vec4(1.0), "assets/ui/level/thank_you_screen.png");
    _gameOverMaterial = new SimpleUIMaterial(glm::vec4(1.0), "assets/ui/level/game_over_screen.png");
    _pauseMaterial = new SimpleUIMaterial(glm::vec4(1.0), "assets/ui/level/pause_screen.png");

    _cityMaterial           = CreateDecalMaterial("City.png",           glm::vec4(1.0), 1.0);
    _foreGroundMaterial     = CreateDecalMaterial("Foreground.png",     glm::vec4(1.0), 1.0);
    _pipesMaterial          = CreateDecalMaterial("Pipes.png",          glm::vec4(1.0), 1.0);
    _pipes2Material         = CreateDecalMaterial("Pipes2.png",         glm::vec4(0.75), 1.0);
    _levelBackgroundMaterial = CreateDecalMaterial("Background.png",     glm::vec4(0.5), 1.0);
    _middleGroundMaterial   = CreateDecalMaterial("Middleground.png",   glm::vec4(1.0), 1.0);

    ChangeScreen(Screen::MAIN_MENU);
}


entt::entity Demo::CreateCamera(const glm::vec2& aspect, float orthoScale)
{
    auto camera = CreateEntity();
    AddComponent<TransformComponent>(camera, glm::vec3(0, 0, 0), glm::vec2(1, 1), 0);
    AddComponent<CameraComponent>(camera, aspect, orthoScale);
    AddComponent<AudioListenerComponent>(camera);
    return camera;
}

entt::entity Demo::CreateEntity()
{
    return _registry.create();
}


void Demo::ChangeScreen(Screen newScreen)
{
    switch (newScreen)
    {
    case Screen::MAIN_MENU:
        if (_currentScreen == Screen::GAME)
        {
            _systems.clear();
            _registry.clear();
        }

        AddSystem<RenderingSystem>(_renderer);
        AddSystem<AudioSystem>(_audio);
        AddSystem<FractionParticleSystem>();
        _currentScreen = newScreen;
        CreateMainScreen();
        break;
    case Screen::GAME:
        if (_currentScreen == Screen::MAIN_MENU)
        {
            _loadLevel = false;
            _transitionMaterial->Tint.a = 0.0f;
            _registry.clear();
        }
        _bgMusic->Stop();
        _currentScreen = newScreen;
        CreateLevel();
        break;
    }
}

//==============MAIN MENU================//
void Demo::CreateMainScreen()
{
    _bgMusic->Play();

    auto cam = CreateCamera(glm::vec2(16, 9), 6);
    _renderer.AspectRatio = glm::vec2(16, 9);
    _renderer.OrthoScale = 6;
    _renderer.ProjectionMatrix = _registry.get<CameraComponent>(cam).GetProjectionMatrix();

    //transitionScreen
    auto transitionScreen = CreateEntity();
    auto& spriteRenderer = AddComponent<SpriteRendererComponent>(transitionScreen, _transitionMaterial);
    auto scale = glm::vec2(3, 3);
    AddComponent<TransformComponent>(transitionScreen, glm::vec3(0, 0, 5), scale, 0);
    AddComponent<ForwardRendererComponent>(transitionScreen);

    //hand
    auto hand = CreateUISprite(_renderer, _handMaterial,glm::vec3(0, 0, 4));
    _handTransform = &_registry.get<TransformComponent>(hand);

    //tablet
    auto tabletOverlay = CreateUISprite(_renderer, _tabletOverlayMaterial, glm::vec3(0, 0, 3.5));
    auto tablet = CreateUISprite(_renderer, _tabletMaterial,glm::vec3(0, 0, 3));

    //global overlay
    auto overlay = CreateUISprite(_renderer, _overlayMaterial, glm::vec3(0, 0, 2));
    auto& overlayTransform = _registry.get<TransformComponent>(overlay);
    overlayTransform.Scale.x = 3;
    overlayTransform.Scale.y = 1.5;

    //buttons
    auto exitButton = CreateUISprite(_renderer, _exitButtonMaterial,glm::vec3(-1, -0.5, 1.1));
    //auto optionsButton = CreateUISprite(_renderer, _optionsButtonMaterial, glm::vec3(-0.7, -0.4, 1.1));
    auto playButton = CreateUISprite(_renderer, _playButtonMaterial,glm::vec3(-1, -0.2, 1.1));

    //title
    auto title = CreateUISprite(_renderer, _titleMaterial, glm::vec3(-0.4, 0.1, 1));

    std::function<void()> exitFunction([] {exit(1);});
    //std::function<void()> optionsFunction([] {});
    std::function<void()> playFunction([this] {_loadLevel = true; _bgMusic->Stop();});

    AddComponent<UIButton>(exitButton, glm::vec2(0.35, 0.1), exitFunction);
    //AddComponent<UIButton>(optionsButton, glm::vec2(0.35, 0.1), optionsFunction);
    AddComponent<UIButton>(playButton, glm::vec2(0.35, 0.1), playFunction);

    auto backGroundLines = CreateUISprite(_renderer, _backgroundLinesMaterial, glm::vec3(0));
    auto& bgLinesTransform = _registry.get<TransformComponent>(backGroundLines);
    bgLinesTransform.Scale.x = 3;
    bgLinesTransform.Scale.y = 1.5;

    auto backGround = CreateUISprite(_renderer, _backgroundMaterial, glm::vec3(0, 0, -1));
    auto& bgTransform = _registry.get<TransformComponent>(backGround);
    bgTransform.Scale.x = 3;
    bgTransform.Scale.y = 1.5;
}

void Demo::Update()
{
    for (auto sys : _systems)
    {
        sys->Update(_registry);
    }

    if (_currentScreen == Screen::GAME)
    {
        switch (Status)
        {
        case LevelCompletionStatus::NONE:
            break;
        default:
            auto& transform = _registry.get<TransformComponent>(_camera);
            if (Status == LevelCompletionStatus::LOST)
            {
                auto gameOver = CreateEntity();
                auto& screenRenderer = AddComponent<SpriteRendererComponent>(gameOver, _gameOverMaterial);
                auto screenScale = _renderer.GetRealScale(screenRenderer.Size.x, screenRenderer.Size.y);

                AddComponent<ForwardRendererComponent>(gameOver);
                AddComponent<TransformComponent>(gameOver, glm::vec3(-transform.Position.x, -transform.Position.y, 5), screenScale, 0);

                _currentUIScreen = UIScreen::GAME_OVER;
            }
            else if (Status == LevelCompletionStatus::WON)
            {
                auto gameOver = CreateEntity();
                auto& screenRenderer = AddComponent<SpriteRendererComponent>(gameOver, _thankYouMaterial);
                auto screenScale = _renderer.GetRealScale(screenRenderer.Size.x, screenRenderer.Size.y);

                AddComponent<ForwardRendererComponent>(gameOver);
                AddComponent<TransformComponent>(gameOver, glm::vec3(-transform.Position.x, -transform.Position.y, 5), screenScale, 0);

                _currentUIScreen = UIScreen::THANK_YOU;
            }

        }

        switch (_currentUIScreen)
        {
            case UIScreen::NONE:
                if (_input.GetButtonState("pause")->JustPressed)
                {
                    _currentUIScreen = UIScreen::PAUSE;
                    _pauseMenu = CreateEntity();
                    auto& screenRenderer = AddComponent<SpriteRendererComponent>(_pauseMenu, _pauseMaterial);
                    auto screenScale = _renderer.GetRealScale(screenRenderer.Size.x, screenRenderer.Size.y);

                    auto& transform = _registry.get<TransformComponent>(_camera);
                    AddComponent<ForwardRendererComponent>(_pauseMenu);
                    AddComponent<TransformComponent>(_pauseMenu, glm::vec3(-transform.Position.x, -transform.Position.y, 5), screenScale, 0);
                }
                break;

            case UIScreen::PAUSE:
                if (_input.GetButtonState("pause")->JustPressed)
                {
                    _registry.destroy(_pauseMenu);
                    _currentUIScreen = UIScreen::NONE;
                }

                if (_input.GetButtonState("decline")->JustPressed)
                {
                    ChangeScreen(Screen::MAIN_MENU);
                }
                break;

            case UIScreen::THANK_YOU:
                if (_input.GetButtonState("accept")->JustPressed)
                {
                    ChangeScreen(Screen::MAIN_MENU);
                }
                break;

            case UIScreen::GAME_OVER:
                if (_input.GetButtonState("accept")->JustPressed)
                {
                    ChangeScreen(Screen::MAIN_MENU);
                }
                break;
        }
        return;
    }

    auto view = _registry.view<TransformComponent, SpriteRendererComponent, UIButton>();
    for (auto ent : view)
    {
        auto [transform, spriteRenderer, uiButton] = view.get<TransformComponent, SpriteRendererComponent, UIButton>(ent);

        auto pos = glm::vec2(_input.MouseX / _renderer.ScreenSize, _input.MouseY / _renderer.ScreenSize);

        auto worldPos = glm::unProject(glm::vec3(pos.x, pos.y, 0), glm::mat4(1), _renderer.ProjectionMatrix, glm::vec4(0, 0, 480, 270));
        worldPos.y *= -1;
        float left = transform.Position.x - uiButton.Size.x;
        float right = transform.Position.x + uiButton.Size.x;
        float up = transform.Position.y - uiButton.Size.y;
        float down = transform.Position.y + uiButton.Size.y;

        if (worldPos.y < down && worldPos.y > up
            && worldPos.x < right && worldPos.x > left)

        {
            if (uiButton.Selected == false)
            {
                _uiMove->Play();
                ((SimpleMaterial*)spriteRenderer.SpriteMaterial)->Tint = glm::vec4(1);
                uiButton.Selected = true;
            }
            if (uiButton.Selected && _input.GetButtonState("ui_accept")->JustPressed == true)
            {
                _uiClick->Play();
                uiButton.Callback();
            }
            continue;
        }
        uiButton.Selected = false;
        ((SimpleMaterial*)spriteRenderer.SpriteMaterial)->Tint = glm::vec4(0.65);
    }

    if (_loadLevel)
    {
        _transitionMaterial->Tint.a += Time::DeltaTime * 1.25;

        if (_transitionMaterial->Tint.a >= 1)
        {
            ChangeScreen(Screen::GAME);
            return;
        }
    }

    auto pos = glm::vec2(_input.MouseX / _renderer.ScreenSize, _input.MouseY / _renderer.ScreenSize);

    auto screenPos = glm::unProject(glm::vec3(pos.x, pos.y, 0), glm::mat4(1), _renderer.ProjectionMatrix, glm::vec4(0, 0, 480, 270));
    screenPos.x += 2;
    screenPos.y = -screenPos.y - 1.65;
    _handTransform->Position = screenPos;
}


entt::entity Demo::CreateUISprite(const Renderer& _renderer, Material* material, glm::vec3 position)
{
    auto entt = CreateEntity();
    auto& spriteRenderer = AddComponent<SpriteRendererComponent>(entt, material);
    auto scale = _renderer.GetRealScale(spriteRenderer.Size.x, spriteRenderer.Size.y);
    AddComponent<TransformComponent>(entt, position, scale, 0);
    AddComponent<ForwardRendererComponent>(entt);
    return entt;
}

//===========GAME=======================//

void Demo::ReturnToMainMenu()
{
    ChangeScreen(Screen::MAIN_MENU);
}

entt::entity Demo::CreateMonster(int hp, float x, float y, const char* animation)
{
    auto monster = _registry.create();
    _registry.emplace<SpriteRendererComponent>(monster, _monsterMaterial);
    auto& animator = _registry.emplace<AnimatedSpriteRendererComponent>(monster, "assets/monster/monster.json");
    auto scale = _renderer.GetRealScale(115, 105);
    _registry.emplace<TransformComponent>(monster, glm::vec3(x, y, 3), glm::vec2(scale), 0.0);
    animator.ChangeAnimation(animation);
    return monster;
}

void Demo::CreateLevel()
{
    Status = LevelCompletionStatus::NONE;
    _currentUIScreen = UIScreen::NONE;
    _camera = CreateCamera(glm::vec2(480, 270), 1);
    auto cameraComponent = _registry.get<CameraComponent>(_camera);
    _renderer.ProjectionMatrix = cameraComponent.GetProjectionMatrix();
    _renderer.AspectRatio = cameraComponent.AspectRatio;
    _renderer.OrthoScale = cameraComponent.OrthoScale;

    AddSystem<PlayerSystem>(_input, _audio, _camera, this);

    auto dust = CreateEntity();
    auto& dustRenderer = AddComponent<SpriteRendererComponent>(dust, _dustMaterial);
    auto dustScale = _renderer.GetRealScale(dustRenderer.Size.x, dustRenderer.Size.y);
    AddComponent<TransformComponent>(dust, glm::vec3(0, 0, 4), dustScale, 0);

    auto screenPos = _renderer.ConvertScreenToWorldCoords(0, 0);
    auto monsterPos = _renderer.ConvertScreenToWorldCoords(63, 840);
    CreateDecal(_renderer, _cityMaterial, glm::vec3(screenPos.x, screenPos.y, 0));
    CreateDecal(_renderer, _foreGroundMaterial, glm::vec3(screenPos.x, screenPos.y, 4.5));
    CreateDecal(_renderer, _pipesMaterial, glm::vec3(screenPos.x, screenPos.y, 3));
    CreateDecal(_renderer, _pipes2Material, glm::vec3(screenPos.x, screenPos.y, 3));
    CreateDecal(_renderer, _levelBackgroundMaterial, glm::vec3(screenPos.x, screenPos.y, 0));
    CreateDecal(_renderer, _middleGroundMaterial, glm::vec3(screenPos.x, screenPos.y, 4));

    auto monsterCapsule = CreateEntity();
    auto& animatedMonsterCapsuleRenderer = AddComponent<AnimatedSpriteRendererComponent>(monsterCapsule, "assets/monster/monster_capsule.json");
    animatedMonsterCapsuleRenderer.ChangeAnimation("Idle");
    auto& capsuleRenderer = AddComponent<SpriteRendererComponent>(monsterCapsule, _monsterCapsuleMaterial);
    auto scale = _renderer.GetRealScale(104, 135);
    glm::vec3 pos = glm::vec3(monsterPos.x, monsterPos.y, 4);
    pos.x += scale.x;
    pos.y -= scale.y;
    AddComponent<TransformComponent>(monsterCapsule, pos, scale, 0);
    AddComponent<RectCollider>(monsterCapsule);
    AddComponent<MonsterCapsuleComponent>(monsterCapsule);

    auto player = CreateEntity();
    auto& animatedSpriteRenderer = AddComponent<AnimatedSpriteRendererComponent>(player, "assets/omniroid/omniroid.json");
    animatedSpriteRenderer.ChangeAnimation("Idle");
    auto& playerSpriteRenderer = AddComponent<SpriteRendererComponent>(player, _playerMaterial);
    auto playerScale = _renderer.GetRealScale(88, 88);
    AddComponent<TransformComponent>(player, glm::vec3(0, 5, 3), playerScale, 0);
    //AddComponent<TransformComponent>(player, glm::vec3(0, 5, 3), playerScale, 0);
    AddComponent<PlayerComponent>(player);

    //load collision rects
    auto entities = _levelData["entities"];
    auto collisionRects = entities["CollisionRect"];
    for (auto& item : collisionRects.items())
    {
        auto& colliderInfo = item.value();
        int x = stoi(colliderInfo["x"].dump());
        int y = stoi(colliderInfo["y"].dump());
        int width = stoi(colliderInfo["width"].dump());
        int height = stoi(colliderInfo["height"].dump());

        auto entity = CreateEntity();

        glm::vec2 position = _renderer.ConvertScreenToWorldCoords(x, y);
        glm::vec2 size = _renderer.GetRealScale(width, height);

        position.x += size.x;
        position.y -= size.y;
        AddComponent<TransformComponent>(entity, glm::vec3(position.x, position.y, 5), size, 0);
        AddComponent<RectCollider>(entity);
    }

    //load all crates
    auto crates = entities["Crate"];
    for (auto& item : crates.items())
    {
        auto& crateInfo = item.value();
        int x = stoi(crateInfo["x"].dump());
        int y = stoi(crateInfo["y"].dump());
        int width = 33;
        int height = 33;

        auto entity = CreateEntity();

        glm::vec2 position = _renderer.ConvertScreenToWorldCoords(x, y);
        glm::vec2 size = _renderer.GetRealScale(width, height);

        position.x += size.x;
        position.y -= size.y;
        AddComponent<TransformComponent>(entity, glm::vec3(position.x, position.y, 3), size, 0);
        AddComponent<RectCollider>(entity);
        AddComponent<SpriteRendererComponent>(entity, _crateMaterial);
        AddComponent<CrateComponent>(entity);
    }

    CreateLight(glm::vec3(monsterPos.x + 105, monsterPos.y + 30, 2), glm::vec3(1.0, 0, 0), 2.0, true, 5.0);
    for (int i = 0; i < 5; i++)
    {
        CreateLight(glm::vec3(480 * i, 135, 2), glm::vec3(0.19, 0.702, 0.502), 2.0, true, 1.0);
        CreateLight(glm::vec3(480 * i, -380, 2), glm::vec3(0.19, 0.702, 0.502), 2.0, true, 1.0);
        CreateLight(glm::vec3(480 * i, -360 - 460, 2), glm::vec3(0.19, 0.702, 0.502), 2.0, true, 1.0);
        CreateLight(glm::vec3(480 * i, -360 - 460 - 540, 2), glm::vec3(0.19, 0.702, 0.502), 2.0, true, 1.0);
    }

}

void Demo::CreateCollisionRect(Renderer& _renderer, int x, int y, int pixelWidth, int pixelHeight)
{
    auto rect = CreateEntity();

    auto& transform = AddComponent<TransformComponent>(rect,
        glm::vec3(0), _renderer.GetRealScale(pixelWidth, pixelHeight), 0);
    AddComponent<RectCollider>(rect);
}

void Demo::CreateLight(glm::vec3 position, glm::vec3 color, float intensity, bool castRays, float sineInfluence)
{
    auto light = CreateEntity();
    AddComponent<TransformComponent>(light, position, glm::vec2(1.0), 0);
    AddComponent<LightComponent>(light, color, intensity, castRays, sineInfluence);
}

Material* Demo::CreateDecalMaterial(const std::string& name, const glm::vec4& tint, float lightAbsorption)
{
    Material* material = nullptr;
        material = new DefaultLitMaterial(
            std::string(COMMON_LEVEL_ASSET_ALBEDO_PATH + name).c_str(),
            std::string(COMMON_LEVEL_ASSET_NORMAL_PATH + name).c_str(),
            std::string(COMMON_LEVEL_ASSET_PARAMS_PATH + name).c_str(),
            tint, 1.0);
        ((DefaultLitMaterial*)material)->LightAbsorption = lightAbsorption;
    return material;
}

void Demo::CreateDecal(Renderer& _renderer, Material* material, const glm::vec3& position)
{
    auto decal = CreateEntity();
    auto& spriteRenderer = AddComponent<SpriteRendererComponent>(decal, material);
    auto scale = _renderer.GetRealScale(spriteRenderer.Size.x, spriteRenderer.Size.y);
    glm::vec3 pos = position;
    pos.x += scale.x;
    pos.y -= scale.y;
    AddComponent<TransformComponent>(decal, pos, scale, 0);
}