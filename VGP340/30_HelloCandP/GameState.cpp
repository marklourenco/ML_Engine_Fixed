#include "GameState.h"
#include "BallPhysicsSystem.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;
using namespace ML_Engine::Physics;

void GameState::Initialize()
{
    // dir light
	mDirectionalLight.direction = Math::Normalize(Math::Vector3(0.0f, -1.0f, 1.0f));
	mDirectionalLight.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mDirectionalLight.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	mGameWorld.Initialize();
    GameObject* cameraGO = mGameWorld.CreateGameObject("Camera");
    mCameraComponent = cameraGO->AddComponent<CameraComponent>();
    cameraGO->AddComponent<FPSCameraComponent>();
    cameraGO->Initialize();

    BallPhysicsSystem::Settings mSettings;
    mSettings.floorY = 0.0f;
    mSettings.bounciness = 0.7f;
    mPhysics.Initialize(mSettings, mCameraComponent->GetCamera(), mDirectionalLight);

    for (int i = 0; i < 5; ++i)
    {
        Ball b;
        b.mPosition = { static_cast<float>(i * 1.1 - 2.5f), static_cast<float>(i + 5.0f), 0.0f };
        mPhysics.AddBall(b);
    }

	mCameraComponent->GetCamera().SetPosition({ 1.5f, 1.0f, -20.0f });
}
void GameState::Terminate()
{
    mPhysics.Terminate();
	mGameWorld.Terminate();
}
void GameState::Update(float deltaTime)
{
    mPhysics.Update(deltaTime);
	mGameWorld.Update(deltaTime);
}
void GameState::Render()
{
    mPhysics.SetCamera(mCameraComponent->GetCamera());
    mPhysics.Render();
    mGameWorld.Render();
    SimpleDraw::Render(mCameraComponent->GetCamera());
}
void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mPhysics.DebugUI();
	mGameWorld.DebugUI();
	ImGui::End();
}