#include "GameState.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;
using namespace ML_Engine::Physics;

Service* MakeCustomService(const std::string& serviceName, GameWorld& gameWorld)
{
    return nullptr;
}
Component* MakeCustomComponent(const std::string& componentName, GameObject& gameObject)
{
	return nullptr;
}
Component* GetCustomComponent(const std::string& componentName, GameObject& gameObject)
{
	return nullptr;
}
void GameState::Initialize()
{
    mLevelFile = L"../../Assets/Templates/Levels/level_mp.json";

	GameWorld::SetCustomService(MakeCustomService);
	GameObjectFactory::SetCustomMake(MakeCustomComponent);
	GameObjectFactory::SetCustomGet(GetCustomComponent);

	mGameWorld.LoadLevel(mLevelFile);
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