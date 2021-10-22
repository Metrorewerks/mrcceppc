#pragma once

namespace unsorted
{
	void start();

	void do_exit(const std::uint32_t& exit_code);

	//no names
	void sub_401110(const std::uint32_t& a1);
	void sub_401DF0();
	bool sub_402080();
	void sub_402450();
	void sub_4022D0();
	void sub_402480(const std::uint32_t& a1, const std::uint32_t& a2);
	bool sub_401FC0();
	void sub_402150(std::uint32_t* a1);

	//variables
	extern int dword_54E000;
	extern int dwTlsIndex;
	extern int dword_54E118;
}