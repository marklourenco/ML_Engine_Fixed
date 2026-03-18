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

	ML_Engine::Graphics::Camera mCamera;
	ML_Engine::Graphics::DirectionalLight mDirectionalLight;
	ML_Engine::Graphics::StandardEffect mStandardEffect;

	// military base
	ML_Engine::Graphics::RenderGroup mMilitaryBase;

	// sky sphere
	ML_Engine::Graphics::RenderObject mSkySphere;

	// terrain
	ML_Engine::Graphics::RenderObject mGround;
	ML_Engine::Graphics::Terrain mTerrain;
	ML_Engine::Graphics::TerrainEffect mTerrainEffect;

	// characters
	ML_Engine::Graphics::RenderGroup mPredator;
	ML_Engine::Graphics::Animation mPredatorTransformAnimation;
	ML_Engine::Graphics::Animator mAnimatorPredator;

	int mClipIndexPredator = -1;
	float mAnimationSpeedPredator = 1.0f;

	ML_Engine::Graphics::RenderGroup mSoldier1;
	ML_Engine::Graphics::Animation mSoldier1TransformAnimation;
	ML_Engine::Graphics::Animator mAnimatorSoldier1;

	int mClipIndexSoldier1 = -1;
	float mAnimationSpeedSoldier1 = 1.0f;

	ML_Engine::Graphics::RenderGroup mSoldier2;
	ML_Engine::Graphics::Animation mSoldier2TransformAnimation;
	ML_Engine::Graphics::Animator mAnimatorSoldier2;

	int mClipIndexSoldier2 = -1;
	float mAnimationSpeedSoldier2 = 1.0f;

	ML_Engine::Graphics::RenderGroup mSoldier3;
	ML_Engine::Graphics::Animation mSoldier3TransformAnimation;
	ML_Engine::Graphics::Animator mAnimatorSoldier3;

	int mClipIndexSoldier3 = -1;
	float mAnimationSpeedSoldier3 = 1.0f;

	// military trucks
	ML_Engine::Graphics::RenderGroup mMilitaryTruck1;
	ML_Engine::Graphics::Animation mMilitaryTruck1TransformAnimation;
	ML_Engine::Graphics::Animator mAnimatorTruck1;

	ML_Engine::Graphics::RenderGroup mMilitaryTruck2;
	ML_Engine::Graphics::Animation mMilitaryTruck2TransformAnimation;
	ML_Engine::Graphics::Animator mAnimatorTruck2;

	ML_Engine::Graphics::RenderGroup mMilitaryTruck3;
	ML_Engine::Graphics::Animation mMilitaryTruck3TransformAnimation;
	ML_Engine::Graphics::Animator mAnimatorTruck3;


	// camera anims
	ML_Engine::Graphics::Animation mCameraTransformAnimation;

	// timer
	float mTimer = 0.0f;

	// events
	void OnChangePredatorAnimEvent(const ML_Engine::Core::Event& e);
	ML_Engine::Core::EventListenerId mChangePredatorAnimListenerId = 0;

	void OnChangeSoldierAnimEvent(const ML_Engine::Core::Event& e);
	ML_Engine::Core::EventListenerId mChangeSoldierAnimListenerId = 0;

	void OnInfraredEvent(const ML_Engine::Core::Event& e);
	ML_Engine::Core::EventListenerId mInfraredListenerId = 0;
	
	void OnSpawnParticleEvent(const ML_Engine::Core::Event& e);
	ML_Engine::Core::EventListenerId mSpawnParticleListenerId = 0;

	void OnPlaySoundEvent(const ML_Engine::Core::Event& e);
	ML_Engine::Core::EventListenerId mPlaySoundListenerId = 0;

	void OnStopSoundEvent(const ML_Engine::Core::Event& e);
	ML_Engine::Core::EventListenerId mStopSoundListenerId = 0;

	// audio
	ML_Engine::Audio::SoundId mAmbienceSoundId = 0;
	ML_Engine::Audio::SoundId mBigExplosionSoundId = 0;
	ML_Engine::Audio::SoundId mBloodySlashSoundId = 0;
	ML_Engine::Audio::SoundId mDeathSoundId = 0;
	ML_Engine::Audio::SoundId mFallSoundId = 0;
	ML_Engine::Audio::SoundId mFallDeathSoundId = 0;
	ML_Engine::Audio::SoundId mMissileAtSoldierSoundId = 0;
	ML_Engine::Audio::SoundId mMissileAtTrucksSoundId = 0;
	ML_Engine::Audio::SoundId mMissileLockOnSoundId = 0;
	ML_Engine::Audio::SoundId mSmallExplosionSoundId = 0;
	ML_Engine::Audio::SoundId mThermalSoundId = 0;
	ML_Engine::Audio::SoundId mTruckSoundId = 0;

	// shadow effect
	ML_Engine::Graphics::ShadowEffect mShadowEffect;

	// infrared
	ML_Engine::Graphics::RenderTarget mRenderTarget;
	ML_Engine::Graphics::RenderObject mScreenQuad;
	ML_Engine::Graphics::Texture mCombineTexture;
	ML_Engine::Graphics::Infrared mInfrared;

	// for debug
	bool mCameraDebug = false;

	// particles
	ML_Engine::Graphics::ParticleSystemEffect mParticleSystemEffect;
	ML_Engine::Physics::ParticleSystem mParticleSystem;
	ML_Engine::Physics::ParticleSystem mExplosionSystem;
};