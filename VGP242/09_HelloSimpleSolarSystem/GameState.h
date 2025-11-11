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
	void RenderObject(const ML_Engine::Graphics::SimpleTextureEffect::RenderData& object, const ML_Engine::Graphics::Camera& camera);

	ML_Engine::Graphics::Camera mCamera;
	ML_Engine::Graphics::Camera mRenderTargetCamera;

	// GPU communication
	ML_Engine::Graphics::SimpleTextureEffect mSimpleTextureEffect;

	// render planets / space
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mSpace;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mSun;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mMercury;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mVenus;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mEarth;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mMars;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mJupiter;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mSaturn;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mUranus;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mNeptune;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mPluto;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mMoon;

	// render target
	ML_Engine::Graphics::RenderTarget mRenderTarget;
};