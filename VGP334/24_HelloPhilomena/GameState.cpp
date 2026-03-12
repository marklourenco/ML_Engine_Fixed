#include "GameState.h"
#include "TimeEventManager.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;
using namespace ML_Engine::Physics;

void GameState::Initialize()
{
    // camera setup
    mCamera.SetPosition({ -4.303f, 2.592f, 2.269f });
    mCamera.SetDirection({ -0.895f, -0.128f, 0.428f });

    // light setup
    mDirectionalLight.direction = Math::Normalize({ 0.036f, -0.965f, 0.261f });
    mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    // set up texture manager
    TextureManager* tm = TextureManager::Get();
    
    // ground
    Mesh meshGround = MeshBuilder::CreatePlane(15, 15, 1.0f, true);
    mGroundObject.meshBuffer.Initialize(meshGround);
    mGroundObject.diffuseMapId = tm->LoadTexture("woodfloor/Wooden_Floor_Diffuse.jpg");
    mGroundObject.specMapId = tm->LoadTexture("woodfloor/Wooden_Floor_Specular.jpg");
    mGroundObject.bumpMapId = tm->LoadTexture("woodfloor/Wooden_Floor_Bump.jpg");
    mGroundObject.normalMapId = tm->LoadTexture("woodfloor/Wooden_Floor_Normal.jpg");

    // walls
    int numWalls = 5;
    for (uint32_t i = 0; i < numWalls; ++i)
    {
        if (i == 0)
        {
            Mesh meshWall = MeshBuilder::CreatePlane(15, 15, 1.0f, false);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            // texture
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("wallpaint/Wall_Paint_Diffuse.jpg");
            mWalls[i].wall.specMapId = tm->LoadTexture("wallpaint/Wall_Paint_Specular.jpg");
            mWalls[i].wall.bumpMapId = tm->LoadTexture("wallpaint/Wall_Paint_Bump.jpg");
            mWalls[i].wall.normalMapId = tm->LoadTexture("wallpaint/Wall_Paint_Normal.jpg");
            // position (side to side, up and down, front and back)
			mWalls[i].wall.transform.position = { 7.0f, 2.5f, 0.0f };
            // rotate (quaternion)
            mWalls[i].wall.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90.0f * Math::Constants::DegToRad);
        }
        else if (i == 1)
        {
            Mesh meshWall = MeshBuilder::CreatePlane(15, 15, 1.0f, false);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            // texture
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("wallpaint/Wall_Paint_Diffuse.jpg");
            mWalls[i].wall.specMapId = tm->LoadTexture("wallpaint/Wall_Paint_Specular.jpg");
            mWalls[i].wall.bumpMapId = tm->LoadTexture("wallpaint/Wall_Paint_Bump.jpg");
            mWalls[i].wall.normalMapId = tm->LoadTexture("wallpaint/Wall_Paint_Normal.jpg");
            // position (side to side, up and down, front and back)
            mWalls[i].wall.transform.position = { 0.0f, 2.5f, 7.0f };
        }
        else if (i == 2)
        {
            Mesh meshWall = MeshBuilder::CreatePlane(15, 15, 1.0f, false);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            // texture
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("wallpaint/Wall_Paint_Diffuse.jpg");
            mWalls[i].wall.specMapId = tm->LoadTexture("wallpaint/Wall_Paint_Specular.jpg");
            mWalls[i].wall.bumpMapId = tm->LoadTexture("wallpaint/Wall_Paint_Bump.jpg");
            mWalls[i].wall.normalMapId = tm->LoadTexture("wallpaint/Wall_Paint_Normal.jpg");
            // position (side to side, up and down, front and back)
            mWalls[i].wall.transform.position = { 0.0f, 2.5f, -7.0f };
            // rotate (quaternion)
            //mWalls[i].wall.transform.rotation.y = 0.707f;
            mWalls[i].wall.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -180.0f * Math::Constants::DegToRad);
        }
        else if (i == 3)
        {
            Mesh meshWall = MeshBuilder::CreatePlane(15, 15, 1.0f, false);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            // texture
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("wallpaint/Wall_Paint_Diffuse.jpg");
            mWalls[i].wall.specMapId = tm->LoadTexture("wallpaint/Wall_Paint_Specular.jpg");
            mWalls[i].wall.bumpMapId = tm->LoadTexture("wallpaint/Wall_Paint_Bump.jpg");
            mWalls[i].wall.normalMapId = tm->LoadTexture("wallpaint/Wall_Paint_Normal.jpg");
            // position (side to side, up and down, front and back)
            mWalls[i].wall.transform.position = { -7.0f, 2.5f, 0.0f };
            // rotate (quaternion)
            mWalls[i].wall.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);
        }
        else if (i == 4)
        {
            Mesh meshWall = MeshBuilder::CreatePlane(15, 15, 1.0f, true);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            // texture
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("woodfloor/Wooden_Floor_Diffuse.jpg");
            mWalls[i].wall.specMapId = tm->LoadTexture("woodfloor/Wooden_Floor_Specular.jpg");
            mWalls[i].wall.bumpMapId = tm->LoadTexture("woodfloor/Wooden_Floor_Bump.jpg");
            mWalls[i].wall.normalMapId = tm->LoadTexture("woodfloor/Wooden_Floor_Normal.jpg");
            // position (side to side, up and down, front and back)
            mWalls[i].wall.transform.position = { 0.0f, 5.0f, 0.0f };
            // rotate (quaternion)
            mWalls[i].wall.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad);
        }
        else
        {
            MeshPC meshWall = MeshBuilder::CreateRectanglePC(2.0f, 5.0f, 2.0f);
            Wall& wall = mWalls.emplace_back();
            mWalls[i].wall.meshBuffer.Initialize(meshWall);
            mWalls[i].wall.diffuseMapId = tm->LoadTexture("wallpaint/Wall_Paint_Diffuse.jpg");
            mWalls[i].wall.specMapId = tm->LoadTexture("wallpaint/Wall_Paint_Specular.jpg");
            mWalls[i].wall.bumpMapId = tm->LoadTexture("wallpaint/Wall_Paint_Bump.jpg");
            mWalls[i].wall.normalMapId = tm->LoadTexture("wallpaint/Wall_Paint_Normal.jpg");
        }
    }

    // paintings
    Mesh meshPainting1 = MeshBuilder::CreatePlane(3, 5, 0.75f, false);
    mLastSupperOld.meshBuffer.Initialize(meshPainting1);
    mLastSupperOld.diffuseMapId = tm->LoadTexture("paintings/LastSupperOld.jpg");
    mLastSupperOld.transform.position = { -6.9f, 2.5f, 3.0f };
    mLastSupperOld.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);
    mLastSupperOld.material.emissive = { 0.3f, 0.3f, 0.3f, 255.0f };

    Mesh meshPainting2 = MeshBuilder::CreatePlane(3, 5, 0.75f, false);
    mLastSupperNew.meshBuffer.Initialize(meshPainting2);
    mLastSupperNew.diffuseMapId = tm->LoadTexture("paintings/LastSupperNew.jpg");
    mLastSupperNew.transform.position = { -6.9f, 2.5f, -3.0f };
    mLastSupperNew.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);
    mLastSupperNew.material.emissive = { 0.3f, 0.3f, 0.3f, 255.0f };

    // characters
    ModelManager* mm = ModelManager::Get();
    mPhilomena.Initialize("Philomena/Philomena.model");
    mPhilomena.animator = &mAnimatorPhilomena;
    mm->AddAnimation(mPhilomena.modelId, L"../../Assets/Models/Philomena/Animations/PhilomenaTalking1.animset");
    mAnimatorPhilomena.Initialize(mPhilomena.modelId);
    // scale
    mPhilomena.transform.scale = { 1.5f, 1.5f, 1.5f };
    // position
    mPhilomena.transform.position = { -5.75f, 0.0f, 4.0f };
    // brightness
    for (uint32_t i = 0; i < mPhilomena.renderObjects.size(); ++i)
    {
        mPhilomena.renderObjects[i].material.emissive = { 0.4f, 0.4f, 0.4f, 255.0f };
    }
    // starting anim
	// mAnimatorPhilomena.PlayAnimation(0, true);

    // standard effect
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    // events
    TimeEventManager* tem = TimeEventManager::Get();
    tem->ScheduleEvent(5.0f, [this]()
        {
            LOG("5 seconds passed!");
            mAnimatorPhilomena.PlayAnimation(0, false);
        });
}
void GameState::Terminate()
{
    mStandardEffect.Terminate();
	mPhilomena.Terminate();
	mLastSupperNew.Terminate();
    mLastSupperOld.Terminate();
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

    // timer
	mTimer += deltaTime;
    TimeEventManager::Get()->Update(deltaTime);

    // anim
    mAnimatorPhilomena.Update(deltaTime * mAnimationSpeedPhilomena);

    // input
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
		mStandardEffect.Render(mLastSupperOld);
		mStandardEffect.Render(mLastSupperNew);
		mStandardEffect.Render(mPhilomena);
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    // LIGHT
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

    // CAMERA POSITION & ROTATION
    Math::Vector3 cameraPosition = mCamera.GetPosition();
    Math::Vector3 cameraRotation = mCamera.GetDirection();
    if (ImGui::DragFloat3("CameraPosition", &cameraPosition.x, 0.01f))
    {
        mCamera.SetPosition(cameraPosition);
    }
	if (ImGui::DragFloat3("CameraRotation", &cameraRotation.x, 0.01f))
	{
		mCamera.SetDirection(cameraRotation);
	}

    // CHARACTERS
    ImGui::DragFloat3("PhilomenaPosition", &mPhilomena.transform.position.x, 0.01f);

    // DISPLAY TIMER (up to 2 decimals)
	ImGui::Text("Time: %.2f", mTimer);

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