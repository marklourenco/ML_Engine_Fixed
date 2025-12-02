#include "GameState.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    mCharacter.Initialize("Character01/Character01.model");
    mCharacter02.Initialize("Character02/Character02.model");
    mCharacter03.Initialize("Character03/Character03.model");

    mTerrain.Initialize(L"../../Assets/Textures/terrain/heightmap_512x512.raw",20.0f);
    mGround.meshBuffer.Initialize(mTerrain.mesh);
    mGround.diffuseMapId = TextureManager::Get()->LoadTexture("terrain/dirt_seamless.jpg");
    mGround.specMapId = TextureManager::Get()->LoadTexture("terrain/grass_2048.jpg");

    MeshPX screenQuad = MeshBuilder::CreateScreenQuadPX();
    mScreenQuad.meshBuffer.Initialize(screenQuad);

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);

    mTerrainEffect.Initialize();
	mTerrainEffect.SetCamera(mCamera);
	mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);
	mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    // infrared
    shaderFile = L"../../Assets/Shaders/Infrared.fx";
    mInfrared.Initialize(shaderFile);
    mInfrared.SetTexture(&mRenderTarget);
    mInfrared.SetTexture(&mCombineTexture, 1);

    GraphicsSystem* gs = GraphicsSystem::Get();
    const uint32_t screenWidth = gs->GetBackBufferWidth();
    const uint32_t screenHeight = gs->GetBackBufferHeight();
    mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

    mCombineTexture.Initialize(L"../../Assets/Textures/PredatorHUD.png");

    // move characters
    mCharacter.transform.position = { 100.0f, 5.0f, 150.0f };
    mCharacter02.transform.position = { 95.0f, 3.0f, 155.0f };
    mCharacter03.transform.position = { 120.0f, 5.0f, 200.0f };

    // move camera
    mCamera.SetPosition({ 100.0f, 10.0f, 140.0f});
}
void GameState::Terminate()
{
    mRenderTarget.Terminate();
    mInfrared.Terminate();
    mCombineTexture.Terminate();
    mTerrainEffect.Terminate();
	mShadowEffect.Terminate();
    mStandardEffect.Terminate();
    mCharacter03.Terminate();
    mCharacter02.Terminate();
    mCharacter.Terminate();
    mGround.Terminate();
}
void GameState::Update(float deltaTime)
{
    mTime += deltaTime;

    UpdateCamera(deltaTime);
}
void GameState::Render()
{
    mShadowEffect.Begin();
		mShadowEffect.Render(mCharacter);
		mShadowEffect.Render(mCharacter02);
		mShadowEffect.Render(mCharacter03);
	mShadowEffect.End();

    // render target start
    mRenderTarget.BeginRender();
        mTerrainEffect.Begin();
	    	mTerrainEffect.Render(mGround);
	    mTerrainEffect.End();
        
        mStandardEffect.Begin();
            mStandardEffect.Render(mCharacter);
            mStandardEffect.Render(mCharacter02);
            mStandardEffect.Render(mCharacter03);
        mStandardEffect.End();
    mRenderTarget.EndRender();
    // render target end

    mInfrared.Begin(mTime);
        mInfrared.Render(mScreenQuad);
    mInfrared.End();
}
void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    // infrared
    mInfrared.DebugUI();

    // lighting
    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction#Light", &mDirectionalLight.direction.x, 0.01f))
        {
            mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
        }
        ImGui::ColorEdit4("Ambient#Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse#Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular#Light", &mDirectionalLight.specular.r);
    }

    // textures
    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
    {
        Material& material01 = mCharacter.renderObjects[0].material;
        ImGui::ColorEdit4("Emissive01#Material", &material01.emissive.r);
        ImGui::ColorEdit4("Ambient01#Material", &material01.ambient.r);
        ImGui::ColorEdit4("Diffuse01#Material", &material01.diffuse.r);
        ImGui::ColorEdit4("Specular01#Material", &material01.specular.r);
        ImGui::DragFloat("Shininess01#Material", &material01.shininess, 0.1f, 0.1f, 1000.f);

        Material& material02 = mCharacter02.renderObjects[0].material;
        ImGui::ColorEdit4("Emissive02#Material", &material02.emissive.r);
        ImGui::ColorEdit4("Ambient02#Material", &material02.ambient.r);
        ImGui::ColorEdit4("Diffuse02#Material", &material02.diffuse.r);
        ImGui::ColorEdit4("Specular02#Material", &material02.specular.r);
        ImGui::DragFloat("Shininess02#Material", &material02.shininess, 0.1f, 0.1f, 1000.f);

        Material& material03 = mCharacter03.renderObjects[0].material;
        ImGui::ColorEdit4("Emissive03#Material", &material03.emissive.r);
        ImGui::ColorEdit4("Ambient03#Material", &material03.ambient.r);
        ImGui::ColorEdit4("Diffuse03#Material", &material03.diffuse.r);
        ImGui::ColorEdit4("Specular03#Material", &material03.specular.r);
        ImGui::DragFloat("Shininess03#Material", &material03.shininess, 0.1f, 0.1f, 1000.f);

        // ground material
        Material& material04 = mGround.material;
        ImGui::ColorEdit4("Emissive04#Material", &material04.emissive.r);
        ImGui::ColorEdit4("Ambient04#Material", &material04.ambient.r);
        ImGui::ColorEdit4("Diffuse04#Material", &material04.diffuse.r);
        ImGui::ColorEdit4("Specular04#Material", &material04.specular.r);
        ImGui::DragFloat("Shininess04#Material", &material04.shininess, 0.1f, 0.1f, 1000.f);
    }

    // characters pos
    ImGui::DragFloat3("CharacterPosition", &mCharacter.transform.position.x, 0.01f);
    ImGui::DragFloat3("CharacterPosition02", &mCharacter02.transform.position.x, 0.01f);
    ImGui::DragFloat3("CharacterPosition03", &mCharacter03.transform.position.x, 0.01f);

    // general textures
    mStandardEffect.DebugUI();
    // mShadowEffect.DebugUI();
    // mTerrainEffect.DebugUI();
    ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
    Input::InputSystem* input = Input::InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.1f;

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltaTime);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime); // INVERT IF U WANT
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }

    Math::Vector3 cameraPosition = mCamera.GetPosition();
    float height = mTerrain.GetHeight(cameraPosition);
    if (height >= 0.0f)
    {
        cameraPosition.y = height + 1.5f;
        mCamera.SetPosition(cameraPosition);
    }
}