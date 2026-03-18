#pragma once

#include <Inc/ML_Engine.h>

enum class GameEventType
{
    ChangePredatorAnim,
    ChangeSoldierAnim,
    ChangeInfrared
};

class ChangePredatorAnimEvent : public ML_Engine::Core::Event
{
public:
    ChangePredatorAnimEvent(int animIndex, bool loop) : anim(animIndex), loop(loop) {}

    int anim = 0;
	bool loop = true;

    SET_EVENT_TYPE_ID(GameEventType::ChangePredatorAnim)
};

class ChangeSoldierAnimEvent : public ML_Engine::Core::Event
{
public:
    ChangeSoldierAnimEvent(int animIndex, int soldier, bool loop) : anim(animIndex), sold(soldier), loop(loop) {}

    int sold = 0;
    int anim = 0;
	bool loop = true;

    SET_EVENT_TYPE_ID(GameEventType::ChangeSoldierAnim)
};

class ChangeInfraredEvent : public ML_Engine::Core::Event
{
public:
    ChangeInfraredEvent() {}

    SET_EVENT_TYPE_ID(GameEventType::ChangeInfrared)
};