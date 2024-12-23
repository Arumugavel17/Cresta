#include <iostream>

namespace Cresta {

	class Layer {
	public:
		void Run() {
			std::cout << "Hello World";
		}
	};

	Layer* CreateLayer();
}

int main() {
	Cresta::Layer* layer = Cresta::CreateLayer();
	layer->Run();

	return 0;
}
