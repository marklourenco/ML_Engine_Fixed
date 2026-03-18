#pragma once

#include <Inc/ML_Engine.h>
#include <vector>

class TimeEventManager
{
public:

    static TimeEventManager* Get();

    void ScheduleEvent(float delay, ML_Engine::Core::Event* event);

    void Update(float deltaTime);

private:

    struct TimedEvent
    {
        float triggerTime;
        ML_Engine::Core::Event* event;
    };

    float mCurrentTime = 0.0f;
    std::vector<TimedEvent> mEvents;
};