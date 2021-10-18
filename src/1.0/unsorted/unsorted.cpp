#include <unsorted/unsorted.hpp>

#include <memory/memory.hpp>

auto unsorted::do_exit(std::uint32_t exit_code) -> void
{
	if (!memory::get<bool>(0x00557950))
	{
		memory::call<void __cdecl(int)>(0x00403D90)(0);

		while (memory::get<int>(0x0054E118) > 0)
		{
			memory::set<int>(0x0054E118, memory::get<int>(0x0054E118) - 1);
		}

		memory::call<void __cdecl (int)>(0x00403DB0)(0);

		if (memory::get<void(__cdecl*)()>(0x00557E5C))
		{
			memory::call<void __cdecl()>(0x00557E5C)();
			memory::set(0x00557E5C, 0);
		}
	}
	memory::call<void __cdecl(std::uint32_t)>(0x00402200)(exit_code);
}

auto unsorted::start() -> void
{
	memory::call<void __cdecl(void(__cdecl**)())>(0x00401110)(reinterpret_cast<void(__cdecl**)()>(0x00508020));
	memory::call<void __cdecl()>(0x00401DF0)();

	if (!memory::call<bool __cdecl()>(0x00402080)())
	{
		do_exit(0xFFFFFFFF);
	}

	memory::call<void __cdecl()>(0x004022D0)();
	memory::call<void __cdecl()>(0x00402450)();
	memory::call<void __cdecl(std::uint32_t, std::uint32_t)>(0x00402480)(memory::get<std::uint32_t>(0x005588C0), memory::get<std::uint32_t>(0x005588B0));
}