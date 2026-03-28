#pragma once
#include <windows.h>
#include <winternl.h>
#include <vector>
#include <string>
#include <chrono>

#include "..\config\config.hpp"
#include "..\sdks\game.hpp"

struct skin_mgr
{
	static void unlock_all(uworld* world);
	static uequippable_inv_model* get_gun_model(uares_game_instance* game_instance, int index);
	static void skin_changer(uares_game_instance* game_instance, ashooter_character* pawn);
	static void buddy_changer(uares_game_instance* game_instance, ashooter_character* pawn);
};