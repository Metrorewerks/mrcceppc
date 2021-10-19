#include <loader/loader.hpp>
#include <memory/memory.hpp>

#include <unsorted/unsorted.hpp>

int __cdecl main(int argc, char* argv[])
{
	printf("--------------------\n");
	printf("Metrorewerks\n");
	printf("--------------------\n");
	printf("Compiled on: %s @ %s\n", __DATE__, __TIME__);
#ifdef DEBUG
	printf("DEBUG MODE\n");
#elif NDEBUG
	printf("RELEASE MODE\n");
#endif
	printf("--------------------\n");

	loader::load("mwcceppc.exe");
	memory::replace(0x00401000, (std::uint32_t)unsorted::start);
	memory::call<void __cdecl()>(0x00401000)();
	return 0;
}