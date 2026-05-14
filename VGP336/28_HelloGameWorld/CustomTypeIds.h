#pragma once
#include <Inc/TypeIds.h>

enum class CustomTypeId
{
	CustomDebugDraw = static_cast<int>(ML_Engine::ComponentId::Count)
};

enum class CustomServiceId
{
	CustomDebugDrawDisplay = static_cast<int>(ML_Engine::ServiceId::Count)
};