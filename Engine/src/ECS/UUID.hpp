#pragma once
#include "Crestaph.hpp"

namespace Cresta
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;
		UUID(UUID&& other) = default;
		UUID& operator=(UUID&& other) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std 
{
	template <typename T> struct hash;

	template<>
	struct hash<Cresta::UUID>
	{
		std::size_t operator()(const Cresta::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}