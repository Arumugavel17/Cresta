#include "Crestaph.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"

namespace Cresta
{
	class CubeMap
	{
	public:
		CubeMap();
		void SetTextures(std::vector<std::string> textures);

		void Draw();
	private:
		Ref<Shader> m_Shader;
		Ref<VertexArray> sm_VAO;
		std::vector<std::string> m_Textures;

		uint32_t m_TextureID;
	};
}