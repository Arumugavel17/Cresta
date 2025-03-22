#pragma once
#include "Crestaph.hpp"

namespace Cresta
{
	enum class TextureType
	{
		NONE = 0,
		Diffuse = 1,
		Specular = 2,
		Ambient = 3,
		Emissive = 4,
		Height = 5,
		Normals = 6,
		Shininess = 7,
		Opacity = 8,
		Displacement = 9,
		Lightmap = 10,
		Reflection = 11,
		Base_color = 12,
		Normal_camera = 13,
		Emission_color = 14,
		Metalness = 15,
		Diffuse_roughness = 16,
		Ambient_occlusion = 17,
		Unknown = 18,
		Sheen = 19,
		Clearcoat = 20,
		Transmission = 21,
		Maya_base = 22,
		Maya_specular = 23,
		Maya_specular_color = 24,
		Maya_specular_roughness = 25,
	};

	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual const TextureSpecification& GetSpecification() const = 0;

		virtual uint64_t GetTextureHandle() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual TextureType GetTextureType() const { return p_TextureType; }

		virtual void SetTextureType(TextureType type) { p_TextureType = type; }

		virtual const std::string& GetPath() const = 0;

		virtual uint64_t CreateHandle() = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;

	protected:
		TextureType p_TextureType;
	};

	class CubeMap : public Texture
	{
	public:
		static Ref<CubeMap> Create(const std::vector<std::string> txt, bool flipTexture = true);
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(int height,int width,unsigned char* data);
		static Ref<Texture2D> Create(const TextureSpecification& specification);
		static Ref<Texture2D> Create(const std::string& path, bool flipTexture = true);
	};
}