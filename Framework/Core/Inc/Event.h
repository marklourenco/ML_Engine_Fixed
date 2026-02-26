#pragma once

namespace ML_Engine::Core
{
	using EventTypeId = std::size_t;

	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		virtual EventTypeId GetTypeId() const = 0;
	};
}

#define SET_EVENT_TYPE_ID(id)\
    static ML_Engine::Core::EventTypeId StaticGetTypeId() { return static_cast<ML_Engine::Core::EventTypeId>(id); }\
    ML_Engine::Core::EventTypeId GetTypeId() const override { return StaticGetTypeId(); }