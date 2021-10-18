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
	unsorted::sub_401110((std::uint32_t)reinterpret_cast<void(__cdecl**)()>(0x00508020));
	//unsorted::sub_401DF0();
	memory::call<void __cdecl()>(0x00401DF0)();

	if (!memory::call<bool __cdecl()>(0x00402080)())
	{
		do_exit(0xFFFFFFFF);
	}

	memory::call<void __cdecl()>(0x004022D0)();
	memory::call<void __cdecl()>(0x00402450)();
	memory::call<void __cdecl(std::uint32_t, std::uint32_t)>(0x00402480)(memory::get<std::uint32_t>(0x005588C0), memory::get<std::uint32_t>(0x005588B0));
}

auto unsorted::sub_401110(const std::uint32_t& a1) -> void
{
	//[/!\]\\

	std::uint32_t v1; // esi
	std::uint32_t v2; // ebp

	v1 = memory::get<std::uint32_t>(0x0054E000);
	v2 = 0;

	while (v1 && **(std::uint32_t**)v1 > **(std::uint32_t**)a1)
	{
		v2 = v1;
		v1 = *(std::uint32_t*)(v1 + 8);
	}

	*(std::uint32_t*)(a1 + 8) = v1;

	if (v2)
	{
		*(std::uint32_t*)(v2 + 8) = a1;
	}
	else
	{
		memory::set<std::uint32_t>(0x0054E000, a1);
	}
}

auto unsorted::sub_401DF0() -> void
{
	//[/!\]\\

	//This function is not properly writing the data at 0x00555768
	// 
	//it needs to be three different pointers with its own data
	//but its writing only the deref value of said pointer
	//
	//ie: instead of 0x00555768 -> 0x0 -> 8
	//it is just writing 0x00555768 -> 8

	int v0; // ebx
	std::uint32_t* v1; // eax
	char* i; // esi
	char* j; // ebp
	std::uint32_t v5; // ebp
	int* v6; // eax
	char* k; // edx
	int v8; // edx
	char* l; // esi
	char* penv; // [esp+4h] [ebp-14h]

	v0 = 0;
	v1 = memory::call<uint32_t* __cdecl()>(0x00401DE0)();
	if (!(*v1)++)
	{
		memory::call<void __cdecl()>(0x00403D30)();

		memory::set(0x00555768, memory::call<std::uint32_t* __cdecl(std::uint32_t)>(0x00404610)(8u));
		if (memory::get<int>(0x00555768))
		{ 
			memory::set<std::uint32_t*>(0x00555768, (std::uint32_t*)::GetStdHandle(0xFFFFFFF6));
			memory::set<bool>(0x00555768 + 4, 0);
			memory::set<bool>(0x00555768 + 5, 0);
		}

		memory::set(0x0055576C, memory::call<std::uint32_t* __cdecl(std::uint32_t)>(0x00404610)(8u));
		if (memory::get<int>(0x0055576C))
		{
			memory::set<std::uint32_t*>(0x0055576C, (std::uint32_t*)::GetStdHandle(0xFFFFFFF5));
			memory::set<bool>(0x0055576C + 4, 0);
			memory::set<bool>(0x0055576C + 5, 0);
		}

		memory::set(0x00555770, memory::call<std::uint32_t* __cdecl(std::uint32_t)>(0x00404610)(8u));
		if (memory::get<int>(0x00555770))
		{
			memory::set<std::uint32_t>(0x00555770, (std::uint32_t)::GetStdHandle(0xFFFFFFF4));
			memory::set<bool>(0x00555770 + 4, 0);
			memory::set<bool>(0x00555770 + 5, 0);
		}

		memory::set(0x00558470, 3);
		penv = GetEnvironmentStrings();

		for (i = GetEnvironmentStrings(); *i == 61; i += strlen(i) + 1);
		;

		for (j = i; *j; j += strlen(j) + 1);
		;

		v5 = j - i;
		v6 = memory::call<int* __cdecl(std::uint32_t)>(0x00404610)(v5);
		memory::set(0x0054E014, (int)v6);

		if (v6)
		{
			memory::call<void __cdecl(char*, char*, std::uint32_t)>(0x00404C30)((char*)v6, i, v5);
		}

		memory::set(0x00557970, (int)reinterpret_cast<int*>(0x0054E014));
		for (k = (char*)memory::get<int>(0x0054E014); *k; k += strlen(k) + 1)
		{
			++v0;
		}

		memory::set(0x00558654, (int)memory::call<int* __cdecl(std::uint32_t)>(0x00404610)((v0 + 1) * 4));

		v8 = 0;

		for (l = (char*)memory::get<std::uint32_t>(0x00557970); *l; l += strlen(l) + 1)
		{
			*(char**)(memory::get<std::uint32_t>(0x00558654) + 4 * v8++) = l;
		}

		*(std::uint32_t*)(memory::get<std::uint32_t>(0x00558654) + 4 * v8) = 0;
		::FreeEnvironmentStringsA(penv);
	}
}