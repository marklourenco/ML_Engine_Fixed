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

	// ground
	ML_Engine::Graphics::RenderObject mGroundObject;

	// walls
	struct Wall
	{
		ML_Engine::Graphics::RenderObject wall;
	};
	std::vector<Wall> mWalls;

	// paintings
	ML_Engine::Graphics::RenderObject mLastSupperOld;
	ML_Engine::Graphics::RenderObject mLastSupperNew;

	// characters
	ML_Engine::Graphics::RenderGroup mPhilomena;
	ML_Engine::Graphics::Animator mAnimatorPhilomena;

	int mClipIndexPhilomena = -1;
	float mAnimationSpeedPhilomena = 1.0f;
};