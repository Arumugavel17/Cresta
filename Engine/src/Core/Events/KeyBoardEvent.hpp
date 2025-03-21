#pragma once
#include "Core/Events/Event.hpp"
#include "Core/KeyCode.hpp"
#include "Core/Log.hpp"

namespace Cresta {

	class KeyEvent : public Event
	{
	public:
		const std::unordered_set<KeyCode>& GetKeyCode() const { return m_KeyCodes; }
		const int GetKeyCount() const { return m_KeyCodes.size(); }
		EVENT_CLASS_CATEGORY(EventCategory::Keyboard_EventCategory)
	protected:
		KeyEvent(const std::unordered_set<KeyCode>& keycode)
			: m_KeyCodes(keycode)
		{

		}

		const std::unordered_set<KeyCode>& m_KeyCodes;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const std::unordered_set<KeyCode>& keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) 
		{
		}

		bool IsKeyPressed(KeyCode code) 
		{
			return m_KeyCodes.find(code) != m_KeyCodes.end();
		}
		template <typename... KeyCodes>
		bool Has(KeyCode first, KeyCodes... rest)
		{
			return (IsKeyPressed(first) && ... && IsKeyPressed(rest));
		}

		bool IsRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCodes << " (repeat = " << m_IsRepeat << ")";

			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::KeyPressed)

	private:
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const std::unordered_set<KeyCode>& keycode)
			: KeyEvent(keycode) 
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCodes << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const std::unordered_set<KeyCode>& keycode)
			: KeyEvent(keycode) 
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCodes << "\n";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventType::KeyTyped)
	};
}