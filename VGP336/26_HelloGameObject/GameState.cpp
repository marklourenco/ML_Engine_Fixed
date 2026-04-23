#include "GameState.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;
using namespace ML_Engine::Physics;

void GameState::Initialize()
{
   mGameWorld.Initialize();

   GameObject* transformGO = mGameWorld.CreateGameObject("Transform");
   transformGO->AddComponent<TransformComponent>();
   transformGO->Initialize();

   GameObject* cameraGO = mGameWorld.CreateGameObject("Camera");
   mCameraComponent = cameraGO->AddComponent<CameraComponent>();
   cameraGO->AddComponent<FPSCameraComponent>();
   cameraGO->Initialize();

   // Visual Game Objects

   GameObject* playerGO = mGameWorld.CreateGameObject("Player");
   TransformComponent* playerTransform = playerGO->AddComponent<TransformComponent>();
   playerTransform->position.x = 2.0f;
   playerGO->Initialize();

   GameObject* bucketGO = mGameWorld.CreateGameObject("Bucket");
   TransformComponent* bucketTransform = bucketGO->AddComponent<TransformComponent>();
   bucketTransform->position.x = 4.0f;
   bucketGO->Initialize();

   GameObject* truckGO = mGameWorld.CreateGameObject("Truck");
   TransformComponent* truckTransform = truckGO->AddComponent<TransformComponent>();
   truckTransform->position.x = 6.0f;
   truckGO->Initialize();

   GameObject* guitarGO = mGameWorld.CreateGameObject("Guitar");
   TransformComponent* guitarTransform = guitarGO->AddComponent<TransformComponent>();
   guitarTransform->position.x = 8.0f;
   guitarGO->Initialize();

   GameObject* trainGO = mGameWorld.CreateGameObject("Train");
   TransformComponent* trainTransform = trainGO->AddComponent<TransformComponent>();
   trainTransform->position.x = 10.0f;
   trainGO->Initialize();
}
void GameState::Terminate()
{
    mGameWorld.Terminate();
}
void GameState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}
void GameState::Render()
{
    mGameWorld.Render();
}
void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mGameWorld.DebugUI();
	ImGui::End();

	SimpleDraw::Render(mCameraComponent->GetCamera());
}