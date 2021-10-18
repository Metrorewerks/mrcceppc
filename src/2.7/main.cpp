#include <loader/loader.hpp>
#include <memory/memory.hpp>

int __cdecl main(int argc, char* argv[])
{
	loader::load("mwcceppc.exe");
	memory::call<void __cdecl()>(0x00401000)();
	return 0;
}