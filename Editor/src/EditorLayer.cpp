#include <EntryPoint.cpp>

namespace Cresta {

	class EditorLayer : public Layer {
	public:
		void Run() {
			std::cout << "Editor Layer";
		}
	};

	Cresta::Layer* Cresta::CreateLayer() {
		return new EditorLayer();
	}
}
