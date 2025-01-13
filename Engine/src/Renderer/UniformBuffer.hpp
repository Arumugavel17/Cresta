#pragma once

namespace Cresta 
{
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
		virtual void Bind() = 0;

		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding,const void* Data);
	};

}
