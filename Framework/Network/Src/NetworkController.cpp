#include "Precompiled.h"
#include "NetworkController.h"

using namespace ML_Engine;
using namespace ML_Engine::Network;

void NetworkController::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
	mNewPosition = true;
}

bool NetworkController::TryGetNewPosition(Math::Vector3& newPosition)
{
	if (mNewPosition)
	{
		newPosition = mPosition;
		mNewPosition = false;
		return true;
	}
	return false;
}

void NetworkController::SetInput(const EventInput& input)
{
	mInputIsDirty = mInputIsDirty || input.moveX != mInput.moveX;
	mInputIsDirty = mInputIsDirty || input.moveY != mInput.moveY;
	mInputIsDirty = mInputIsDirty || input.jump != mInput.jump;
	mInputIsDirty = mInputIsDirty || input.shiftSpeed != mInput.shiftSpeed;
	mInput = input;
}

bool NetworkController::TryGetDirtyInput(EventInput& input)
{
	if (mInputIsDirty)
	{
		input = mInput;
		mInputIsDirty = false;
		return true;
	}
	return false;
}

const EventInput& NetworkController::GetInput() const
{
	return mInput;
}

void NetworkController::SetLocalTransform(const Graphics::Transform* transform)
{
	mLocalTransform = transform;
}

const Graphics::Transform* NetworkController::GetLocalTransform() const
{
	return mLocalTransform;
}

void NetworkController::SetRigidBody(const Physics::RigidBody* rigidBody)
{
	mRigidBody = rigidBody;
}

const Physics::RigidBody* NetworkController::GetRigidBody() const
{
	return mRigidBody;
}

void NetworkController::SetTargetTransform(const Graphics::Transform& targetTransform, float timeStamp)
{
	if (timeStamp > mLastTimeStamp)
	{
		mTargetSimTransform = targetTransform;
		mLastTimeStamp = timeStamp;
		mCurrentTime = 0.0f;
	}
}

void NetworkController::Update(float deltaTime)
{
	if (mLastTimeStamp > 0.0f)
	{
		mCurrentTime += deltaTime;
		float updateRate = POSITION_UPDATE_RATE;
		mSimTransform.position = Math::Lerp(mLocalTransform->position, mTargetSimTransform.position, mCurrentTime / updateRate);
		SetPosition(mSimTransform.position);
	}
}

const Graphics::Transform& NetworkController::GetSimTransform() const
{
	return mSimTransform;
}