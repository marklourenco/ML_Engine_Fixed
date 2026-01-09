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

	ML_Engine::Graphics::RenderObject mRenderObjectPyramid;
	ML_Engine::Graphics::RenderObject mRenderObjectPlatform;
	ML_Engine::Graphics::RenderObject mRenderObjectBall1;
	ML_Engine::Graphics::RenderObject mRenderObjectBall2;
	ML_Engine::Graphics::RenderObject mRenderObjectFloor;
	ML_Engine::Graphics::StandardEffect mStandardEffect;

	ML_Engine::Graphics::Animation mAnimation;
	ML_Engine::Graphics::Animation mAnimation2;
	ML_Engine::Graphics::Animation mAnimation3;
	float mAnimationTime = 0.0f;
};