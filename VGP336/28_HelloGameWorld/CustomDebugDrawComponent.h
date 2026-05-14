#pragma once

#include "CustomTypeIds.h"
#include <Inc/ML_Engine.h>

class CustomDebugDrawComponent : public ML_Engine::Component
{
public:
	SET_TYPE_ID(CustomTypeId::CustomDebugDraw);

	void Initialize() override;
	void Terminate() override;
	void DebugUI() override;
	void Deserialize(const rapidjson::Value& value) override;

	void AddDebugDraw() const;
private:
	const ML_Engine::TransformComponent* mTransformComponent = nullptr;
	ML_Engine::Math::Vector3 mPosition = ML_Engine::Math::Vector3::Zero;
	ML_Engine::Graphics::Color mColor = ML_Engine::Graphics::Colors::White;

	uint32_t mSlices = 0;
	uint32_t mRings = 0;
	float mRadius = 0;

};