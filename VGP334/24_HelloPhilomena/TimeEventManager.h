#pragma once

#include <Inc/ML_Engine.h>

class TimeEventManager : public ML_Engine::Core::Event
{
public:

    static TimeEventManager* Get();

    void ScheduleEvent(float delay, std::function<void()> callback);

    void Update(float deltaTime);

private:

    struct TimedEvent
    {
        float triggerTime;
        std::function<void()> callback;
    };

    float mCurrentTime = 0.0f;
    std::vector<TimedEvent> mEvents;
};