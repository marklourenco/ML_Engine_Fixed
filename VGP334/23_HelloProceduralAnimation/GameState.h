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

	ML_Engine::Graphics::RenderObject mGroundObject;
	ML_Engine::Physics::CollisionShape mGroundShape;
	ML_Engine::Physics::RigidBody mGroundRigidBody;

	struct Segment
	{
		ML_Engine::Graphics::RenderObject segment;
		float range = 0.0f;
	};
	std::vector<Segment> mSegments;
	ML_Engine::Physics::CollisionShape mBodyAnchorShape;
	ML_Engine::Physics::RigidBody mBodyAnchor;
};