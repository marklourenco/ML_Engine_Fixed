#include "GameState.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;
using namespace ML_Engine::Physics;

void GameState::Initialize()
{
   mGameWorld.Initialize();

   GameObject* transformGO = mGameWorld.CreateGameObject("Transform", L"../../Assets/Templates/Objects/transform_obj.json");
   transformGO->Initialize();

   GameObject* cameraGO = mGameWorld.CreateGameObject("Camera", L"../../Assets/Templates/Objects/fps_camera.json");
   mCameraComponent = cameraGO->GetComponent<CameraComponent>();
   cameraGO->Initialize();

   // Visual Game Objects

   GameObject* playerGO = mGameWorld.CreateGameObject("Player");
   TransformComponent* playerTransform = playerGO->GetComponent<TransformComponent>();
   playerTransform->position.x = 2.0f;
   playerGO->Initialize();
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