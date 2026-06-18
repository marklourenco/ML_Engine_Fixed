#include "Precompiled.h"
#include "WallComponent.h"
#include "WallService.h"
#include "GameObject.h"
#include "GameWorld.h"

using namespace ML_Engine;

void WallComponent::Initialize()
{
	WallService* wallService = GetOwner().GetWorld().GetService<WallService>();
	if (wallService != nullptr)
	{
		wallService->Register(this);
	}
}

void WallComponent::Terminate()
{
	WallService* wallService = GetOwner().GetWorld().GetService<WallService>();
	if (wallService != nullptr)
	{
		wallService->Unregister(this);
	}
}

void WallComponent::Deserialize(const rapidjson::Value& value)
{
	// no data, component is just a tag
}

void WallComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& originalValue)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	value.AddMember("WallComponent", componentValue, doc.GetAllocator());
}
