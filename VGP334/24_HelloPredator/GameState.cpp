#include "GameState.h"
#include "GameEvents.h"
#include "TimeEventManager.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;
using namespace ML_Engine::Input;
using namespace ML_Engine::Physics;
using namespace ML_Engine::Audio;

void GameState::Initialize()
{
    // camera setup
    mCamera.SetPosition({ 122.0f, 22.0f, 96.0f });
    mCamera.SetDirection({ 0.896f, -0.258f, 0.501f });

    // light setup
    mDirectionalLight.direction = Math::Normalize({ 0.036f, -0.965f, 0.261f });
    mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
    mDirectionalLight.diffuse = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    // set up texture manager
    TextureManager* tm = TextureManager::Get();

    // military base
	mMilitaryBase.Initialize("MilitaryBase/MilitaryBase.model");
    for (uint32_t i = 0; i < mMilitaryBase.renderObjects.size(); ++i)
    {
        mMilitaryBase.renderObjects[i].material.emissive = { 0.4f, 0.4f, 0.4f, 255.0f };
    }
    mMilitaryBase.transform.position = { 157.48f, -6.34f, -158.52f };
    mMilitaryBase.transform.rotation = { 0.7, 0.0f, 0.0f, 1.0f };

    // sky sphere
    MeshPX skysphere = MeshBuilder::CreateSkySpherePX(60, 60, 100.0f);
    mSkySphere.diffuseMapId = tm->LoadTexture("nightsky.jpg");

    // terrain
    mTerrain.Initialize(L"../../Assets/Textures/terrain/heightmap_512x512.raw", 20.0f);
    mGround.meshBuffer.Initialize(mTerrain.mesh);
    mGround.diffuseMapId = tm->LoadTexture("terrain/dirt_seamless.jpg");
    mGround.specMapId = tm->LoadTexture("terrain/grass_2048.jpg");

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);

    mTerrainEffect.Initialize();
    mTerrainEffect.SetCamera(mCamera);
    mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mTerrainEffect.SetDirectionalLight(mDirectionalLight);
    mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mGround.transform.position = { -250.0f, 0.0f, -250.0f };

    mGround.material.ambient = { 0.0f, 0.0f, 0.0f, 255.0f };


    // characters
    ModelManager* mm = ModelManager::Get();
    mPredator.Initialize("Predator/Predator.model");
    mPredator.animator = &mAnimatorPredator;
    mm->AddAnimation(mPredator.modelId, L"../../Assets/Models/Predator/Animations/PredatorAttackDoubleSlash.animset");
    mm->AddAnimation(mPredator.modelId, L"../../Assets/Models/Predator/Animations/PredatorIdle.animset");
    mm->AddAnimation(mPredator.modelId, L"../../Assets/Models/Predator/Animations/PredatorInteract.animset");
    mm->AddAnimation(mPredator.modelId, L"../../Assets/Models/Predator/Animations/PredatorJump.animset");
    mm->AddAnimation(mPredator.modelId, L"../../Assets/Models/Predator/Animations/PredatorPoint.animset");
    mm->AddAnimation(mPredator.modelId, L"../../Assets/Models/Predator/Animations/PredatorRun.animset");
    mAnimatorPredator.Initialize(mPredator.modelId);
    mPredator.transform.scale = { 1.0f, 1.0f, 1.0f };
    mPredator.transform.position = { 100.0f, 10.91f, 131.75f };
    // deg to rad
    mPredator.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90 * Math::Constants::DegToRad);
    // starting anim
    mAnimatorPredator.PlayAnimation(1, true);
    
    mSoldier1.Initialize("Soldier/Soldier.model");
    mSoldier1.animator = &mAnimatorSoldier1;
    mm->AddAnimation(mSoldier1.modelId, L"../../Assets/Models/Soldier/Animations/SoldierDeath.animset");
    mm->AddAnimation(mSoldier1.modelId, L"../../Assets/Models/Soldier/Animations/SoldierFight.animset");
    mm->AddAnimation(mSoldier1.modelId, L"../../Assets/Models/Soldier/Animations/SoldierIdle.animset");
    mm->AddAnimation(mSoldier1.modelId, L"../../Assets/Models/Soldier/Animations/SoldierRun.animset");
    mm->AddAnimation(mSoldier1.modelId, L"../../Assets/Models/Soldier/Animations/SoldierWalk.animset");
    mAnimatorSoldier1.Initialize(mSoldier1.modelId);
    mSoldier1.transform.scale = { 2.0f, 2.0f, 2.0f };
	mSoldier1.transform.position = { 156.88f, 5.94f, 161.97f };
	// mSoldier1.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad);
    mAnimatorSoldier1.PlayAnimation(4, true);

    mSoldier2.Initialize("Soldier/Soldier.model");
    mSoldier2.animator = &mAnimatorSoldier2;
    mm->AddAnimation(mSoldier2.modelId, L"../../Assets/Models/Soldier/Animations/SoldierDeath.animset");
    mm->AddAnimation(mSoldier2.modelId, L"../../Assets/Models/Soldier/Animations/SoldierFight.animset");
    mm->AddAnimation(mSoldier2.modelId, L"../../Assets/Models/Soldier/Animations/SoldierIdle.animset");
    mm->AddAnimation(mSoldier2.modelId, L"../../Assets/Models/Soldier/Animations/SoldierRun.animset");
    mm->AddAnimation(mSoldier2.modelId, L"../../Assets/Models/Soldier/Animations/SoldierWalk.animset");
    mAnimatorSoldier2.Initialize(mSoldier2.modelId);
    mSoldier2.transform.scale = { 2.0f, 2.0f, 2.0f };
	mSoldier2.transform.position = { 180.79f, 0.1f, 143.12f };
	// mSoldier2.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad);
    mAnimatorSoldier2.PlayAnimation(2, true);

    mSoldier3.Initialize("Soldier/Soldier.model");
    mSoldier3.animator = &mAnimatorSoldier3;
    mm->AddAnimation(mSoldier3.modelId, L"../../Assets/Models/Soldier/Animations/SoldierDeath.animset");
    mm->AddAnimation(mSoldier3.modelId, L"../../Assets/Models/Soldier/Animations/SoldierFight.animset");
    mm->AddAnimation(mSoldier3.modelId, L"../../Assets/Models/Soldier/Animations/SoldierIdle.animset");
    mm->AddAnimation(mSoldier3.modelId, L"../../Assets/Models/Soldier/Animations/SoldierRun.animset");
    mm->AddAnimation(mSoldier3.modelId, L"../../Assets/Models/Soldier/Animations/SoldierWalk.animset");
    mAnimatorSoldier3.Initialize(mSoldier3.modelId);
    mSoldier3.transform.scale = { 2.0f, 2.0f, 2.0f };
	mSoldier3.transform.position = { 180.79f, 0.3f, 127.04f };
	mSoldier3.transform.rotation = Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad);
    mAnimatorSoldier3.PlayAnimation(2, true);

    // trucks

    mMilitaryTruck1.Initialize("MilitaryTruck/MilitaryTruck.model");
    mMilitaryTruck1.transform.scale = { 5.0f, 5.0f, 5.0f };

    mMilitaryTruck2.Initialize("MilitaryTruck/MilitaryTruck.model");
    mMilitaryTruck2.transform.scale = { 5.0f, 5.0f, 5.0f };

    mMilitaryTruck3.Initialize("MilitaryTruck/MilitaryTruck.model");
    mMilitaryTruck3.transform.scale = { 5.0f, 5.0f, 5.0f };

    // brightness
    for (uint32_t i = 0; i < mPredator.renderObjects.size(); ++i)
    {
        mPredator.renderObjects[i].material.emissive = { 0.4f, 0.4f, 0.4f, 255.0f };
    }

    // truck painting bc diffuse maps were not loading
    for (uint32_t i = 0; i < mMilitaryTruck1.renderObjects.size(); ++i)
    {
        mMilitaryTruck1.renderObjects[i].material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
        mMilitaryTruck1.renderObjects[i].material.diffuse = { 0.3f, 0.3f, 0.3f, 1.0f };
		mMilitaryTruck1.renderObjects[i].material.specular = { 0.3f, 0.3f, 0.3f, 1.0f };
        mMilitaryTruck1.renderObjects[i].material.emissive = { 0.3f, 0.3f, 0.3f, 1.0f };

        mMilitaryTruck2.renderObjects[i].material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
        mMilitaryTruck2.renderObjects[i].material.diffuse = { 0.3f, 0.3f, 0.3f, 1.0f };
        mMilitaryTruck2.renderObjects[i].material.specular = { 0.3f, 0.3f, 0.3f, 1.0f };
        mMilitaryTruck2.renderObjects[i].material.emissive = { 0.3f, 0.3f, 0.3f, 1.0f };

        mMilitaryTruck3.renderObjects[i].material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
        mMilitaryTruck3.renderObjects[i].material.diffuse = { 0.3f, 0.3f, 0.3f, 1.0f };
        mMilitaryTruck3.renderObjects[i].material.specular = { 0.3f, 0.3f, 0.3f, 1.0f };
        mMilitaryTruck3.renderObjects[i].material.emissive = { 0.3f, 0.3f, 0.3f, 1.0f };
    }

    // bullet
    Mesh circle = MeshBuilder::CreateSphere(16, 16, 1.0f);
    mBullet.meshBuffer.Initialize(circle);
	mBullet.material.diffuse = Graphics::Colors::Red;
	mBullet.material.specular = Graphics::Colors::Red;
	mBullet.material.ambient = Graphics::Colors::Red;
	mBullet.material.emissive = Graphics::Colors::Red;

    // standard effect
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    // animation
    mCameraTransformAnimation = AnimationBuilder()
		.AddPositionKey({ 122.0f, 22.0f, 96.0f }, 0.0f)
        .AddRotationKey({ 0.896f, -0.258f, 0.501f, 1.0f }, 0.0f)
        .AddPositionKey({ 130.493f, 22.0f, 100.687 }, 7.0f)
        .AddRotationKey({ 0.896f, -0.258f, 0.501f, 1.0f }, 7.0f)
		.AddPositionKey({ 98.542f, 14.174f, 132.546 }, 7.01f)
        .AddRotationKey({ 0.978f, -0.197f, -0.073f, 1.0f }, 7.01f)
		.AddPositionKey({ 98.542f, 14.174f, 132.546 }, 9.0f)
		.AddPositionKey({ 99.613f, 13.922f, 132.143 }, 12.0f)
		.AddPositionKey({ 100.526f, 13.723f, 131.622 }, 12.01f)
		.AddPositionKey({ 100.526f, 13.723f, 131.622 }, 15.0f)
        .AddRotationKey({ 0.978f, -0.197f, -0.073f, 1.0f }, 15.0f)
		.AddPositionKey({ 140.366f, 9.683f, 167.18 }, 15.01f)
        .AddRotationKey({ 0.685f, -0.371f, -0.627f, 1.0f }, 15.01f)
		.AddPositionKey({ 140.366f, 9.683f, 167.18 }, 18.15f)
        .AddRotationKey({ 0.685f, -0.371f, -0.627f, 1.0f }, 18.15f)
		.AddPositionKey({ 155.358f, 8.364f, 148.186 }, 18.16f)
        .AddRotationKey({ 0.407f, -0.237f, 0.882f, 1.0f }, 18.161f)
		.AddPositionKey({ 155.358f, 8.364f, 148.186 }, 22.4f)
        .AddRotationKey({ 0.407f, -0.237f, 0.882f, 1.0f }, 22.4f)
		.AddPositionKey({ 150.877f, 12.867f, 132.168 }, 22.5f)
        .AddRotationKey({ 0.819f, -0.316f, 0.479f, 1.0f }, 22.51f)
		.AddPositionKey({ 150.877f, 12.867f, 132.168 }, 24.99f)
        .AddRotationKey({ 0.819f, -0.316f, 0.479f, 1.0f }, 24.99f)
		.AddPositionKey({ 178.138f, 3.517f, 145.603 }, 25.0f)
        .AddRotationKey({ 0.157f, -0.281f, -0.947f, 1.0f }, 25.0f)
        .AddPositionKey({ 178.138f, 3.517f, 145.603 }, 31.99f)
        .AddRotationKey({ 0.157f, -0.281f, -0.947f, 1.0f }, 31.99f)
        .AddPositionKey({ 77.943f, 8.039f, 34.062 }, 32.0f)
        .AddRotationKey({ 0.701f, -0.224f, 0.678f, 1.0f }, 32.0f)
        .AddPositionKey({ 90.482f, 7.934f, 46.568 }, 35.5f)
        .AddRotationKey({ 0.701f, -0.224f, 0.678f, 1.0f }, 35.5f)
        .AddPositionKey({ 177.105f, 3.656f, 140.975f }, 35.51f)
        .AddRotationKey({ 0.767f, -0.348f, 0.54f, 1.0f }, 35.511f)
        .AddPositionKey({ 177.105f, 3.656f, 140.975f }, 37.0f)
        .AddRotationKey({ 0.767f, -0.348f, 0.54f, 1.0f }, 37.0f)
        .AddPositionKey({ 180.47f, 3.196f, 145.175f }, 37.0f)
        .AddRotationKey({ -0.610f, -0.140f, -0.780f, 1.0f }, 37.0f)
        .Build();

    mPredatorTransformAnimation = AnimationBuilder()
        .AddPositionKey({ 100.0f, 10.91f, 131.75f }, 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90 * Math::Constants::DegToRad), 0.0f)
        .AddPositionKey({ 100.0f, 10.91f, 131.75f }, 15.0f)
        .AddPositionKey({ 138.0f, -0.05f, 160.57f }, 15.01f)
        .AddPositionKey({ 152.0f, -0.05f, 160.57f }, 16.5f)
        .AddPositionKey({ 152.0f, -0.05f, 160.57f }, 17.85f)
        .AddPositionKey({ 154.66f, 7.64f, 160.57f }, 18.0f)
        .AddPositionKey({ 156.88f, 5.77f, 160.57f }, 18.15f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90 * Math::Constants::DegToRad), 18.16f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 0 * Math::Constants::DegToRad), 18.161f)
        .AddPositionKey({ 156.88f, 5.77f, 152.0f }, 19.5f)
        .AddPositionKey({ 156.88f, 5.77f, 152.0f }, 22.4f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 0 * Math::Constants::DegToRad), 22.4f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90 * Math::Constants::DegToRad), 22.4f)
        .AddPositionKey({ 156.877f, 5.77f, 149.86f }, 22.5f)
        .AddPositionKey({ 156.877f, 5.77f, 149.86f }, 23.85f)
        .AddPositionKey({ 168.327f, 6.93f, 146.23f }, 24.0f)
        .AddPositionKey({ 179.67f, 0.03f, 143.21f }, 24.15f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90 * Math::Constants::DegToRad), 24.99f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 0 * Math::Constants::DegToRad), 25.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 0 * Math::Constants::DegToRad), 34.99f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 45 * Math::Constants::DegToRad), 35.0f)
        .Build();
    
    mSoldier1TransformAnimation = AnimationBuilder()
        .AddScaleKey({ 1.5f, 1.5f, 1.5f }, 0.0f)
        .AddPositionKey({ 156.88f, 5.77f, 150.0f }, 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 180 * Math::Constants::DegToRad), 0.0f)
        .AddPositionKey({ 156.88f, 5.94f, 161.97f }, 7.0f)
        .AddPositionKey({ 156.88f, 5.94f, 161.97f }, 10.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 180 * Math::Constants::DegToRad), 10.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad), 10.01f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 0 * Math::Constants::DegToRad), 10.02f)
        .AddPositionKey({ 156.88f, 5.77f, 150.0f }, 17.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 0 * Math::Constants::DegToRad), 17.01f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad), 17.02f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 180 * Math::Constants::DegToRad), 17.03f)
        .AddPositionKey({ 156.88f, 5.77f, 150.0f }, 22.49f)
        .AddPositionKey({ 156.88f, 5.77f, 151.39f }, 22.5f) // end of the line
        .Build();

    mSoldier2TransformAnimation = AnimationBuilder()
        .AddScaleKey({ 1.5f, 1.5f, 1.5f }, 0.0f)
        .AddPositionKey({ 180.79f, 0.1f, 143.12f }, 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad), 0.0f) // end of the line
        .AddPositionKey({ 180.79f, 0.1f, 143.12f }, 35.0f)
        .AddPositionKey({ 180.0f, 0.1f, 143.12f }, 35.1f)
        .Build();

    mSoldier3TransformAnimation = AnimationBuilder()
    .AddScaleKey({ 1.5f, 1.5f, 1.5f }, 0.0f)
        .AddPositionKey({ 180.79f, 0.3f, 127.04f }, 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad), 0.0f)
        .AddPositionKey({ 180.79f, 0.3f, 127.04f }, 24.99f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad), 24.99f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 0 * Math::Constants::DegToRad), 25.0f)
        .AddPositionKey({ 180.79f, 0.3f, 115.0f }, 30.0f) // end of the line
        .Build();

    mMilitaryTruck1TransformAnimation = AnimationBuilder()
        .AddScaleKey({ 200.0f, 200.0f, 200.0f }, 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad), 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 135 * Math::Constants::DegToRad), 0.0f)
		.AddPositionKey({ 79.0f, 3.0f, 35.0f }, 32.0f)
		.AddPositionKey({ 109.0f, 3.0f, 65.0f }, 40.0f)
		.AddPositionKey({ 109.0f, 3.0f, 65.0f }, 42.99f)
		.AddPositionKey({ 109.0f, 1000.0f, 65.0f }, 43.0f)
        .Build();
    
    mMilitaryTruck2TransformAnimation = AnimationBuilder()
        .AddScaleKey({ 200.0f, 200.0f, 200.0f }, 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad), 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 135 * Math::Constants::DegToRad), 0.0f)
        .AddPositionKey({ 79.0f, 3.0f, 45.0f }, 32.0f)
        .AddPositionKey({ 109.0f, 3.0f, 75.0f }, 40.0f)
        .AddPositionKey({ 109.0f, 3.0f, 75.0f }, 42.99f)
        .AddPositionKey({ 109.0f, 1000.0f, 75.0f }, 43.0f)
        .Build();
    
    mMilitaryTruck3TransformAnimation = AnimationBuilder()
        .AddScaleKey({ 200.0f, 200.0f, 200.0f }, 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90 * Math::Constants::DegToRad), 0.0f)
        .AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 135 * Math::Constants::DegToRad), 0.0f)
        .AddPositionKey({ 89.0f, 3.0f, 35.0f }, 32.0f)
        .AddPositionKey({ 119.0f, 3.0f, 65.0f }, 40.0f)
        .AddPositionKey({ 119.0f, 3.0f, 65.0f }, 42.99f)
        .AddPositionKey({ 119.0f, 1000.0f, 65.0f }, 43.0f)
        .Build();

    mBulletTransformAnimation = AnimationBuilder()
        .AddScaleKey({ 0.05f, 0.05f, 0.05f }, 0.0f)
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 0.0f)
        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 29.19f)
        .AddPositionKey({ 179.36f, 2.096f, 142.362f }, 29.2f)
        .AddPositionKey({ 180.79f, 2.096f, 115.0f }, 30.0f)
        .AddPositionKey({ 179.36f, 100.0f, 142.362f }, 30.01f)
        .AddPositionKey({ 179.36f, 100.0f, 142.362f }, 37.49f)
        .AddPositionKey({ 180.289f, 2.582f, 142.953f }, 37.5f)
        .AddPositionKey({ 180.289f, 30.0f, 142.953f }, 38.0f)
        .AddPositionKey({ 180.289f, 30.0f, 142.953f }, 42.0f)
        .AddPositionKey({ 109.0f, 50.0f, 65.0f }, 42.01f)
        .AddScaleKey({ 0.05f, 0.05f, 0.05f }, 42.01f)
        .AddScaleKey({ 0.5f, 0.5f, 0.5f }, 42.02f)
        .AddPositionKey({ 109.0f, 3.0f, 65.0f }, 43.0f)
        .AddPositionKey({ 119.0f, 1000.0f, 65.0f }, 43.01f)
        .Build();

    // events
    EventManager* em = EventManager::Get();
    mChangePredatorAnimListenerId = em->AddListener(
        ChangePredatorAnimEvent::StaticGetTypeId(),
        std::bind(&GameState::OnChangePredatorAnimEvent, this, std::placeholders::_1)
    );
    mChangeSoldierAnimListenerId = em->AddListener(
        ChangeSoldierAnimEvent::StaticGetTypeId(),
        std::bind(&GameState::OnChangeSoldierAnimEvent, this, std::placeholders::_1)
    );
    mInfraredListenerId = em->AddListener(
		ChangeInfraredEvent::StaticGetTypeId(),
		std::bind(&GameState::OnInfraredEvent, this, std::placeholders::_1)
	);
    mSpawnParticleListenerId = em->AddListener(
        SpawnParticleEvent::StaticGetTypeId(),
        std::bind(&GameState::OnSpawnParticleEvent, this, std::placeholders::_1)
    );
    mPlaySoundListenerId = em->AddListener(
		PlaySoundEvent::StaticGetTypeId(),
		std::bind(&GameState::OnPlaySoundEvent, this, std::placeholders::_1)
	);
	mStopSoundListenerId = em->AddListener(
		StopSoundEvent::StaticGetTypeId(),
		std::bind(&GameState::OnStopSoundEvent, this, std::placeholders::_1)
	);

    // schedule events
    TimeEventManager* tem = TimeEventManager::Get();

    // predator
    tem->ScheduleEvent(0.0f, new ChangePredatorAnimEvent(1, true));
    tem->ScheduleEvent(15.01f, new ChangePredatorAnimEvent(5, true));
    tem->ScheduleEvent(16.5f, new ChangePredatorAnimEvent(3, true));
    tem->ScheduleEvent(18.16f, new ChangePredatorAnimEvent(5, true));
    tem->ScheduleEvent(19.5f, new ChangePredatorAnimEvent(0, false));
    tem->ScheduleEvent(22.5f, new ChangePredatorAnimEvent(3, false));
    tem->ScheduleEvent(25.0f, new ChangePredatorAnimEvent(4, false));
    tem->ScheduleEvent(35.5f, new ChangePredatorAnimEvent(2, false));

    // soldier 1 (clip index, soldier chosen)
    tem->ScheduleEvent(0.0f, new ChangeSoldierAnimEvent(4, 1, true));
    tem->ScheduleEvent(7.0f, new ChangeSoldierAnimEvent(2, 1, true));
    tem->ScheduleEvent(10.0f, new ChangeSoldierAnimEvent(4, 1, true));
    tem->ScheduleEvent(17.0f, new ChangeSoldierAnimEvent(2, 1, true));
    tem->ScheduleEvent(20.5f, new ChangeSoldierAnimEvent(0, 1, false));

    // soldier 2
    
    tem->ScheduleEvent(0.0f, new ChangeSoldierAnimEvent(2, 2, true));
    tem->ScheduleEvent(24.2f, new ChangeSoldierAnimEvent(0, 2, false));

    // soldier 3
    
    tem->ScheduleEvent(25.0f, new ChangeSoldierAnimEvent(3, 3, true));
    tem->ScheduleEvent(30.0f, new ChangeSoldierAnimEvent(0, 3, false));
    
    // infrared
    
    tem->ScheduleEvent(12.01f, new ChangeInfraredEvent());
    tem->ScheduleEvent(15.0f, new ChangeInfraredEvent());

    // particle schedule

    tem->ScheduleEvent(20.5f, new SpawnParticleEvent(mSoldier1TransformAnimation.GetTransform(20.5f).position, 0));
    tem->ScheduleEvent(24.2f, new SpawnParticleEvent(mSoldier2TransformAnimation.GetTransform(24.2f).position, 0));
    tem->ScheduleEvent(30.0f, new SpawnParticleEvent(mSoldier3TransformAnimation.GetTransform(30.0f).position, 0));

    tem->ScheduleEvent(43.0f, new SpawnParticleEvent({ 112.194f, 2.833f, 67.081f }, 1));

    // audio schedule

    tem->ScheduleEvent(12.0f, new PlaySoundEvent(10)); // thermal
    tem->ScheduleEvent(18.0f, new PlaySoundEvent(4)); // fall
    tem->ScheduleEvent(20.4f, new PlaySoundEvent(2)); // bloody slash
    tem->ScheduleEvent(20.5f, new PlaySoundEvent(3)); // death
    tem->ScheduleEvent(24.2f, new PlaySoundEvent(5)); // fall death
    tem->ScheduleEvent(25.2f, new PlaySoundEvent(8)); // missile lock on
    tem->ScheduleEvent(29.2f, new PlaySoundEvent(6)); // missile at soldier
    tem->ScheduleEvent(30.0f, new PlaySoundEvent(9)); // small explosion
    tem->ScheduleEvent(30.0f, new PlaySoundEvent(3)); // death
    tem->ScheduleEvent(32.0f, new PlaySoundEvent(11)); // trucks
    tem->ScheduleEvent(34.5f, new PlaySoundEvent(12)); // typing
    tem->ScheduleEvent(37.5f, new PlaySoundEvent(7)); // missile at trucks
    tem->ScheduleEvent(42.0f, new PlaySoundEvent(1)); // big explosion

    // audio
    SoundEffectManager* sm = SoundEffectManager::Get();
    mAmbienceSoundId = sm->Load("Ambience.wav");
    mBigExplosionSoundId = sm->Load("BigExplosion.wav");
    mBloodySlashSoundId = sm->Load("BloodySlash.wav");
    mDeathSoundId = sm->Load("Death.wav");
    mFallSoundId = sm->Load("Fall.wav");
    mFallDeathSoundId = sm->Load("FallDeath.wav");
    mMissileAtSoldierSoundId = sm->Load("MissileAtSoldier.wav");
    mMissileAtTrucksSoundId = sm->Load("MissileAtTrucks.wav");
    mMissileLockOnSoundId = sm->Load("MissileLockOn.wav");
    mSmallExplosionSoundId = sm->Load("SmallExplosion.wav");
    mThermalSoundId = sm->Load("Thermal.wav");
    mTruckSoundId = sm->Load("Trucks.wav");
    mTypingSoundId = sm->Load("Typing.wav");

    // play initial sound
    sm->Play(mAmbienceSoundId);

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

    MeshPX screenQuad = MeshBuilder::CreateScreenQuadPX();
    mScreenQuad.meshBuffer.Initialize(screenQuad);

    // particles

    mParticleSystemEffect.Initialize();
    mParticleSystemEffect.SetCamera(mCamera);

    ParticleSystemInfo blood;
    blood.textureId = TextureManager::Get()->LoadTexture("Images/explosion.png");
    blood.maxParticles = 1000;
    blood.particlesPerEmit = { 25, 35 };
    blood.delay = 1.0f;
    blood.lifeTime = 5.0f;
    blood.timeBetweenEmit = { 999.0f, 999.0f };
    blood.spawnAngle = { 0, 360.0f };
    blood.spawnSpeed = { 1.0f, 3.0f };
    blood.particleLifeTime = { 0.5f, 2.0f };
    blood.spawnDirection = Math::Vector3::YAxis;
    blood.spawnPosition = Math::Vector3::Zero;
    blood.startScale = { { 0.25f, 0.25f, 0.25f }, { 0.5f, 0.5f, 0.5f }};
    blood.endScale = { Math::Vector3::One, Math::Vector3::One };
    blood.startColor = { Graphics::Colors::DarkRed, Graphics::Colors::DarkRed };
    blood.endColor = { Graphics::Colors::DarkRed, Graphics::Colors::DarkRed };
    mParticleSystem.Initialize(blood);

    ParticleSystemInfo explosion;
    explosion.textureId = TextureManager::Get()->LoadTexture("Images/explosion.png");
    explosion.maxParticles = 2000;
    explosion.particlesPerEmit = { 1, 2 };
    explosion.delay = 0.0f;
    explosion.lifeTime = 5.0f;
    explosion.timeBetweenEmit = { 0.5f, 1.0f };
    explosion.spawnAngle = { -180.0f, 0.0f };
    explosion.spawnSpeed = { 0.0f, 0.0f };
    explosion.particleLifeTime = { 2.0f, 4.0f };
    explosion.spawnDirection = Math::Vector3::YAxis;
    explosion.spawnPosition = Math::Vector3::Zero;
    explosion.startScale = { { 10.0f, 10.0f, 10.0f }, { 20.0f, 20.0f, 20.0f } };
    explosion.endScale = { { 100.0f, 100.0f, 100.0f }, { 200.0f, 200.0f, 200.0f } };
    explosion.startColor = { { Graphics::Colors::Red }, { Graphics::Colors::OrangeRed } };
    explosion.endColor = { { Graphics::Colors::Orange }, { Graphics::Colors::LightYellow } };
	mExplosionSystem.Initialize(explosion); 
}
void GameState::Terminate()
{
	mExplosionSystem.Terminate();
    mParticleSystem.Terminate();
    mParticleSystemEffect.Terminate();
    mScreenQuad.meshBuffer.Terminate();
	mCombineTexture.Terminate();
	mRenderTarget.Terminate();
	mInfrared.Terminate();
    EventManager* em = EventManager::Get();
    em->RemoveListener(
        PlaySoundEvent::StaticGetTypeId(),
		mPlaySoundListenerId
	);
	em->RemoveListener(
		StopSoundEvent::StaticGetTypeId(),
		mStopSoundListenerId
	);
	em->RemoveListener(
		ChangeInfraredEvent::StaticGetTypeId(),
		mInfraredListenerId
	);
    em->RemoveListener(
        ChangeSoldierAnimEvent::StaticGetTypeId(),
        mChangeSoldierAnimListenerId
    );
    em->RemoveListener(
        ChangePredatorAnimEvent::StaticGetTypeId(),
        mChangePredatorAnimListenerId
    );
    mMilitaryBase.Terminate();
    mTerrainEffect.Terminate();
    mShadowEffect.Terminate();
    mGround.Terminate();
    mStandardEffect.Terminate();
    mBullet.Terminate();
    mMilitaryTruck3.Terminate();
    mMilitaryTruck2.Terminate();
    mMilitaryTruck1.Terminate();
	mSoldier3.Terminate();
	mSoldier2.Terminate();
	mSoldier1.Terminate();
	mPredator.Terminate();
    mSkySphere.Terminate();
}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

    // timer
	mTimer += deltaTime;
    TimeEventManager::Get()->Update(deltaTime);

    // anim
    mAnimatorPredator.Update(deltaTime * mAnimationSpeedPredator);
	mAnimatorSoldier1.Update(deltaTime * mAnimationSpeedSoldier1);
	mAnimatorSoldier2.Update(deltaTime * mAnimationSpeedSoldier2);
	mAnimatorSoldier3.Update(deltaTime * mAnimationSpeedSoldier3);

    // input
    InputSystem* input = InputSystem::Get();

    // particle system
    mParticleSystem.Update(deltaTime);
	mExplosionSystem.Update(deltaTime);
}
void GameState::Render()
{
    if (mCameraDebug)
    {
       mPredator.transform = mPredatorTransformAnimation.GetTransform(mTimer);
       mCamera.SetPosition(mCameraTransformAnimation.GetTransform(mTimer).position);
       mCamera.SetDirection({ mCameraTransformAnimation.GetTransform(mTimer).rotation.x, mCameraTransformAnimation.GetTransform(mTimer).rotation.y, mCameraTransformAnimation.GetTransform(mTimer).rotation.z });
       mSoldier1.transform = mSoldier1TransformAnimation.GetTransform(mTimer);
       mSoldier2.transform = mSoldier2TransformAnimation.GetTransform(mTimer);
       mSoldier3.transform = mSoldier3TransformAnimation.GetTransform(mTimer);
       mMilitaryTruck1.transform = mMilitaryTruck1TransformAnimation.GetTransform(mTimer);
       mMilitaryTruck2.transform = mMilitaryTruck2TransformAnimation.GetTransform(mTimer);
       mMilitaryTruck3.transform = mMilitaryTruck3TransformAnimation.GetTransform(mTimer);
       mBullet.transform = mBulletTransformAnimation.GetTransform(mTimer);
    }
    
    SimpleDraw::Render(mCamera);

    mRenderTarget.BeginRender();
        mShadowEffect.Begin();
            mShadowEffect.Render(mPredator);
            mShadowEffect.Render(mSoldier1);
            mShadowEffect.Render(mSoldier2);
            mShadowEffect.Render(mSoldier3);
            mShadowEffect.Render(mMilitaryTruck1);
            mShadowEffect.Render(mMilitaryTruck2);
            mShadowEffect.Render(mMilitaryTruck3);
            mShadowEffect.Render(mMilitaryBase);
        mShadowEffect.End();

        mTerrainEffect.Begin();
            mTerrainEffect.Render(mGround);
        mTerrainEffect.End();

        mParticleSystemEffect.Begin();
            mParticleSystem.Render(mParticleSystemEffect);
			mExplosionSystem.Render(mParticleSystemEffect);
        mParticleSystemEffect.End();

        mStandardEffect.Begin();
	    	mStandardEffect.Render(mPredator);
	    	mStandardEffect.Render(mSoldier1);
	    	mStandardEffect.Render(mSoldier2);
	    	mStandardEffect.Render(mSoldier3);
	    	mStandardEffect.Render(mMilitaryTruck1);
	    	mStandardEffect.Render(mMilitaryTruck2);
	    	mStandardEffect.Render(mMilitaryTruck3);
	    	mStandardEffect.Render(mBullet);
            mStandardEffect.Render(mSkySphere);
            mStandardEffect.Render(mMilitaryBase);
        mStandardEffect.End();
	mRenderTarget.EndRender();

    mInfrared.Begin(mTimer);
        mInfrared.Render(mScreenQuad);
    mInfrared.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    // infrared
    mInfrared.DebugUI();

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

    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
    {
        Material& material01 = mMilitaryTruck1.renderObjects[0].material;
        ImGui::ColorEdit4("Emissive01#Material", &material01.emissive.r);
        ImGui::ColorEdit4("Ambient01#Material", &material01.ambient.r);
        ImGui::ColorEdit4("Diffuse01#Material", &material01.diffuse.r);
        ImGui::ColorEdit4("Specular01#Material", &material01.specular.r);
        ImGui::DragFloat("Shininess01#Material", &material01.shininess, 0.1f, 0.1f, 1000.f);
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
    ImGui::DragFloat3("PredatorPosition", &mPredator.transform.position.x, 0.01f);
    ImGui::DragFloat3("PredatorRotation", &mPredator.transform.rotation.x, 0.01f);
	ImGui::Separator();
	ImGui::DragFloat3("Soldier1Position", &mSoldier1.transform.position.x, 0.01f);
	ImGui::DragFloat3("Soldier1Rotation", &mSoldier1.transform.rotation.x, 0.01f);
    ImGui::Separator();
	ImGui::DragFloat3("Soldier2Position", &mSoldier2.transform.position.x, 0.01f);
	ImGui::DragFloat3("Soldier2Rotation", &mSoldier2.transform.rotation.x, 0.01f);
    ImGui::Separator();
	ImGui::DragFloat3("Soldier3Position", &mSoldier3.transform.position.x, 0.01f);
	ImGui::DragFloat3("Soldier3Rotation", &mSoldier3.transform.rotation.x, 0.01f);
    ImGui::Separator();
    ImGui::DragFloat3("Truck1Position", &mMilitaryTruck1.transform.position.x, 0.01f);
    ImGui::DragFloat3("Truck1Rotation", &mMilitaryTruck1.transform.rotation.x, 0.01f);
    ImGui::DragFloat3("Truck1Scale", &mMilitaryTruck1.transform.scale.x, 0.01f);
    ImGui::Separator();
    ImGui::DragFloat3("Truck2Position", &mMilitaryTruck2.transform.position.x, 0.01f);
    ImGui::DragFloat3("Truck2Rotation", &mMilitaryTruck2.transform.rotation.x, 0.01f);
    ImGui::DragFloat3("Truck2Scale", &mMilitaryTruck2.transform.scale.x, 0.01f);
    ImGui::Separator();
    ImGui::DragFloat3("Truck3Position", &mMilitaryTruck3.transform.position.x, 0.01f);
    ImGui::DragFloat3("Truck3Rotation", &mMilitaryTruck3.transform.rotation.x, 0.01f);
    ImGui::DragFloat3("Truck3Scale", &mMilitaryTruck3.transform.scale.x, 0.01f);
    ImGui::Separator();
    ImGui::DragFloat3("BulletPosition", &mBullet.transform.position.x, 0.01f);
    ImGui::DragFloat3("BulletScale", &mBullet.transform.scale.x, 0.01f);
    ImGui::Separator();

    // PROPS
    ImGui::DragFloat3("MilitaryBasePosition", &mMilitaryBase.transform.position.x, 0.01f);
    ImGui::DragFloat3("MilitaryBaseRotation", &mMilitaryBase.transform.rotation.x, 0.01f);

    // DISPLAY TIMER (up to 2 decimals)
	ImGui::Text("Time: %.2f", mTimer);

    // CAMERA DEBUG
    ImGui::Checkbox("Camera Debug", &mCameraDebug);

    ImGui::End();
	SimpleDraw::Render(mCamera);
}

void GameState::UpdateCamera(float deltaTime)
{
    Input::InputSystem* input = Input::InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 50.0f : 1.0f;
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

void GameState::OnChangePredatorAnimEvent(const Event& e)
{
    const ChangePredatorAnimEvent& animEvent = static_cast<const ChangePredatorAnimEvent&>(e);
    mAnimatorPredator.PlayAnimation(animEvent.anim, animEvent.loop);
}

void GameState::OnChangeSoldierAnimEvent(const Event& e)
{
    const ChangeSoldierAnimEvent& animEvent = static_cast<const ChangeSoldierAnimEvent&>(e);

    if (animEvent.sold == 1)
    {
		mAnimatorSoldier1.PlayAnimation(animEvent.anim, animEvent.loop);
    }
	else if (animEvent.sold == 2)
	{
		mAnimatorSoldier2.PlayAnimation(animEvent.anim, animEvent.loop);
	}
	else if (animEvent.sold == 3)
	{
		mAnimatorSoldier3.PlayAnimation(animEvent.anim, animEvent.loop);
	}
}

void GameState::OnInfraredEvent(const ML_Engine::Core::Event& e)
{
    if (mInfrared.GetMode() == Infrared::Mode::Infrared)
    {
        mInfrared.SetMode(Infrared::Mode::None);

        for (uint32_t i = 0; i < mSoldier1.renderObjects.size(); ++i)
        {
            mSoldier1.renderObjects[i].material.emissive = { 0.0f, 0.0f, 0.0f, 0.5f };
            mSoldier2.renderObjects[i].material.emissive = { 0.0f, 0.0f, 0.0f, 0.5f };
            mSoldier3.renderObjects[i].material.emissive = { 0.0f, 0.0f, 0.0f, 0.5f };
        }
    }
	else if (mInfrared.GetMode() == Infrared::Mode::None)
	{
		mInfrared.SetMode(Infrared::Mode::Infrared);

        for (uint32_t i = 0; i < mSoldier1.renderObjects.size(); ++i)
        {
            mSoldier1.renderObjects[i].material.emissive = { 1.0f, 1.0f, 1.0f, 255.0f };
            mSoldier2.renderObjects[i].material.emissive = { 1.0f, 1.0f, 1.0f, 255.0f };
            mSoldier3.renderObjects[i].material.emissive = { 1.0f, 1.0f, 1.0f, 255.0f };
        }
	}
}

void GameState::OnSpawnParticleEvent(const ML_Engine::Core::Event& e)
{
    const SpawnParticleEvent& particleEvent = static_cast<const SpawnParticleEvent&>(e);

    // blood
    if (particleEvent.type == 0)
    {
        Math::Vector3 spawnPos = particleEvent.pos;
        spawnPos.y += 2.5f; // move up to chest

        mParticleSystem.SetPosition(spawnPos);
        mParticleSystem.SpawnParticles();
    }
    else if (particleEvent.type == 1)
    {
        PhysicsWorld::Get()->SetGravity({ 0.0f, 0.0f, 0.0f });
        Math::Vector3 spawnPos = particleEvent.pos;

        mExplosionSystem.SetPosition(spawnPos);
        mExplosionSystem.SpawnParticles();
    }
}

void GameState::OnPlaySoundEvent(const ML_Engine::Core::Event& e)
{
    const PlaySoundEvent& soundEvent = static_cast<const PlaySoundEvent&>(e);
    SoundEffectManager* sm = SoundEffectManager::Get();

    if(soundEvent.index == 0)
	{
        sm->Play(mAmbienceSoundId);
	}
	else if (soundEvent.index == 1)
	{
		sm->Play(mBigExplosionSoundId);
	}
	else if (soundEvent.index == 2)
	{
		sm->Play(mBloodySlashSoundId);
	}
	else if (soundEvent.index == 3)
	{
		sm->Play(mDeathSoundId);
	}
	else if (soundEvent.index == 4)
	{
		sm->Play(mFallSoundId);
	}
	else if (soundEvent.index == 5)
	{
		sm->Play(mFallDeathSoundId);
	}
	else if (soundEvent.index == 6)
	{
		sm->Play(mMissileAtSoldierSoundId);
	}
	else if (soundEvent.index == 7)
	{
		sm->Play(mMissileAtTrucksSoundId);
	}
	else if (soundEvent.index == 8)
	{
		sm->Play(mMissileLockOnSoundId);
	}
	else if (soundEvent.index == 9)
	{
		sm->Play(mSmallExplosionSoundId);
	}
	else if (soundEvent.index == 10)
	{
		sm->Play(mThermalSoundId);
	}
    else if (soundEvent.index == 11)
	{
		sm->Play(mTruckSoundId);
	}
    else if (soundEvent.index == 12)
    {
		sm->Play(mTypingSoundId);
    }
}

void GameState::OnStopSoundEvent(const ML_Engine::Core::Event& e)
{
    const PlaySoundEvent& soundEvent = static_cast<const PlaySoundEvent&>(e);
    SoundEffectManager* sm = SoundEffectManager::Get();

	if (soundEvent.index == 0)
	{
		sm->Stop(mAmbienceSoundId);
	}
	else if (soundEvent.index == 1)
	{
		sm->Stop(mBigExplosionSoundId);
	}
	else if (soundEvent.index == 2)
	{
		sm->Stop(mBloodySlashSoundId);
	}
	else if (soundEvent.index == 3)
	{
		sm->Stop(mDeathSoundId);
	}
	else if (soundEvent.index == 4)
	{
		sm->Stop(mFallSoundId);
	}
	else if (soundEvent.index == 5)
	{
		sm->Stop(mFallDeathSoundId);
	}
	else if (soundEvent.index == 6)
	{
		sm->Stop(mMissileAtSoldierSoundId);
	}
	else if (soundEvent.index == 7)
	{
		sm->Stop(mMissileAtTrucksSoundId);
	}
	else if (soundEvent.index == 8)
	{
		sm->Stop(mMissileLockOnSoundId);
	}
	else if (soundEvent.index == 9)
	{
		sm->Stop(mSmallExplosionSoundId);
	}
	else if (soundEvent.index == 10)
	{
		sm->Stop(mThermalSoundId);
	}
	else if (soundEvent.index == 11)
	{
		sm->Stop(mTruckSoundId);
	}
    else if (soundEvent.index == 12)
	{
		sm->Stop(mTypingSoundId);
	}
}