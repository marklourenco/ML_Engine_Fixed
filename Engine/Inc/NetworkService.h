#pragma once

#include "Service.h"

namespace ML_Engine
{
	class NetworkService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Network);

		void Update(float deltaTime) override;
		void DebugUI() override;
	};
}