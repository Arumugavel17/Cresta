#pragma once
#include "Core/Events/Event.hpp"
#include "Crestaph.hpp"

namespace Cresta 
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) 
		{
		}

		EVENT_CLASS_CATEGORY(EventCategory::Window_EventCategory);
		EVENT_CLASS_TYPE(EventType::WindowResize);

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }


	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_CATEGORY(EventCategory::Window_EventCategory);
		EVENT_CLASS_TYPE(EventType::WindowClose);
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_CATEGORY(EventCategory::Application_EventCategory);
		EVENT_CLASS_TYPE(EventType::AppTick);
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;


		EVENT_CLASS_CATEGORY(EventCategory::Application_EventCategory);
		EVENT_CLASS_TYPE(EventType::AppUpdate);
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_CATEGORY(EventCategory::Application_EventCategory);
		EVENT_CLASS_TYPE(EventType::AppRender);
	};
}