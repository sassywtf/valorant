#pragma once
#include "../sdks/structs.hpp"
#include "../sdks/engine.hpp"
#include "../menu/menu.hpp"
#include "../cheat/cheat.hpp"
#include "../skin/skin.hpp"

namespace hooks {
	// 
	void draw(ugame_viewport_client* game_viewport, ucanvas* canvas) {
		auto world = game_viewport->world();
		if (!world) return;

		game_canvas = canvas;
		globals::cached_world = (uint64_t)world;

		canvas->draw_text(_(L"SASSY.WTF"), fvector2d(80.f, 80.f), COL, false, false, true, fvector2d(1.f, 1.f));

		cheat_mgr::loop(world);
		cheat_mgr::chat_spammer(world);
		cheat_mgr::insta_lock();
		menu_mgr::crosshair();
		menu_mgr::draw();

		if (m_config.skins.unlock_all) {
			m_config.skins.unlock_all = false;
			skin_mgr::unlock_all(world);
		}

		if (m_config.settings.unload) {
			if (local_controller) {
				auto camera_manager = local_controller->camera_manager();
				if (camera_manager && globals::old_cave_camera != 0) {
					memory::write<uintptr_t>(uintptr_t(camera_manager), globals::old_cave_camera);
				}
			}

			auto viewport = uengine::static_class()->game_viewport();
			if (viewport && globals::old_cave_render != 0) {
				memory::write<uintptr_t>(uintptr_t(viewport), globals::old_cave_render);
				return;
			}
		}
	}

	void (*orender)(ugame_viewport_client*, ucanvas*);
	void render(ugame_viewport_client* game_viewport, ucanvas* canvas) {
		draw(game_viewport, canvas);
		return spoof_call(orender, game_viewport, canvas);
	}
}