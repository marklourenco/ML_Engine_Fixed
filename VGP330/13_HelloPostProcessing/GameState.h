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

	float mTime = 0.0f;

	void UpdateCamera(float deltaTime);

	ML_Engine::Graphics::Camera mCamera;
	ML_Engine::Graphics::DirectionalLight mDirectionalLight;

	ML_Engine::Graphics::RenderGroup mCharacter;
	ML_Engine::Graphics::RenderObject mGround;
	ML_Engine::Graphics::RenderTarget mRenderTarget;
	ML_Engine::Graphics::RenderObject mScreenQuad;
	ML_Engine::Graphics::Texture mCombineTexture;

	ML_Engine::Graphics::StandardEffect mStandardEffect;
	ML_Engine::Graphics::PostProcessingEffect mPostProcessingEffect;
};