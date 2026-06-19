#include "GameState.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;
using namespace ML_Engine::Physics;

void GameState::Initialize()
{
    mLevelFile = L"../../Assets/Templates/Levels/walljump_level.json";

	mGameWorld.LoadLevel(mLevelFile);

    mSimpleTextureEffect.Initialize();
    ML_Engine::Graphics::MeshPX skySphere = ML_Engine::Graphics::MeshBuilder::CreateSkySpherePX(30, 30, 500.0f);
    mSkySphere.mesh.Initialize(skySphere);
    mSkySphere.textureId = ML_Engine::Graphics::TextureManager::Get()->LoadTexture("space.jpg");
}
void GameState::Terminate()
{
    mSkySphere.mesh.Terminate();
    ML_Engine::Graphics::TextureManager::Get()->ReleaseTexture(mSkySphere.textureId);
	mSimpleTextureEffect.Terminate();

    mGameWorld.Terminate();
}
void GameState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}
void GameState::Render()
{
    auto* cameraService = mGameWorld.GetService<CameraService>();
    mSimpleTextureEffect.SetCamera(cameraService->GetMain());
    mSimpleTextureEffect.Begin();
    mSimpleTextureEffect.Render(mSkySphere);
	mSimpleTextureEffect.End();

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