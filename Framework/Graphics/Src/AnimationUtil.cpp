#include "Precompiled.h"
#include "AnimationUtil.h"

#include "Color.h"
#include "SimpleDraw.h"

using namespace ML_Engine;
using namespace ML_Engine::Graphics;

// empty namespace for global functions isolated to the cpp file
namespace
{
	void ComputeBoneTransformsRecursive(const Bone* bone, AnimationUtil::BoneTransforms& boneTransforms)
	{
		if (bone != nullptr)
		{
			boneTransforms[bone->index] = bone->toParentTransform;
			// if there is a parent, apply the parent's transforms as well
			if (bone->parent != nullptr)
			{
				boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
			}
			for (const Bone* child : bone->children)
			{
				ComputeBoneTransformsRecursive(child, boneTransforms);
			}
		}
	}
}

void AnimationUtil::ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(modelId);
	if (model != nullptr && model->skeleton != nullptr)
	{
		// resize to sync the number of bones with the matrices
		boneTransforms.resize(model->skeleton->bones.size());
		// generate the matrices
		ComputeBoneTransformsRecursive(model->skeleton->root, boneTransforms);
	}
}

void AnimationUtil::DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(modelId);
	if (model != nullptr && model->skeleton != nullptr)
	{
		for (const auto& bone : model->skeleton->bones)
		{
			if (bone->parent != nullptr)
			{
				const Math::Vector3 bonePos = Math::GetTranslation(boneTransforms[bone->index]);
				const Math::Vector3 parentPos = Math::GetTranslation(boneTransforms[bone->parentIndex]);
				SimpleDraw::AddLine(bonePos, parentPos, Colors::HotPink);
				SimpleDraw::AddSphere(10, 10, 0.03f, Colors::Purple, bonePos);
			}
		}
	}
}

void AnimationUtil::ApplyBoneOffset(ModelId modelId, BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(modelId);
	if (model != nullptr && model->skeleton != nullptr)
	{
		for (auto& bone : model->skeleton->bones)
		{
			boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
		}
	}
}
