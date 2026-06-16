#pragma once

#include <Inc/ML_Engine.h>

#include "BallPhysicsSystem.h"

class GameState : public ML_Engine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	BallPhysicsSystem mPhysics;

	ML_Engine::GameWorld mGameWorld;
	ML_Engine::CameraComponent* mCameraComponent = nullptr;
	ML_Engine::Graphics::DirectionalLight mDirectionalLight;
};