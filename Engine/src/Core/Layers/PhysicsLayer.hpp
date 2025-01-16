#pragma once
#include "Layer.hpp"
#include "Core/Physics/Physics.hpp"

namespace Cresta
{
	class  PhysicsLayer : public Layer
	{
	public:
		 PhysicsLayer (Ref<Scene> scene);
		~ PhysicsLayer ();

		virtual void OnEvent(Event& e) override;

		virtual void SceneCallBack() override;
		virtual void OnFixedUpdate() override;
		virtual void OnUpdate() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGUIRender() override;

	private:
		void Init();
		Ref<Physics> m_Physics;
	};
}