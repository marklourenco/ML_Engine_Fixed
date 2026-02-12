#include "GameState.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;
using namespace ML_Engine::Physics;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    TextureManager* tm = TextureManager::Get();

	MeshPC meshBall1 = MeshBuilder::CreateSpherePC(10.0f, 10.0f, 0.3f);
	mBallObject.meshBuffer.Initialize(meshBall1);
	mBallObject.diffuseMapId = tm->LoadTexture("earth.jpg");
    mBallObject.transform.position.y = 3.0f;
	mBallShape.InitializeSphere(0.3f);
	mBallRigidBody.Initialize(mBallObject.transform, mBallShape, 1.0f);

    Mesh meshGround = MeshBuilder::CreatePlane(10, 10, 1.0f, true);
    mGroundObject.meshBuffer.Initialize(meshGround);
    mGroundObject.diffuseMapId = tm->LoadTexture("bricks.jpg");
	mGroundShape.InitializeBox({ 5.0f, 0.1f, 5.0f });
	mGroundRigidBody.Initialize(mGroundObject.transform, mGroundShape, 0.0f);

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

	Mesh boxShape = MeshBuilder::CreateCube(1.0f);
	TextureId boxTexture = tm->LoadTexture("earth.jpg");

    float yOffset = 4.5f;
	float xOffset = 0.0f;
    int rowCount = 1;
	int boxIndex = 0;
    mBoxes.resize(10);
    while (boxIndex < mBoxes.size())
    {
		xOffset = -((static_cast<float>(rowCount) - 1.0f) * 0.5f);
        for (int r = 0; r < rowCount; ++r)
        {
			BoxData& box = mBoxes[boxIndex];
			box.box.meshBuffer.Initialize(boxShape);
			box.box.diffuseMapId = boxTexture;
			box.box.transform.position = { xOffset, yOffset, 4.0f };
			box.shape.InitializeBox({ 0.5f, 0.5f, 0.5f });
            xOffset += 1.0f;
            ++boxIndex;
        }
		yOffset -= 1.0f;
        rowCount += 1;
    }
    for (int i = mBoxes.size() - 1; i >= 0 ; --i)
    {
		mBoxes[i].rigidBody.Initialize(mBoxes[i].box.transform, mBoxes[i].shape, 4.0f);
	}
}
void GameState::Terminate()
{
    for (BoxData& box : mBoxes)
    {
		box.rigidBody.Terminate();
		box.shape.Terminate();
		box.box.Terminate();
    }
	mBallObject.Terminate();
    mGroundObject.Terminate();
    mStandardEffect.Terminate();
	mBallShape.Terminate();
	mBallRigidBody.Terminate();
	mGroundShape.Terminate();
	mGroundRigidBody.Terminate();
}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    if (InputSystem::Get()->IsMousePressed(MouseButton::LBUTTON))
    {
		Math::Vector3 spawnPos = mCamera.GetPosition() + (mCamera.GetDirection() * 0.5f);
		mBallRigidBody.SetPosition(spawnPos);
		mBallRigidBody.SetVelocity(mCamera.GetDirection() * 20.0f);
	}
}
void GameState::Render()
{
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
		mStandardEffect.Render(mBallObject);
        mStandardEffect.Render(mGroundObject);
        for (BoxData& box : mBoxes)
        {
			mStandardEffect.Render(box.box);
        }
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

    ImGui::Separator();

	Math::Vector3 pos = mBallObject.transform.position;
    if (ImGui::DragFloat3("Ball Position", &pos.x))
    {
		mBallObject.transform.position = pos;
        mBallRigidBody.SetPosition(mBallObject.transform.position);
	}

	PhysicsWorld::Get()->DebugUI();

    ImGui::End();
	SimpleDraw::Render(mCamera);
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