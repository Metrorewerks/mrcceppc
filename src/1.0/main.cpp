#include <loader/loader.hpp>
#include <memory/memory.hpp>

#include <unsorted/unsorted.hpp>

int __cdecl main(int argc, char* argv[])
{
	loader::load("mwcceppc.exe");
	memory::replace(0x00401000, (std::uint32_t)unsorted::start);
	memory::call<void __cdecl()>(0x00401000)();
	return 0;
}