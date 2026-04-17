#pragma once


namespace ML_Engine
{
	enum class ComponentId
	{
		Invalid,				// default value, not valid vid
		Transform,              // transform component for location data
		Camera,					// contains the camera for viewing
		FPSCamera,				// move the Camera Component with FPS constrols
		Count				    // last value, can be used to chain custom components
	};
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
    uint32_t GetTypeId() const override { return StaticGetTypeId(); }