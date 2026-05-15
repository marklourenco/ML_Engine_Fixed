#pragma once


namespace ML_Engine
{
	enum class ComponentId
	{
		Invalid,				// default value, not valid vid
		Transform,              // transform component for location data
		Camera,					// contains the camera for viewing
		FPSCamera,				// move the Camera Component with FPS constrols
		Mesh,                   // add a mesh  shape on the game object to render
		Model,                  // adds a model to the game object to render
		Animator,               // adds an animator controller to the game object
		RigidBody,              // adds a physics object to the game object
		SoundEffect,			// adds a sound effect to an object
		SoundBank,				// adds a group of sounds to an object
		UIText,                 // adds a UI text component
		UISprite,               // adds a UI sprite component
		UIButton,               // adds a UI button component
		Count				    // last value, can be used to chain custom components
	};

	enum class ServiceId
	{
		Invalid,                // default value, not valid vid
		Camera,                 // manages the camera in the world
		Render,                 // renders renderobjects in the world
		Physics,                // registers and monitors physics objects
		UIRender,               // renders UI components
		Count                    // last value, can be used to chain custom services
	};
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
    uint32_t GetTypeId() const override { return StaticGetTypeId(); }