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

    Mesh meshGround = MeshBuilder::CreatePlane(100, 100, 1.0f, true);
    mGroundObject.meshBuffer.Initialize(meshGround);
    mGroundObject.diffuseMapId = tm->LoadTexture("bricks.jpg");
	mGroundShape.InitializeBox({ 50.0f, 0.1f, 5.0f });
	mGroundRigidBody.Initialize(mGroundObject.transform, mGroundShape, 0.0f);

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    uint32_t numSegments = 10;
    float maxSphereSize = 1.0f;
    float minSphereSize = 0.25f;
    for (uint32_t i = 0; i < numSegments; ++i)
    {
        const float t =( 1.0f - static_cast<float>(i) / static_cast<float>(numSegments - 1));
        float sphereSize = Math::Lerp(minSphereSize, maxSphereSize, t);
        Mesh sphere = MeshBuilder::CreateSphere(20, 20, sphereSize);
        Segment& part = mSegments.emplace_back();
        part.segment.meshBuffer.Initialize(sphere);
        part.segment.diffuseMapId = tm->LoadTexture("Images/blue.jpg");
        part.range = sphereSize;
        if (i > 0)
        {
            Math::Vector3 prevPos = mSegments[i - 1].segment.transform.position;
            float prevRange = mSegments[i - 1].range;
            part.segment.transform.position.x -= (prevPos.x - prevRange);
        }
    }
    mBodyAnchorShape.InitializeSphere(maxSphereSize);
    mBodyAnchor.Initialize(mSegments[0].segment.transform, mBodyAnchorShape, 1.0f);
}
void GameState::Terminate()
{
    mBodyAnchor.Terminate();
    mBodyAnchorShape.Terminate();
    for (Segment& segment : mSegments)
    {
        segment.segment.Terminate();
    }
    mSegments.clear();
    mStandardEffect.Terminate();
	mGroundRigidBody.Terminate();
	mGroundShape.Terminate();
    mGroundObject.Terminate();
}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    InputSystem* input = InputSystem::Get();
    if (input->IsMousePressed(MouseButton::LBUTTON))
    {
        Math::Vector3 spawnPos = mCamera.GetPosition() + (mCamera.GetDirection() * 0.5f);
        mBodyAnchor.SetPosition(spawnPos);
        mBodyAnchor.SetVelocity(mCamera.GetDirection() * 20.0f);
    }
    if (input->IsKeyDown(KeyCode::RSHIFT))
    {
        Math::Vector3 velocity = Math::Vector3::Zero;
        velocity.y = mBodyAnchor.GetVelocity().y;
        if (input->IsKeyDown(KeyCode::UP))
        {
            velocity.z = +1.0f;
        }
        if (input->IsKeyDown(KeyCode::DOWN))
        {
            velocity.z -= 1.0f;
        }
        if (input->IsKeyDown(KeyCode::RIGHT))
        {
            velocity.x = +1.0f;
        }
        if (input->IsKeyDown(KeyCode::LEFT))
        {
            velocity.x -= 1.0f;
        }
        if (velocity.y < 1.0f && input->IsKeyDown(KeyCode::SPACE))
        {
            velocity.y = 10.0f;
        }
        mBodyAnchor.SetVelocity(velocity);
    }
    for (uint32_t i = 1; i < mSegments.size(); ++i)
    {
        Segment& seg = mSegments[i];
        seg.segment.transform.position.y -= 9.81f * deltaTime;
    }
}
void GameState::Render()
{
    for (uint32_t i = 1; i < mSegments.size(); ++i)
    {
        Segment& prevSeg = mSegments[i - 1];
        Segment& seg = mSegments[i];

        seg.segment.transform.position.y = Math::Max(seg.segment.transform.position.y, seg.range);
        Math::Vector3 dir = seg.segment.transform.position - prevSeg.segment.transform.position;
        if (Math::MagnitudeSqr(dir) > 0.0f)
        {
            dir = Math::Normalize(dir);
        }
        else
        {
            dir = Math::Vector3::ZAxis;
        }
        seg.segment.transform.position = prevSeg.segment.transform.position + (dir * prevSeg.range);
        seg.segment.transform.position.y = Math::Max(seg.segment.transform.position.y, seg.range);
    }

   //  SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
        mStandardEffect.Render(mGroundObject);
        for (const Segment& segment : mSegments)
        {
            mStandardEffect.Render(segment.segment);
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

	//Math::Vector3 pos = mBallObject.transform.position;
 //   if (ImGui::DragFloat3("Ball Position", &pos.x))
 //   {
	//	mBallObject.transform.position = pos;
 //       mBallRigidBody.SetPosition(mBallObject.transform.position);
	//}

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