#include "Precompiled.h"
#include "Animation.h"
#include "Animation.h"
#include "Animator.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;

void Animator::Initialize(ModelId id)
{
	// store model id
	mModelId = id;
	mIsLooping = false;
	mAnimationTick = 0.0f;
	// -1 means not animating
	mClipIndex = -1;
}

// reset anim, play new one
void Animator::PlayAnimation(int clipIndex, bool looping)
{
	mClipIndex = clipIndex;
	mIsLooping = looping;
	mAnimationTick = 0.0f;
}

void Animator::Update(float deltaTime)
{
	// not playing so dont update
	if (mClipIndex < 0)
	{
		return;
	}

	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	mAnimationTick += animClip.ticksPerSecond * deltaTime;
	if (mIsLooping)
	{
		// loop the timer
		while (mAnimationTick > animClip.tickDuration)
		{
			mAnimationTick -= animClip.tickDuration;
		}
	}
	else
	{
		// if not looping, clamp the end of duration
		mAnimationTick = Math::Min(mAnimationTick, animClip.tickDuration);
	}
}

bool Animator::IsFinished() const
{
	if (mClipIndex < 0 || mIsLooping)
	{
		return false;
	}

	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	return mAnimationTick >= animClip.tickDuration;
}

size_t Animator::GetAnimationCount() const
{
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	return model->animationClips.size();
}

bool Animator::GetToParentTransform(const Bone* bone, Math::Matrix4& transform) const
{
	if (mClipIndex < 0)
	{
		return false;
	}

	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	const Animation* animation = animClip.boneAnimations[bone->index].get();
	if (animation == nullptr)
	{
		return false;
	}
	Transform animTransform = animation->GetTransform(mAnimationTick);
	transform = animTransform.GetMatrix4();
	return true;
}