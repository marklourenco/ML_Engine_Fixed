#include "GameState.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;
using namespace ML_Engine::Physics;

void GameState::Initialize()
{
    mLevelFile = L"../../Assets/Templates/Levels/walljump_level.json";

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
    if (ImGui::Button("ReloadLevel"))
    {
        mGameWorld.Terminate();
        mGameWorld.LoadLevel(mLevelFile);
    }
	ImGui::End();
}