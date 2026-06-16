#include "Precompiled.h"
#include "RenderObjectComponent.h"
#include "SaveUtil.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "RenderService.h"

using namespace ML_Engine;

void RenderObjectComponent::Initialize()
{
	RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
	if (renderService != nullptr)
	{
		renderService->Register(this);
	}
}
void RenderObjectComponent::Terminate()
{
	RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
	if (renderService != nullptr)
	{
		renderService->Unregister(this);
	}
}
void RenderObjectComponent::Deserialize(const rapidjson::Value& value)
{
	SaveUtil::ReadBool("CastShadow", mCastShadow, value);
}
void RenderObjectComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& originalValue)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	// compare with original, if different, save current value
	if (originalValue.HasMember("CastShadow") && mCastShadow != originalValue["CastShadow"].GetBool())
	{
		SaveUtil::WriteBool("CastShadow", mCastShadow, doc, componentValue);
	}
	value.AddMember("PlayerControllerComponent", componentValue, doc.GetAllocator());
}
bool RenderObjectComponent::CanCastShadow() const
{
	return mCastShadow;
}
