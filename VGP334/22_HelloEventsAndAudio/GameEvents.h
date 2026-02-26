#pragma once

#include <Inc/ML_Engine.h>

enum class GameEventType
{
	PressSpace = 1,
	PressEnter
};

class PressSpaceEvent : public ML_Engine::Core::Event
{
public:
	PressSpaceEvent() {}
	SET_EVENT_TYPE_ID(GameEventType::PressSpace)
};

class PressEnterEvent : public ML_Engine::Core::Event
{
public:
	PressEnterEvent() {}
	SET_EVENT_TYPE_ID(GameEventType::PressEnter)
};