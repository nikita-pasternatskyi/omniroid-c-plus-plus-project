#pragma once
#include <unordered_map>
#include <string>
#include "Animation.h"
#include "../src/json/JSONReader.h"

struct AnimatedSpriteRendererComponent
{
    AnimatedSpriteRendererComponent(const char* jsonFile)
        : CurrentTimer(0.0f), CurrentAnimationFrame(0), _currentAnimation()
	{
        auto json = JsonReader::ReadJson(jsonFile);
        auto frames = json["frames"];
        for (auto& an : json["meta"]["frameTags"].items())
        {
            auto& animationInfo = an.value();
            auto name = animationInfo["name"].dump();
            name.erase(name.size()-1);
            name.erase(0, 1);
            int framesBegin = stoi(animationInfo["from"].dump());
            int framesEnd = stoi(animationInfo["to"].dump());
            int loopedIndex = name.find("_looped");
            int loopedFrameIndex = name.find("d_");
            bool repeat = loopedIndex != name.npos;
            int loopFrame = 0;
            if (repeat)
            {
                if (loopedFrameIndex != name.npos)
                {
                    loopFrame = stoi(name.substr(loopedFrameIndex + 2, name.size())) - framesBegin;
                }
                name = name.substr(0, loopedIndex);
            }

            Animations[name].Looped = repeat;
            Animations[name].LoopFrame = loopFrame;

            for (int i = framesBegin; i <= framesEnd; i++)
            {
                auto& frame = frames[i];
                auto& pos = frame["frame"];
                auto& off = frame["spriteSourceSize"];
                
                int posX = stoi(pos["x"].dump());
                int posY = stoi(pos["y"].dump());
                int sizeX = stoi(pos["w"].dump());
                int sizeY = stoi(pos["h"].dump());

                int offX = stoi(off["x"].dump());
                int offY = stoi(off["y"].dump());
                    
                int duration = stoi(frame["duration"].dump());
                glm::vec2 position = {posX, posY};
                glm::vec2 offset = {offX, offY};
                glm::vec2 size = {sizeX, sizeY};
                Animations[name].Frames.push_back({ position , offset, size, duration });
            }
        }
	}

    void ChangeAnimation(const std::string& name)
    {
        if (name == _currentAnimation)
            return;
        if (Animations.find(name) == Animations.end())
        {
            LOG_WARNING("[ANIMATED SPRITE RENDERER]::Animation{" << name << "}was not found!");
            return;
        }

        CurrentTimer = 0;
        CurrentAnimationFrame = 0;
        _currentAnimation = name;
    }
	std::unordered_map<std::string, Animation> Animations;
	float CurrentTimer;
	int CurrentAnimationFrame;
    std::string GetCurrentAnimation() { return _currentAnimation; }
private:
    std::string _currentAnimation;
};