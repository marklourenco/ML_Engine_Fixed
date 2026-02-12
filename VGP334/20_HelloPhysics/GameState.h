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

	ML_Engine::Graphics::RenderObject mBallObject;
	ML_Engine::Physics::CollisionShape mBallShape;
	ML_Engine::Physics::RigidBody mBallRigidBody;

	ML_Engine::Graphics::RenderObject mGroundObject;
	ML_Engine::Physics::CollisionShape mGroundShape;
	ML_Engine::Physics::RigidBody mGroundRigidBody;

	struct BoxData
	{
		ML_Engine::Graphics::RenderObject box;
		ML_Engine::Physics::CollisionShape shape;
		ML_Engine::Physics::RigidBody rigidBody;
	};
	using Boxes = std::vector<BoxData>;
	Boxes mBoxes;

	ML_Engine::Graphics::StandardEffect mStandardEffect;

	// cloth info
	ML_Engine::Graphics::RenderObject mCloth;
	ML_Engine::Graphics::Mesh mClothMesh;
	ML_Engine::Physics::SoftBody mClothSoftBody;
};