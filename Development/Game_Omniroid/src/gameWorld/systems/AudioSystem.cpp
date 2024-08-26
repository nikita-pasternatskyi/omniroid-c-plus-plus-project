#include "AudioSystem.h"
#include "TransformComponent.h"
#include "AudioListenerComponent.h"

AudioSystem::AudioSystem(Audio& audio)
	: _audio(std::make_unique<Audio>(audio))
{
}

void AudioSystem::Update(entt::registry& registry)
{
	auto view = registry.view<AudioListenerComponent, TransformComponent>();

	for (auto entity : view)
	{
		auto transform = view.get<TransformComponent>(entity);
		FMOD_3D_ATTRIBUTES attributes = { { 0 } };
		attributes.forward.z = -1.0f;
		attributes.up.y = 1.0f;
		attributes.position = FMOD_VECTOR{-transform.Position.x, transform.Position.y, 0};
		_audio->SetListenerAttributes(0, &attributes);
	}
}
