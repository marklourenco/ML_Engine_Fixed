#pragma once

#include "Common.h"

#include "App.h"
#include "AppState.h"


// game object info
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "GameWorld.h"

// components
#include "TypeIds.h"
#include "Component.h""
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"

namespace ML_Engine
{
	App& MainApp();
}