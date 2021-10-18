#include <loader/loader.hpp>

#pragma comment(linker, "/base:0x400000")
#pragma comment(linker, "/merge:.data=.cld")
#pragma comment(linker, "/merge:.rdata=.clr")
#pragma comment(linker, "/merge:.cl=.main")
#pragma comment(linker, "/merge:.text=.main")
#pragma comment(linker, "/section:.main,re")

#pragma bss_seg(".payload")
char payload_data[BINARY_PAYLOAD_SIZE];

#pragma data_seg(".main")
char main_data[0x1000] = { 1 };

auto loader::load_section(const HMODULE target, const HMODULE source, IMAGE_SECTION_HEADER* section) -> void
{
	void* target_ptr = reinterpret_cast<void*>(reinterpret_cast<std::uint32_t>(target) + section->VirtualAddress);
	const void* source_ptr = reinterpret_cast<void*>(reinterpret_cast<std::uint32_t>(source) + section->PointerToRawData);

	if (section->SizeOfRawData > 0)
	{
		const auto size_of_data = std::min(section->SizeOfRawData, section->Misc.VirtualSize);

		DWORD old_protect;
		VirtualProtect(target_ptr, size_of_data, PAGE_EXECUTE_READWRITE, &old_protect);

		std::memmove(target_ptr, source_ptr, size_of_data);
	}
}

auto loader::load_sections(const HMODULE target, const HMODULE source) -> void
{
	const auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(source);
	const auto nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<std::uint32_t>(source) + dos_header->e_lfanew);

	auto section = IMAGE_FIRST_SECTION(nt_headers);

	for (auto i = 0u; i < nt_headers->FileHeader.NumberOfSections; ++i, ++section)
	{
		if (section)
		{
			loader::load_section(target, source, section);
		}
	}
}

auto loader::find_library(LPCSTR library) -> HMODULE
{
	auto handle = GetModuleHandleA(library);

	if (!handle)
	{
		handle = LoadLibraryA(library);
	}

	return handle;
}

auto loader::load_imports(const HMODULE target, const HMODULE source) -> void
{
	const auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(source);
	const auto nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<std::uint32_t>(source) + dos_header->e_lfanew);

	const auto import_directory = &nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	auto descriptor = PIMAGE_IMPORT_DESCRIPTOR(reinterpret_cast<std::uint32_t>(target) + import_directory->VirtualAddress);

	while (descriptor->Name)
	{
		std::string library_name = LPSTR(reinterpret_cast<std::uint32_t>(target) + descriptor->Name);

		auto name_table_entry = reinterpret_cast<uintptr_t*>(reinterpret_cast<std::uint32_t>(target) + descriptor->OriginalFirstThunk);
		auto address_table_entry = reinterpret_cast<uintptr_t*>(reinterpret_cast<std::uint32_t>(target) + descriptor->FirstThunk);

		if (!descriptor->OriginalFirstThunk)
		{
			name_table_entry = reinterpret_cast<uintptr_t*>(reinterpret_cast<std::uint32_t>(target) + descriptor->FirstThunk);
		}

		while (*name_table_entry)
		{
			FARPROC function = nullptr;

			if (IMAGE_SNAP_BY_ORDINAL(*name_table_entry))
			{
				auto mod = loader::find_library(library_name.data());
				if (mod)
				{
					function = GetProcAddress(mod, MAKEINTRESOURCEA(IMAGE_ORDINAL(*name_table_entry)));
				}
			}
			else
			{
				auto import = PIMAGE_IMPORT_BY_NAME(reinterpret_cast<std::uint32_t>(target) + *name_table_entry);

				auto mod = loader::find_library(library_name.data());
				if (mod)
				{
					function = GetProcAddress(mod, import->Name);
				}
			}

			if (!function)
			{
				throw std::runtime_error("unresolved import!");
			}

			*address_table_entry = reinterpret_cast<uintptr_t>(function);

			name_table_entry++;
			address_table_entry++;
		}

		descriptor++;
	}
}

auto loader::load(const char* bin_name) -> void
{
	std::ifstream bin(bin_name, std::ifstream::binary);

	if (!bin.is_open())
	{
		return;
	}

	bin.seekg(0, bin.end);
	auto binary_size = bin.tellg();
	bin.seekg(0, bin.beg);

	std::vector<std::uint8_t> executable_buffer;
	executable_buffer.resize(binary_size);

	bin.read(reinterpret_cast<char*>(&executable_buffer[0]), binary_size);

	const auto mod = GetModuleHandleA(nullptr);
	const auto module_dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(mod);
	const auto module_nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<uint32_t>(mod) + module_dos_header->e_lfanew);

	const auto source = reinterpret_cast<HMODULE>(&executable_buffer[0]);
	const auto source_dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(source);
	const auto source_nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<uint32_t>(source) + source_dos_header->e_lfanew);

	loader::load_sections(mod, source);
	loader::load_imports(mod, source);

	DWORD old_protect;
	VirtualProtect(module_nt_headers, 0x1000, PAGE_EXECUTE_READWRITE, &old_protect);

	module_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT] = source_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	std::memmove(module_nt_headers, source_nt_headers, sizeof(IMAGE_NT_HEADERS) + (module_nt_headers->FileHeader.NumberOfSections * (sizeof(IMAGE_SECTION_HEADER))));
}