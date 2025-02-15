#pragma once
#include "Crestaph.hpp"
#include "Core/Events/Event.hpp"
#include "ECS/Scene/Scene.hpp"

namespace Cresta 
{
	class Layer 
	{
	public:
		Layer(std::string layername, Ref<Scene> scene) 
			: m_LayerName(layername), 
			  p_ActiveScene(scene)
		{
			p_ActiveScene->AddSceneUpdateCallBack([this]() { this->SceneCallBack();});
		}

		virtual ~Layer() = default;

		virtual void OnEvent(Event& e) = 0;

		virtual void SceneCallBack() = 0;
		virtual void OnFixedUpdate() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		virtual void OnImGUIRender() = 0;

		void SetScene(Ref<Scene> scene) { p_ActiveScene = scene; }
		Ref<Scene> GetScene() { return p_ActiveScene; }

		virtual std::string toString() { return m_LayerName; }
	protected:
		Ref<Scene> p_ActiveScene;
	private:
		std::string m_LayerName;
	};
}