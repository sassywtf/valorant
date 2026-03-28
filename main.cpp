#include <windows.h>
#include <iostream>

#include "globals.hpp"

#include "util/crt.hpp"
#include "util/import.hpp"
#include "util/xorstr.hpp"

#include "memory/memory.hpp"
#include "spoofer/spoofer.hpp"

#include "sdks/structs.hpp"
#include "sdks/engine.hpp"

#include "menu/menu.hpp"
#include "hooks/render.hpp"

/*
want to update? this source is from 2023 january.
- add mem r/w and vthook
- correct offsets
- shift float to double for ue5
- find new render virtual func to hook
- new spoofcall ofc
- i walk up to the preacher man
*/

bool __stdcall DllMain(void* hinstDLL, std::uint32_t dwReason, void* lpReserved) {
	if (dwReason != DLL_PROCESS_ATTACH) return true;

	//AllocConsole();
	//freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	//freopen_s((FILE**)stdin, "CONIN$", "r", stdin);

	globals::base_address = memory::base_address(_(L"VALORANT-Win64-Shipping.exe"));
	if (!globals::base_address) return false;

	detail::codecave = (void*)(globals::base_address + globals::codecave);

	auto viewport = uengine::static_class()->game_viewport();
	if (!viewport) return false;

	if (!memory::shadow_vt((uintptr_t)viewport, 0x350 / 8, hooks::render, (void**)&hooks::orender, (void**)&globals::old_cave_render, true)) return false;

	return true;
}