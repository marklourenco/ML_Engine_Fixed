#include "GameState.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;
using namespace ML_Engine::Physics;

void GameState::Initialize()
{
    // camera setup
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // light setup
    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    // set up texture manager
    TextureManager* tm = TextureManager::Get();
    
    // ground
    Mesh meshGround = MeshBuilder::CreatePlane(25, 25, 1.0f, true);
    mGroundObject.meshBuffer.Initialize(meshGround);
    mGroundObject.diffuseMapId = tm->LoadTexture("misc/concrete.jpg");

    // walls
    int numWalls = 5;
    for (uint32_t i = 0; i < numWalls; ++i)
    {
        if (i == 0)
        {
            MeshPC meshWall = MeshBuilder::CreateRectanglePC(1.0f, 5.0f, 15.0f);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            // texture
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("misc/concrete.jpg");
            // position (side to side, up and down, front and back)
			mWalls[i].wall.transform.position = { 5.0f, 2.5f, 0.0f };
        }
        else if (i == 1)
        {
            MeshPC meshWall = MeshBuilder::CreateRectanglePC(11.0f, 5.0f, 1.0f);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            // texture
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("misc/concrete.jpg");
            // position (side to side, up and down, front and back)
            mWalls[i].wall.transform.position = { 0.0f, 2.5f, 8.0f };
        }
        else if (i == 2)
        {
            MeshPC meshWall = MeshBuilder::CreateRectanglePC(11.0f, 5.0f, 1.0f);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            // texture
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("misc/concrete.jpg");
            // position (side to side, up and down, front and back)
            mWalls[i].wall.transform.position = { 0.0f, 2.5f, -8.0f };
        }
        else if (i == 3)
        {
            MeshPC meshWall = MeshBuilder::CreateRectanglePC(1.0f, 5.0f, 4.0f);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            // texture
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("misc/concrete.jpg");
            // position (side to side, up and down, front and back)
            mWalls[i].wall.transform.position = { -4.0f, 2.5f, 0.0f };
        }
        else if (i == 4)
        {
            MeshPC meshWall = MeshBuilder::CreateRectanglePC(1.0f, 5.0f, 15.0f);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            // texture
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("misc/concrete.jpg");
            // position (side to side, up and down, front and back)
            mWalls[i].wall.transform.position = { -5.0f, 2.5f, 0.0f };
        }
        else
        {
            MeshPC meshWall = MeshBuilder::CreateRectanglePC(2.0f, 5.0f, 2.0f);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("misc/concrete.jpg");
        }
    }

    // standard effect
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
}
void GameState::Terminate()
{
    mStandardEffect.Terminate();
    for (Wall& wall : mWalls)
    {
        wall.wall.Terminate();
    }
    mWalls.clear();
    mGroundObject.Terminate();
}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    InputSystem* input = InputSystem::Get();
}
void GameState::Render()
{
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
        mStandardEffect.Render(mGroundObject);
        for (const Wall& wall : mWalls)
        {
            mStandardEffect.Render(wall.wall);
        }
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

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