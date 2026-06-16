#include "Precompiled.h"
#include "WallJumpComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "WallService.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "SaveUtil.h"

using namespace ML_Engine;
using namespace ML_Engine::Input;

void WallJumpComponent::Initialize()
{
	mRigidBodyComponent = GetOwner().GetComponent<RigidBodyComponent>();
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	ASSERT(mRigidBodyComponent != nullptr, "WallJumpComponent: owner needs a RigidBodyComponent");
	ASSERT(mTransformComponent != nullptr, "WallJumpComponent: owner needs a TransformComponent");
}

void WallJumpComponent::Terminate()
{
	mRigidBodyComponent = nullptr;
	mTransformComponent = nullptr;
}

void WallJumpComponent::Update(float deltaTime)
{
	auto* input = InputSystem::Get();

	if (mPostJumpHoldOff > 0.0f)
	{
		mPostJumpHoldOff -= deltaTime;
	}

	if (mLaunchTimer > 0.0f)
	{
		mLaunchTimer -= deltaTime;
		Math::Vector3 vel = mRigidBodyComponent->GetVelocity();
		vel.x = mLaunchVelocity.x;
		vel.z = mLaunchVelocity.z;
		mRigidBodyComponent->SetVelocity(vel);
	}

	if (IsGrounded())
	{
		mAirTime = 0.0f;
		mCanGrab = true;
		mLaunchTimer = 0.0f;

		if (mIsGrabbing)
		{
			ExitGrab();
		}

		return;
	}

	mAirTime += deltaTime;

	if (mIsGrabbing)
	{
		mGrabTimer -= deltaTime;

		if (input->IsKeyDown(KeyCode::SPACE))
		{
			PerformWallJump();
			return;
		}

		if (mGrabTimer <= 0.0f)
		{
			ExitGrab();
			return;
		}

		WallService* ws = GetOwner().GetWorld().GetService<WallService>();
		if (ws != nullptr)
		{
			WallHitResult hit = ws->FindNearestWall(mTransformComponent->position, mDetectionRadius);
			if (!hit.hit)
			{
				ExitGrab();
				return;
			}
		}

		Math::Vector3 vel = mRigidBodyComponent->GetVelocity();
		vel.x = 0.0f;
		vel.y = mWallSlideSpeed;
		vel.z = 0.0f;
		mRigidBodyComponent->SetVelocity(vel);
		return;
	}

	if (!mCanGrab || mAirTime < mAirTimeMin || mPostJumpHoldOff > 0.0f)
	{
		return;
	}

	WallService* ws = GetOwner().GetWorld().GetService<WallService>();
	if (ws == nullptr)
	{
		return;
	}

	WallHitResult hit = ws->FindNearestWall(mTransformComponent->position, mDetectionRadius);
	if (hit.hit)
	{
		EnterGrab(hit.wallNormal);
	}
}

void WallJumpComponent::DebugUI()
{
	ImGui::Text("WallJump");
	ImGui::Text("Grabbing: %s", mIsGrabbing ? "YES" : "NO");
	ImGui::Text("CanGrab: %s", mCanGrab ? "YES" : "NO");
	ImGui::Text("GrabTimer: %.2f", mGrabTimer);
	ImGui::Text("AirTime: %.2f", mAirTime);
	ImGui::Text("PostJumpHoldOff: %.2f", mPostJumpHoldOff);
	ImGui::Text("LaunchTimer: %.2f", mLaunchTimer);
	ImGui::Separator();
	ImGui::DragFloat("DetectionRadius", &mDetectionRadius, 0.05f, 0.1f, 10.0f);
	ImGui::DragFloat("GrabWindow", &mGrabTimer, 0.05f, 0.1f, 10.0f);
	ImGui::DragFloat("WallSlideSpeed", &mWallSlideSpeed, 0.05f, -10.0f, 0.0f);
	ImGui::DragFloat("JumpHorizontal", &mJumpHorizontal, 0.5f, 0.0f, 50.0f);
	ImGui::DragFloat("JumpVertical", &mJumpVertical, 0.5f, 0.0f, 50.0f);
	ImGui::DragFloat("AirTimeMin", &mAirTimeMin, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("GroundThreshold", &mGroundThreshold, 0.01f, 0.01f, 2.0f);
	ImGui::DragFloat("PostJumpHoldOff", &mPostJumpHoldOff, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("LaunchDuration", &mLaunchDuration, 0.01f, 0.0f, 1.0f);
}

void WallJumpComponent::Deserialize(const rapidjson::Value& value)
{
	SaveUtil::ReadFloat("DetectionRadius", mDetectionRadius, value);
	SaveUtil::ReadFloat("GrabWindow", mGrabWindow, value);
	SaveUtil::ReadFloat("WallSlideSpeed", mWallSlideSpeed, value);
	SaveUtil::ReadFloat("JumpHorizontal", mJumpHorizontal, value);
	SaveUtil::ReadFloat("JumpVertical", mJumpVertical, value);
	SaveUtil::ReadFloat("AirTimeMin", mAirTimeMin, value);
	SaveUtil::ReadFloat("GroundThreshold", mGroundThreshold, value);
	SaveUtil::ReadFloat("PostJumpHoldOff", mPostJumpHoldOffDur, value);
	SaveUtil::ReadFloat("LaunchDuration", mLaunchDuration, value);
}

void WallJumpComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& originalValue)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	value.AddMember("WallJumpComponent", componentValue, doc.GetAllocator());
}

void WallJumpComponent::EnterGrab(const Math::Vector3& wallNormal)
{
	mIsGrabbing = true;
	mCanGrab = false;
	mGrabTimer = mGrabWindow;
	mWallNormal = wallNormal;

	Math::Vector3 vel = mRigidBodyComponent->GetVelocity();
	vel.x = 0.0f;
	vel.y = mWallSlideSpeed;
	vel.z = 0.0f;
	mRigidBodyComponent->SetVelocity(vel);
}

void WallJumpComponent::ExitGrab()
{
	mIsGrabbing = false;
	mWallNormal = Math::Vector3::Zero;
}

void WallJumpComponent::PerformWallJump()
{
	Math::Vector3 wallNormal = mWallNormal;
	ExitGrab();

	Math::Vector3 jumpVel = wallNormal * mJumpHorizontal;
	jumpVel.y = mJumpVertical;
	mRigidBodyComponent->SetVelocity(jumpVel);

	mLaunchVelocity = { jumpVel.x, 0.0f, jumpVel.z };
	mLaunchTimer = mLaunchDuration;
	mPostJumpHoldOff = mPostJumpHoldOffDur;
	mAirTime = mAirTimeMin;
}

bool WallJumpComponent::IsGrounded() const
{
	return std::abs(mRigidBodyComponent->GetVelocity().y) < mGroundThreshold;
}