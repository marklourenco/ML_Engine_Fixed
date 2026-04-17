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

   // GameObject* cameraGO = mGameWorld.CreateGameObject("Camera");
   // cameraGO->AddComponent<CameraComponent>();
   // cameraGO->Initialize();

   // GameObject* meshGO = mGameWorld.CreateGameObject("Mesh");
   // meshGO->AddComponent<MeshComponent>();
   // meshGO->Initialize();
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
}