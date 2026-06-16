#pragma once

#include "Component.h"

namespace ML_Engine
{
	class WallComponent : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Wall);
		
		void Initialize() override;
		void Terminate() override;
		void Deserialize(const rapidjson::Value& value) override;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& originalValue) override;
	};
}