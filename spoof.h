#pragma once

#include <Windows.h>
#include <iostream>
#include <algorithm>

namespace detail
{
	extern "C" void* _spoofer_stub();

	template <typename Ret, typename... Args>
	static inline auto shellcode_stub_helper(const void* shell,Args... args) -> Ret {
		auto fn = (Ret(*)(Args...))(shell);
		return fn(args...);
	}

	template <std::size_t Argc, typename>
	struct argument_remapper{
		template<typename Ret,typename First,typename Second,typename Third,typename Fourth,typename... Pack>static auto do_call(const void* shell, void* shell_param, First first, Second second,Third third, Fourth fourth, Pack... pack) -> Ret {
			return shellcode_stub_helper< Ret, First, Second, Third, Fourth, void*, void*, Pack... >(shell, first, second, third, fourth, shell_param, nullptr, pack...);
		}
	};

	template <std::size_t Argc>
	struct argument_remapper<Argc, std::enable_if_t<Argc <= 4>> {
		template<typename Ret,typename First = void*,typename Second = void*,typename Third = void*,typename Fourth = void*>
			static auto do_call(const void* shell,void* shell_param,First first = First{},Second second = Second{},Third third = Third{},Fourth fourth = Fourth{}) -> Ret {
			return shellcode_stub_helper<Ret,First,Second,Third,Fourth,void*,void*>(
				shell,first,second,third,fourth,shell_param,nullptr);
		}
	};
}
//
//// Define the function template 'hide'
//template <typename Ret, typename... Args>
//static inline auto hide(Ret(*fn)(Args...), Args... args) -> Ret
//{
//    // Static variable to hold the jump address
//    static const void* jmprbx = nullptr;
//
//    // If the jump address is not set
//    if (!jmprbx) {
//        // Get the base address of ntdll
//        const auto ntdll = reinterpret_cast<const unsigned char*>(*(uintptr_t*)(__readgsqword(0x60) + 0x10));
//
//        // Get the DOS and NT headers
//        const auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(ntdll);
//        const auto nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(ntdll + dos->e_lfanew);
//
//        // Get the first section
//        const auto sections = IMAGE_FIRST_SECTION(nt);
//        const auto num_sections = nt->FileHeader.NumberOfSections;
//
//        // Define the section name
//        constexpr char section_name[5]{ '.', 't', 'e', 'x', 't' };
//
//        // Find the section with the given name
//        const auto section = std::find_if(sections, sections + num_sections, & {
//            return std::equal(s.Name, s.Name + 5, section_name);
//            });
//
//        // Define the instruction bytes
//        constexpr unsigned char instr_bytes[2]{ 0xFF, 0x26 };
//
//        // Get the virtual address of the section
//        const auto va = ntdll + section->VirtualAddress;
//
//        // Find the instruction bytes in the section
//        jmprbx = std::search(va, va + section->Misc.VirtualSize, instr_bytes, instr_bytes + 2);
//    }
//
//    // Define the structure for shell parameters
//    struct shell_params {
//        const void* trampoline;
//        void* function;
//        void* rdx;
//    };
//
//    // Initialize the shell parameters
//    shell_params p{
//        jmprbx,
//        reinterpret_cast<void*>(fn)
//    };
//
//    // Define the argument remapper
//    using mapper = detail::argument_remapper<sizeof...(Args), void>;
//
//    // Call the shellcode stub with the arguments
//    return mapper::template do_call<Ret, Args...>((const void*)&detail::_spoofer_stub, &p, args...);
//}

template <typename Ret, typename... Args>
static inline auto hide(Ret(*fn)(Args...), Args... args) -> Ret
{
	static const void* jmprbx = nullptr;
	if (!jmprbx) {
		const auto ntdll = reinterpret_cast<const unsigned char*>(*(uintptr_t*)(__readgsqword(0x60) + 0x10));
		const auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(ntdll);
		const auto nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(ntdll + dos->e_lfanew);
		const auto sections = IMAGE_FIRST_SECTION(nt);
		const auto num_sections = nt->FileHeader.NumberOfSections;

		constexpr char section_name[5]{ '.', 't', 'e', 'x', 't' };
		const auto section = std::find_if(sections, sections + num_sections, [&](const auto& s) {
			return std::equal(s.Name, s.Name + 5, section_name);
			});

		constexpr unsigned char instr_bytes[2]{ 0xFF, 0x26 };
		const auto va = ntdll + section->VirtualAddress;
		jmprbx = std::search(va, va + section->Misc.VirtualSize, instr_bytes, instr_bytes + 2);
	}

	struct shell_params {
		const void* trampoline;
		void* function;
		void* rdx;
	};

	shell_params p{
		jmprbx,
		reinterpret_cast<void*>(fn)
	};

	using mapper = detail::argument_remapper<sizeof...(Args), void>;
	return mapper::template do_call<Ret, Args...>((const void*)&detail::_spoofer_stub, &p, args...);
}