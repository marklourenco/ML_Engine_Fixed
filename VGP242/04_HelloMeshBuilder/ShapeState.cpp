#include "ShapeState.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Math;
using namespace ML_Engine::Input;

void ShapeState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f , 0.0f });

    mTransformBuffer.Initialize(sizeof(Math::Matrix4));
    // creates a shape out of the vertices
    CreateShape();
    mMeshBuffer.Initialize(mMesh);

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);


    mTexture.Initialize(L"../../Assets/Textures/space.jpg");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void ShapeState::Terminate()
{
	mTexture.Terminate();
    mSampler.Terminate();
    mTransformBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mMeshBuffer.Terminate();
}

void ShapeState::Update(float deltaTime)
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

    // if 1-4 keys select shapes
    if (input->IsKeyPressed(KeyCode::ONE))      mCurrentShape = ShapeType::Sphere;
	if (input->IsKeyPressed(KeyCode::TWO))      mCurrentShape = ShapeType::PlaneHorizontal;
	if (input->IsKeyPressed(KeyCode::THREE))      mCurrentShape = ShapeType::PlaneVertical;
    if (input->IsKeyPressed(KeyCode::FOUR))    mCurrentShape = ShapeType::SkySphere;

    // terminate and recreate
    if (input->IsKeyPressed(KeyCode::ONE) || input->IsKeyPressed(KeyCode::TWO) ||
        input->IsKeyPressed(KeyCode::THREE) || input->IsKeyPressed(KeyCode::FOUR))
    {
        CreateShape();
        mMeshBuffer.Terminate();
        mMeshBuffer.Initialize(mMesh);
    }

}

void ShapeState::Render()
{
    // prepare GPU
    mVertexShader.Bind();
    mPixelShader.Bind();

    mSampler.BindPS(0);
	mTexture.BindPS(0);

    // sync transform buffer
    mTransformBuffer.BindVS(0);

    // update buffer data
    Math::Matrix4 matWorld = Math::Matrix4::Identity;
    Math::Matrix4 matView = mCamera.GetViewMatrix();
    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    Math::Matrix4 matFinal = matWorld * matView * matProj; // world view projection (mvp)
    Math::Matrix4 wvp = Math::Transpose(matFinal);
    mTransformBuffer.Update(&wvp);

    // draw
    mMeshBuffer.Render();
}

void ShapeState::CreateShape()
{
    // re create shape here (default sphere)

    switch (mCurrentShape)
    {
    case ShapeType::Sphere:
        mMesh = MeshBuilder::CreateSpherePX(30, 30, 1.0f);
        break;
	case ShapeType::PlaneHorizontal:
		mMesh = MeshBuilder::CreatePlanePX(30, 30, 1.0f, true);
		break;
	case ShapeType::PlaneVertical:
		mMesh = MeshBuilder::CreatePlanePX(30, 30, 1.0f, false);
		break;
    case ShapeType::SkySphere:
        mMesh = MeshBuilder::CreateSkySpherePX(30, 30, 200.0f);
        break;
    }
}