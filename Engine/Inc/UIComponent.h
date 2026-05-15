#pragma once

#include "Component.h"

namespace ML_Engine
{
	class UIComponent : public Component
	{
	public:
		virtual void Render() = 0;
	};
}