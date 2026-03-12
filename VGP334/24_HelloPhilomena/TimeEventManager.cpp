#include "TimeEventManager.h"

TimeEventManager* TimeEventManager::Get()
{
    static TimeEventManager instance;
    return &instance;
}

void TimeEventManager::ScheduleEvent(float delay, std::function<void()> callback)
{
    TimedEvent e;
    e.triggerTime = mCurrentTime + delay;
    e.callback = callback;

    mEvents.push_back(e);
}

void TimeEventManager::Update(float deltaTime)
{
    mCurrentTime += deltaTime;

    for (size_t i = 0; i < mEvents.size(); )
    {
        if (mCurrentTime >= mEvents[i].triggerTime)
        {
            mEvents[i].callback();
            mEvents.erase(mEvents.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}