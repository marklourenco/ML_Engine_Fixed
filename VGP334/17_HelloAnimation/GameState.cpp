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

	MeshPC mesh = MeshBuilder::CreatePyramidPC(1.0f);
    mRenderObjectPyramid.meshBuffer.Initialize(mesh);
    TextureManager* tm = TextureManager::Get();
    mRenderObjectPyramid.diffuseMapId = tm->LoadTexture("bricks.jpg");

	MeshPC meshPlatform = MeshBuilder::CreateRectanglePC(3.0f, 0.1f, 0.5f);
	mRenderObjectPlatform.meshBuffer.Initialize(meshPlatform);
	mRenderObjectPlatform.diffuseMapId = tm->LoadTexture("bricks.jpg");
	mRenderObjectPlatform.transform.position = { 0.0f, 0.5f, 0.0f };

	MeshPC meshBall1 = MeshBuilder::CreateSpherePC(10.0f, 10.0f, 0.3f);
	mRenderObjectBall1.meshBuffer.Initialize(meshBall1);
	mRenderObjectBall1.diffuseMapId = tm->LoadTexture("earth.jpg");

    MeshPC meshBall2 = MeshBuilder::CreateSpherePC(10.0f, 10.0f, 0.3f);
    mRenderObjectBall2.meshBuffer.Initialize(meshBall2);
    mRenderObjectBall2.diffuseMapId = tm->LoadTexture("earth.jpg");

    Mesh meshFloor = MeshBuilder::CreatePlane(3, 3, 3);
    mRenderObjectFloor.meshBuffer.Initialize(meshFloor);
    mRenderObjectFloor.diffuseMapId = tm->LoadTexture("bricks.jpg");
    mRenderObjectFloor.specMapId = tm->LoadTexture("bricks_spec.jpg");
    mRenderObjectFloor.normalMapId = tm->LoadTexture("bricks_normal.jpg");
    mRenderObjectFloor.bumpMapId = tm->LoadTexture("bricks_bump.jpg");

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    mAnimationTime = 0.0f;
	// platform animation
    mAnimation = AnimationBuilder()
        .AddPositionKey({ 0.0f, 0.5f, 0.0f }, 0.0f)
        .AddPositionKey({ 0.0f, 0.5f, 0.0f }, 15.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, 15.0f * Math::Constants::DegToRad), 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, -15.0f * Math::Constants::DegToRad), 3.75f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, 15.0f * Math::Constants::DegToRad), 7.5f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, -15.0f * Math::Constants::DegToRad), 11.25f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, 15.0f * Math::Constants::DegToRad), 15.0f)
        .Build();

	// ball1 animation
    mAnimation2 = AnimationBuilder()
		.AddPositionKey({ -1.0f, 0.5f, 0.0f }, 0.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 0.0f)
		.AddPositionKey({ -1.0f, 0.9f, 0.0f }, 3.75f)
        .AddScaleKey({ 0.5f, 0.5f, 0.5f }, 3.75f)
		.AddPositionKey({ -1.0f, 0.5f, 0.0f }, 7.5f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 7.5f)
		.AddPositionKey({ -1.0f, 0.9f, 0.0f }, 11.25f)
        .AddScaleKey({ 0.5f, 0.5f, 0.5f }, 11.25f)
		.AddPositionKey({ -1.0f, 0.5f, 0.0f }, 15.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 15.0f)
        .Build();

	// ball2 animation
    mAnimation3 = AnimationBuilder()
        .AddPositionKey({ 1.0f, 0.9f, 0.0f }, 0.0f)
        .AddScaleKey({ 0.5f, 0.5f, 0.5f }, 0.0f)
        .AddPositionKey({ 1.0f, 0.5f, 0.0f }, 3.75f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 3.75f)
		.AddPositionKey({ 1.0f, 0.9f, 0.0f }, 7.5f)
        .AddScaleKey({ 0.5f, 0.5f, 0.5f }, 7.5f)
		.AddPositionKey({ 1.0f, 0.5f, 0.0f }, 11.25f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 11.25f)
		.AddPositionKey({ 1.0f, 0.9f, 0.0f }, 15.0f)
        .AddScaleKey({ 0.5f, 0.5f, 0.5f }, 15.0f)
		.Build();
}
void GameState::Terminate()
{
    mRenderObjectPyramid.Terminate();
	mRenderObjectPlatform.Terminate();
	mRenderObjectBall1.Terminate();
	mRenderObjectBall2.Terminate();
    mRenderObjectFloor.Terminate();
    mStandardEffect.Terminate();
}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    
	mAnimationTime += deltaTime;
    while (mAnimationTime > mAnimation.GetDuration())
    {
        mAnimationTime -= mAnimation.GetDuration();
	}
}
void GameState::Render()
{
	mRenderObjectPlatform.transform = mAnimation.GetTransform(mAnimationTime);
	mRenderObjectBall1.transform = mAnimation2.GetTransform(mAnimationTime);
	mRenderObjectBall2.transform = mAnimation3.GetTransform(mAnimationTime);
    SimpleDraw::AddGroundPlane(10.0f, Colors::DarkGray);
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
        mStandardEffect.Render(mRenderObjectPyramid);
		mStandardEffect.Render(mRenderObjectPlatform);
		mStandardEffect.Render(mRenderObjectBall1);
		mStandardEffect.Render(mRenderObjectBall2);
        mStandardEffect.Render(mRenderObjectFloor);
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

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

    ImGui::Separator();

    mStandardEffect.DebugUI();

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
}