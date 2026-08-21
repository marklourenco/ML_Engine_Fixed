#pragma once

#include "Common.h"

#include "App.h"
#include "AppState.h"
#include "SaveUtil.h"

// game object info
#include "GameObject.h"
#include "GameObjectHandle.h"
#include "GameWorld.h"
#include "GameObjectFactory.h"

// components
#include "TypeIds.h"
#include "Component.h""
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "RenderObjectComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"
#include "SoundEventComponent.h"
#include "SoundBankComponent.h"
#include "UIComponent.h"
#include "UITextComponent.h"
#include "UISpriteComponent.h"
#include "UIButtonComponent.h"
#include "PlayerControllerComponent.h"
#include "TPSCameraComponent.h"
#include "WallComponent.h"
#include "WallJumpComponent.h"
#include "NetworkControllerComponent.h"

// services
#include "Service.h"
#include "CameraService.h"
#include "RenderService.h"
#include "PhysicsService.h"
#include "UIRenderService.h"
#include "WallService.h"
#include "NetworkService.h"

namespace ML_Engine
{
	App& MainApp();
}