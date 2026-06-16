#include "Precompiled.h"
#include "WallService.h"
#include "WallComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

using namespace ML_Engine;

void WallService::Initialize()
{
}

void WallService::Terminate()
{
}

void WallService::Update(float deltaTime)
{
}

void WallService::Deserialize(const rapidjson::Value& data)
{
}

void WallService::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
}

void WallService::Register(WallComponent* wall)
{
	mWalls.push_back(wall);
}

void WallService::Unregister(WallComponent* wall)
{
	auto it = std::find(mWalls.begin(), mWalls.end(), wall);
	if (it != mWalls.end())
	{
		mWalls.erase(it);
	}
}

WallHitResult WallService::FindNearestWall(const Math::Vector3& position, float radius) const
{
	WallHitResult result;
	float nearest = radius;

	for (WallComponent* wall : mWalls)
	{
		TransformComponent* tc = wall->GetOwner().GetComponent<TransformComponent>();
		if (tc == nullptr)
		{
			continue;
		}

		Math::Vector3 diff = position - tc->position;
		Math::Vector3 horizontal = { diff.x, 0.0f, diff.z };
		float dist = Math::Magnitude(horizontal);

		if (dist < nearest && dist > 0.001f)
		{
			nearest = dist;
			result.hit = true;
			result.distance = dist;
			result.wallPosition = tc->position;

			Math::Vector3 n = Math::Normalize(horizontal);
			if (std::abs(n.x >= std::abs(n.z)))
			{
				result.wallNormal = { (n.x >= 0.0f ? 1.0f : -1.0f), 0.0f, 0.0f };
			}
			else
			{
				result.wallNormal = { 0.0f, 0.0f, (n.z >= 0.0f ? 1.0f : -1.0f) };
			}
		}
	}

	return result;
}
