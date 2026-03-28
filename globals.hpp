#pragma once

namespace globals
{
	inline uint64_t base_address = 0;
	inline uint64_t cached_world = 0;

	inline uint64_t old_cave_render = 0;
	inline uint64_t old_cave_camera = 0;

	// main
	constexpr uint32_t codecave = 0x6F24167;
	constexpr uint32_t static_find_object = 0x463DD00;
	constexpr uint32_t static_load_object = 0x463E7B0;
	constexpr uint32_t process_event = 0x46296B0;

	// reflected
	constexpr uint32_t world_to_screen = 0x6225DA0;
	constexpr uint32_t line_of_sight_to = 0x6139400;

	constexpr uint32_t add_pitch_input = 0x6542650;
	constexpr uint32_t add_yaw_input = 0x6543000;
	constexpr uint32_t input_key = 0x67A9A00;

	constexpr uint32_t get_spread_values = 0x316C400;
	constexpr uint32_t get_spread_angles = 0x3AF3DC0;
	constexpr uint32_t get_firing_location = 0x38029C0;

	constexpr uint32_t view_rotation_full_recoil = 0x3807010;
	constexpr uint32_t view_rotation_no_recoil = 0x3807090;

	constexpr uint32_t play_finisher_effect = 0x3103CC0;

	constexpr uint32_t set_outline_mode = 0x60AE0C0;
	constexpr uint32_t set_outline_colors_for_render = 0x4E23960;
	constexpr uint32_t mark_render_state_dirty = 0x6057BA0;

	constexpr uint32_t k2_text_size = 0x67AD7E0;
	constexpr uint32_t k2_draw_text = 0x67AD130;
	constexpr uint32_t k2_draw_line = 0x67AD460;
	constexpr uint32_t k2_draw_box = 0x67AC8E0;

	constexpr uint32_t free_memory = 0x42A8020;
}