#pragma once
#include "Crestaph.hpp"
#include "Core/Events/Event.hpp"
#include <memory>
#include <functional>

namespace Cresta 
{
	using EventCallBackFn = std::function<void(Event&)>;

	class Window 
	{
	public:
		Window() = default;
		virtual ~Window() = default;
		virtual void Init() = 0;
		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void* GetWindowContext() = 0;
		virtual void OnShutDown() = 0;
		virtual void SetEventCallBack(std::function<void(Event&)> fn) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		
		void ResizeViewPort();
		
		static Ref<Window> Create();

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallBackFn CallBackFunction;
		};

	protected:
		WindowData p_Data;
	};
}