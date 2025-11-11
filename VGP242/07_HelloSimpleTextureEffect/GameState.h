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
	ML_Engine::Graphics::Camera mRenderTargetCamera;

	// GPU communication
	ML_Engine::Graphics::SimpleTextureEffect mSimpleTextureEffect;

	// render object
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mObject0;
	ML_Engine::Graphics::SimpleTextureEffect::RenderData mObject1;

	// render target
	ML_Engine::Graphics::RenderTarget mRenderTarget;
};