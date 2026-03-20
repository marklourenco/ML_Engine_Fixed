#include "TimeEventManager.h"

using namespace ML_Engine::Core;

TimeEventManager* TimeEventManager::Get()
{
    static TimeEventManager instance;
    return &instance;
}

void TimeEventManager::ScheduleEvent(float delay, Event* event)
{
    TimedEvent e;
    e.triggerTime = mCurrentTime + delay;
    e.event = event;

    mEvents.push_back(e);
}

void TimeEventManager::Update(float deltaTime)
{
    mCurrentTime += deltaTime;

    for (size_t i = 0; i < mEvents.size();)
    {
        if (mCurrentTime >= mEvents[i].triggerTime)
        {
            EventManager::Broadcast(*mEvents[i].event);

            // cleanup
            delete mEvents[i].event;
            mEvents.erase(mEvents.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}