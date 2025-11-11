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

	ML_Engine::Graphics::RenderGroup mCharacter;
	ML_Engine::Graphics::RenderGroup mCharacter02;
	ML_Engine::Graphics::RenderGroup mCharacter03;
	ML_Engine::Graphics::StandardEffect mStandardEffect;
};