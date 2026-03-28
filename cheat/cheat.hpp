#pragma once
#include <windows.h>
#include <winternl.h>
#include <vector>
#include <string>
#include <chrono>

#include "..\config\config.hpp"
#include "..\sdks\game.hpp"
#include "..\skin\skin.hpp"
#include "..\hooks\camera.hpp"
#include "..\hooks\death.hpp"
//#include "..\hooks\damaged.hpp"

inline auto dist_max = FLT_MAX;
inline aactor* target_actor = nullptr;
inline uskeletal_mesh_component* target_mesh = nullptr;
inline int target_bone_id = -1;

struct cheat_mgr
{
	static void boxes2d(aplayer_controller* controller, uskeletal_mesh_component* mesh, bool visible);
	static void boxes3d(aplayer_controller* controller, ashooter_character* actor, uskeletal_mesh_component* mesh, bool visible);
	static void boxes_corner(aplayer_controller* controller, uskeletal_mesh_component* mesh, bool visible);
	static void viewpoint(aplayer_controller* controller, uskeletal_mesh_component* mesh, uscene_component* root, bool visible);
	static void snapline(aplayer_controller* controller, uskeletal_mesh_component* mesh, bool visible);
	static void skeleton(aplayer_controller* controller, uskeletal_mesh_component* mesh, bool visible);
	static void head(aplayer_controller* controller, uskeletal_mesh_component* mesh, uscene_component* root, bool visible);
	static void name(aplayer_controller* controller, ashooter_character* actor, uskeletal_mesh_component* mesh, bool visible);
	static void health(aplayer_controller* controller, uskeletal_mesh_component* mesh, float life, bool visible);
	static void weapon(aplayer_controller* controller, ashooter_character* actor, uskeletal_mesh_component* mesh, bool visible);
	static void distance(aplayer_controller* controller, uskeletal_mesh_component* mesh, float distance, bool visible);
	static void recoil_crosshair(aplayer_controller* controller);

	static void insta_lock();
	static void chat_spammer(uworld* world);
	static void loop(uworld* world);
};