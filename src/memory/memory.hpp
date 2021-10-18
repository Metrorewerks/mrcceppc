#pragma once

namespace memory
{
	template <typename T> static auto set(std::uint32_t place, T value) -> void
	{
		*reinterpret_cast<T*>(place) = value;
	}

	template <typename T> static auto get(std::uint32_t place) -> T
	{
		return *reinterpret_cast<T*>(place);
	}

	inline auto replace(std::uint32_t address, std::uint32_t function) -> void
	{
		*reinterpret_cast<std::uint8_t*>(address) = 0xE9;
		*reinterpret_cast<std::uint32_t*>(address + 1) = (function - address - 5);
	}

	template <typename T> inline auto call(std::uint32_t address) -> std::function<T>
	{
		return std::function<T>(reinterpret_cast<T*>(address));
	}
};