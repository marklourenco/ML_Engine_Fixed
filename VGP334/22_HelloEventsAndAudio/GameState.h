#pragma once

#include <Inc/ML_Engine.h>

class GameState : public ML_Engine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

	void UpdateCamera(float deltaTime);

	void OnSpacePressedEvent(const ML_Engine::Core::Event& e);

	ML_Engine::Graphics::Camera mCamera;
	ML_Engine::Graphics::ParticleSystemEffect mParticleSystemEffect;
	ML_Engine::Physics::ParticleSystem mParticleSystem;
	ML_Engine::Physics::ParticleSystem mFireworkParticles;

	ML_Engine::Core::EventListenerId mSpacePressedListenerId = 0;
	ML_Engine::Core::EventListenerId mEnterPressedListenerId = 0;

	ML_Engine::Graphics::Animation mFireworkAnimation;
	float mFireworkAnimationTime = 0.0f;

	ML_Engine::Audio::SoundId mLaunchSoundId = 0;
	ML_Engine::Audio::SoundId mExplosionSoundId = 0;
};