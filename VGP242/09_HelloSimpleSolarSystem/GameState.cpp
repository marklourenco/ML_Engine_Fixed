#include "GameState.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;

// list of camera targets (for camera & debug UI)
enum class CameraTarget
{
	Sun,
	Mercury,
	Venus,
	Earth,
	Mars,
	Jupiter,
	Saturn,
	Uranus,
	Neptune,
	Pluto,
	Moon
};

// current camera target and names for the debug UI
CameraTarget gCurrentTarget = CameraTarget::Sun;
const char* gTargetNames[] = {
	"Sun", "Mercury", "Venus", "Earth", "Mars",
	"Jupiter", "Saturn", "Uranus", "Neptune", "Pluto", "Moon"
};

bool gDrawOrbit = true;
float gPlanetRotationSpeed = 0.01f;
float gOrbitalRotationSpeed = 0.01f;
float gZoom = 2.5f;

void GameState::Initialize()
{
	// initialize the main camera
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	// initialize the render (debug UI) target camera
	mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	// initialize GPU communication
	mSimpleTextureEffect.Initialize();

	// initialize planets / space (mesh -> texture -> world transform)
	MeshPX space = MeshBuilder::CreateSkySpherePX(60, 60, 40.0f);
	mSpace.mesh.Initialize(space);
	mSpace.textureId = TextureManager::Get()->LoadTexture(L"space.jpg");
	mSpace.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 0.0f });

	MeshPX sun = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
	mSun.mesh.Initialize(sun);
	mSun.textureId = TextureManager::Get()->LoadTexture(L"sun.jpg");
	mSun.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 0.0f });

	MeshPX mercury = MeshBuilder::CreateSpherePX(60, 60, 0.2f);
	mMercury.mesh.Initialize(mercury);
	mMercury.textureId = TextureManager::Get()->LoadTexture(L"planets/mercury.jpg");
	mMercury.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 2.0f });

	MeshPX venus = MeshBuilder::CreateSpherePX(60, 60, 0.5f);
	mVenus.mesh.Initialize(venus);
	mVenus.textureId = TextureManager::Get()->LoadTexture(L"planets/venus.jpg");
	mVenus.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 3.5f });

	MeshPX earth = MeshBuilder::CreateSpherePX(60, 60, 0.5f);
	mEarth.mesh.Initialize(earth);
	mEarth.textureId = TextureManager::Get()->LoadTexture(L"earth.jpg");
	mEarth.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 5.0f });

	MeshPX mars = MeshBuilder::CreateSpherePX(60, 60, 0.3f);
	mMars.mesh.Initialize(mars);
	mMars.textureId = TextureManager::Get()->LoadTexture(L"planets/mars.jpg");
	mMars.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 6.5f });

	MeshPX jupiter = MeshBuilder::CreateSpherePX(60, 60, 0.8f);
	mJupiter.mesh.Initialize(jupiter);
	mJupiter.textureId = TextureManager::Get()->LoadTexture(L"planets/jupiter.jpg");
	mJupiter.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 8.0f });

	MeshPX saturn = MeshBuilder::CreateSpherePX(60, 60, 0.6f);
	mSaturn.mesh.Initialize(saturn);
	mSaturn.textureId = TextureManager::Get()->LoadTexture(L"planets/saturn.jpg");
	mSaturn.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 9.5f });

	MeshPX uranus = MeshBuilder::CreateSpherePX(60, 60, 0.2f);
	mUranus.mesh.Initialize(uranus);
	mUranus.textureId = TextureManager::Get()->LoadTexture(L"planets/uranus.jpg");
	mUranus.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 11.0f });

	MeshPX neptune = MeshBuilder::CreateSpherePX(60, 60, 0.4f);
	mNeptune.mesh.Initialize(neptune);
	mNeptune.textureId = TextureManager::Get()->LoadTexture(L"planets/neptune.jpg");
	mNeptune.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 12.5f });

	MeshPX pluto = MeshBuilder::CreateSpherePX(60, 60, 0.1f);
	mPluto.mesh.Initialize(pluto);
	mPluto.textureId = TextureManager::Get()->LoadTexture(L"planets/pluto.jpg");
	mPluto.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 14.0f });

	MeshPX moon = MeshBuilder::CreateSpherePX(60, 60, 0.2f);
	mMoon.mesh.Initialize(moon);
	mMoon.textureId = TextureManager::Get()->LoadTexture(L"planets/pluto.jpg");
	mMoon.matWorld = Math::Matrix4::Translation({ 0.0f, 0.0f, 0.8f }) * mEarth.matWorld;

	// initialize render target
	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);
}

void GameState::Terminate()
{
	mRenderTarget.Terminate();

	TextureManager::Get()->ReleaseTexture(mSpace.textureId);
	TextureManager::Get()->ReleaseTexture(mSun.textureId);
	TextureManager::Get()->ReleaseTexture(mMercury.textureId);
	TextureManager::Get()->ReleaseTexture(mVenus.textureId);
	TextureManager::Get()->ReleaseTexture(mEarth.textureId);
	TextureManager::Get()->ReleaseTexture(mMars.textureId);
	TextureManager::Get()->ReleaseTexture(mJupiter.textureId);
	TextureManager::Get()->ReleaseTexture(mSaturn.textureId);
	TextureManager::Get()->ReleaseTexture(mUranus.textureId);
	TextureManager::Get()->ReleaseTexture(mNeptune.textureId);
	TextureManager::Get()->ReleaseTexture(mPluto.textureId);
	TextureManager::Get()->ReleaseTexture(mMoon.textureId);

	mSpace.mesh.Terminate();
	mSun.mesh.Terminate();
	mMercury.mesh.Terminate();
	mVenus.mesh.Terminate();
	mEarth.mesh.Terminate();
	mMars.mesh.Terminate();
	mJupiter.mesh.Terminate();
	mSaturn.mesh.Terminate();
	mUranus.mesh.Terminate();
	mNeptune.mesh.Terminate();
	mPluto.mesh.Terminate();
	mMoon.mesh.Terminate();

	mSimpleTextureEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);

	// Orbit circles
	if (gDrawOrbit)
	{
		SimpleDraw::AddGroundCircle(60, 2.0f, Colors::DarkGray, { 0.0f, 0.0f, 0.0f });
		SimpleDraw::AddGroundCircle(60, 3.5f, Colors::DarkGray, { 0.0f, 0.0f, 0.0f });
		SimpleDraw::AddGroundCircle(60, 5.0f, Colors::DarkGray, { 0.0f, 0.0f, 0.0f });
		SimpleDraw::AddGroundCircle(60, 6.5f, Colors::DarkGray, { 0.0f, 0.0f, 0.0f });
		SimpleDraw::AddGroundCircle(60, 8.0f, Colors::DarkGray, { 0.0f, 0.0f, 0.0f });
		SimpleDraw::AddGroundCircle(60, 9.5f, Colors::DarkGray, { 0.0f, 0.0f, 0.0f });
		SimpleDraw::AddGroundCircle(60, 11.0f, Colors::DarkGray, { 0.0f, 0.0f, 0.0f });
		SimpleDraw::AddGroundCircle(60, 12.5f, Colors::DarkGray, { 0.0f, 0.0f, 0.0f });
		SimpleDraw::AddGroundCircle(60, 14.0f, Colors::DarkGray, { 0.0f, 0.0f, 0.0f });
	}

	// local rotation
	mSun.matWorld = Math::Matrix4::RotationY((0.005f + gPlanetRotationSpeed) * deltaTime) * mSun.matWorld;
	mMercury.matWorld = Math::Matrix4::RotationY((0.1f + gPlanetRotationSpeed) * deltaTime) * mMercury.matWorld;
	mVenus.matWorld = Math::Matrix4::RotationY((0.05f + gPlanetRotationSpeed) * deltaTime) * mVenus.matWorld;
	mEarth.matWorld = Math::Matrix4::RotationY((0.174533f + gPlanetRotationSpeed) * deltaTime) * mEarth.matWorld;
	mMars.matWorld = Math::Matrix4::RotationY((0.2f + gPlanetRotationSpeed) * deltaTime) * mMars.matWorld;
	mJupiter.matWorld = Math::Matrix4::RotationY((0.5f + gPlanetRotationSpeed) * deltaTime) * mJupiter.matWorld;
	mSaturn.matWorld = Math::Matrix4::RotationY((0.3f + gPlanetRotationSpeed) * deltaTime) * mSaturn.matWorld;
	mUranus.matWorld = Math::Matrix4::RotationY((0.1f + gPlanetRotationSpeed) * deltaTime) * mUranus.matWorld;
	mNeptune.matWorld = Math::Matrix4::RotationY((0.05f + gPlanetRotationSpeed) * deltaTime) * mNeptune.matWorld;
	mPluto.matWorld = Math::Matrix4::RotationY((0.008f + gPlanetRotationSpeed) * deltaTime) * mPluto.matWorld;
	mMoon.matWorld = Math::Matrix4::RotationY((0.02355f + gPlanetRotationSpeed) * deltaTime) * mMoon.matWorld;

	mSpace.matWorld = Math::Matrix4::RotationAxis(Math::Vector3::YAxis, (0.001f + 0.005f) * deltaTime) * mSpace.matWorld;
	mSpace.matWorld = Math::Matrix4::RotationAxis(Math::Vector3::XAxis, (0.001f + 0.005f) * deltaTime) * mSpace.matWorld;

	// rotate around the sun
	mMercury.matWorld = mMercury.matWorld * Math::Matrix4::RotationY((0.4787f + gOrbitalRotationSpeed) * deltaTime);
	mVenus.matWorld = mVenus.matWorld * Math::Matrix4::RotationY((0.3502f + gOrbitalRotationSpeed) * deltaTime);
	mEarth.matWorld = mEarth.matWorld * Math::Matrix4::RotationY((0.2978f + gOrbitalRotationSpeed) * deltaTime);
	mMars.matWorld = mMars.matWorld * Math::Matrix4::RotationY((0.24077f + gOrbitalRotationSpeed) * deltaTime);
	mJupiter.matWorld = mJupiter.matWorld * Math::Matrix4::RotationY((0.1307f + gOrbitalRotationSpeed) * deltaTime);
	mSaturn.matWorld = mSaturn.matWorld * Math::Matrix4::RotationY((0.0969f + gOrbitalRotationSpeed) * deltaTime);
	mUranus.matWorld = mUranus.matWorld * Math::Matrix4::RotationY((0.0681f + gOrbitalRotationSpeed) * deltaTime);
	mNeptune.matWorld = mNeptune.matWorld * Math::Matrix4::RotationY((0.0543f + gOrbitalRotationSpeed) * deltaTime);
	mPluto.matWorld = mPluto.matWorld * Math::Matrix4::RotationY((0.0474f + gOrbitalRotationSpeed) * deltaTime);
	mMoon.matWorld = Math::Matrix4::RotationY((0.0103f + gOrbitalRotationSpeed) * deltaTime) * Math::Matrix4::Translation({ 0.0f, 0.0f, 0.8f }) * mEarth.matWorld;

	// camera target
	Math::Vector3 targetPosition = Math::Vector3::Zero;
	switch (gCurrentTarget)
	{
	case CameraTarget::Sun:
		targetPosition = Math::GetTranslation(mSun.matWorld);
		break;
	case CameraTarget::Mercury:
		targetPosition = Math::GetTranslation(mMercury.matWorld);
		break;
	case CameraTarget::Venus:
		targetPosition = Math::GetTranslation(mVenus.matWorld);
		break;
	case CameraTarget::Earth:
		targetPosition = Math::GetTranslation(mEarth.matWorld);
		break;
	case CameraTarget::Mars:
		targetPosition = Math::GetTranslation(mMars.matWorld);
		break;
	case CameraTarget::Jupiter:
		targetPosition = Math::GetTranslation(mJupiter.matWorld);
		break;
	case CameraTarget::Saturn:
		targetPosition = Math::GetTranslation(mSaturn.matWorld);
		break;
	case CameraTarget::Uranus:
		targetPosition = Math::GetTranslation(mUranus.matWorld);
		break;
	case CameraTarget::Neptune:
		targetPosition = Math::GetTranslation(mNeptune.matWorld);
		break;
	case CameraTarget::Pluto:
		targetPosition = Math::GetTranslation(mPluto.matWorld);
		break;
	case CameraTarget::Moon:
		targetPosition = Math::GetTranslation(mMoon.matWorld);
		break;
	}

	Math::Vector3 direction = Math::Vector3(0.0f, 0.0f, -1.0f);
	Math::Vector3 cameraPosition = targetPosition - direction * gZoom;
	mRenderTargetCamera.SetPosition(cameraPosition);
	mRenderTargetCamera.SetLookAt(targetPosition);
}

void GameState::Render()
{
	SimpleDraw::Render(mCamera);

	// render to the render target
	mRenderTarget.BeginRender();
	switch (gCurrentTarget)
	{
	case CameraTarget::Sun:
		RenderObject(mSun, mRenderTargetCamera);
		break;
	case CameraTarget::Mercury:
		RenderObject(mMercury, mRenderTargetCamera);
		break;
	case CameraTarget::Venus:
		RenderObject(mVenus, mRenderTargetCamera);
		break;
	case CameraTarget::Earth:
		RenderObject(mEarth, mRenderTargetCamera);
		break;
	case CameraTarget::Mars:
		RenderObject(mMars, mRenderTargetCamera);
		break;
	case CameraTarget::Jupiter:
		RenderObject(mJupiter, mRenderTargetCamera);
		break;
	case CameraTarget::Saturn:
		RenderObject(mSaturn, mRenderTargetCamera);
		break;
	case CameraTarget::Uranus:
		RenderObject(mUranus, mRenderTargetCamera);
		break;
	case CameraTarget::Neptune:
		RenderObject(mNeptune, mRenderTargetCamera);
		break;
	case CameraTarget::Pluto:
		RenderObject(mPluto, mRenderTargetCamera);
		break;
	case CameraTarget::Moon:
		RenderObject(mMoon, mRenderTargetCamera);
		break;
	}
	mRenderTarget.EndRender();

	// render to the scene
	RenderObject(mSpace, mCamera);
	RenderObject(mSun, mCamera);
	RenderObject(mMercury, mCamera);
	RenderObject(mVenus, mCamera);
	RenderObject(mEarth, mCamera);
	RenderObject(mMars, mCamera);
	RenderObject(mJupiter, mCamera);
	RenderObject(mSaturn, mCamera);
	RenderObject(mUranus, mCamera);
	RenderObject(mNeptune, mCamera);
	RenderObject(mPluto, mCamera);
	RenderObject(mMoon, mCamera);
}

void GameState::RenderObject(const SimpleTextureEffect::RenderData& object, const Camera& camera)
{
	mSimpleTextureEffect.SetCamera(camera);
	mSimpleTextureEffect.Begin();
	mSimpleTextureEffect.Render(object);
	mSimpleTextureEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug UI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Options");
	ImGui::Checkbox("Draw Orbit", &gDrawOrbit);
	
	ImGui::DragFloat("Planet Rotation Speed", &gPlanetRotationSpeed, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Orbital Rotation Speed", &gOrbitalRotationSpeed, 0.01f, 0.0f, 1.0f);
	
	ImGui::Separator();
	
	ImGui::Text("Render Target");
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 }
	);
	
	int currentTarget = static_cast<int>(gCurrentTarget);
	if (ImGui::Combo("Camera Target", &currentTarget, gTargetNames, IM_ARRAYSIZE(gTargetNames)))
	{
		gCurrentTarget = static_cast<CameraTarget>(currentTarget);
		Math::Vector3 targetPosition = Math::Vector3::Zero;
		switch (gCurrentTarget)
		{
		case CameraTarget::Sun:
			targetPosition = Math::GetTranslation(mSun.matWorld);
			break;
		case CameraTarget::Mercury:
			targetPosition = Math::GetTranslation(mMercury.matWorld);
			break;
		case CameraTarget::Venus:
			targetPosition = Math::GetTranslation(mVenus.matWorld);
			break;
		case CameraTarget::Earth:
			targetPosition = Math::GetTranslation(mEarth.matWorld);
			break;
		case CameraTarget::Mars:
			targetPosition = Math::GetTranslation(mMars.matWorld);
			break;
		case CameraTarget::Jupiter:
			targetPosition = Math::GetTranslation(mJupiter.matWorld);
			break;
		case CameraTarget::Saturn:
			targetPosition = Math::GetTranslation(mSaturn.matWorld);
			break;
		case CameraTarget::Uranus:
			targetPosition = Math::GetTranslation(mUranus.matWorld);
			break;
		case CameraTarget::Neptune:
			targetPosition = Math::GetTranslation(mNeptune.matWorld);
			break;
		case CameraTarget::Pluto:
			targetPosition = Math::GetTranslation(mPluto.matWorld);
			break;
		case CameraTarget::Moon:
			targetPosition = Math::GetTranslation(mMoon.matWorld);
			break;
		}

		Math::Vector3 direction = Math::Vector3(0.0f, 0.0f, -1.0f);
		Math::Vector3 cameraPosition = targetPosition - direction * gZoom;
		mRenderTargetCamera.SetPosition(cameraPosition);
		mRenderTargetCamera.SetLookAt(targetPosition);

	}

	ImGui::DragFloat("Zoom", &gZoom, 0.01f, 2.0f, 4.0f);

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