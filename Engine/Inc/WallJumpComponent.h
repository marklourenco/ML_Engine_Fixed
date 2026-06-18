#pragma once

#include "Component.h"

namespace ML_Engine
{
	class TransformComponent;
	class RigidBodyComponent;

	class WallJumpComponent : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::WallJump);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;
		void Deserialize(const rapidjson::Value& value) override;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& originalValue) override;

	private:
		void EnterGrab(const Math::Vector3& wallNormal);
		void ExitGrab();
		void PerformWallJump();
		bool IsGrounded() const;

		TransformComponent* mTransformComponent = nullptr;
		RigidBodyComponent* mRigidBodyComponent = nullptr;

		bool mIsGrabbing = false;
		bool mCanGrab = true;
		float mGrabTimer = 0.0f;
		float mAirTime = 0.0f;
		float mPostJumpHoldOff = 0.0f;
		Math::Vector3 mWallNormal = Math::Vector3::Zero;
		Math::Vector3 mLaunchVelocity = Math::Vector3::Zero;
		float mLaunchTimer = 0.0f;

		float mDetectionRadius = 0.5f;
		float mGrabWindow = 0.8f;
		float mWallSlideSpeed = -0.5f;
		float mJumpHorizontal = 8.0f;
		float mJumpVertical = 10.0f;
		float mAirTimeMin = 0.15f;
		float mGroundThreshold = 0.2f;
		float mPostJumpHoldOffDur = 0.25f;
		float mLaunchDuration = 0.35f;
	};
}