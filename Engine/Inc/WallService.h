#pragma once

#include "Service.h"

namespace ML_Engine
{
	class WallComponent;

	struct WallHitResult
	{
		bool hit = false;
		float distance = 0.0f;
		Math::Vector3 wallPosition = Math::Vector3::Zero;
		Math::Vector3 wallNormal = Math::Vector3::Zero;
	};

	class WallService : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Wall);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void Deserialize(const rapidjson::Value& data) override;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Register(WallComponent* wall);
		void Unregister(WallComponent* wall);

		WallHitResult FindNearestWall(const Math::Vector3& position, float radius) const;

	private:
		std::vector<WallComponent*> mWalls;
	};
}