#pragma once
#include <windows.h>
#include <winternl.h>
#include <vector>
#include <map>

#include "../sdks/structs.hpp"

struct config_mgr
{
	struct {
		bool enable;
		bool box_2d;
		bool box_3d;
		bool box_corner;
		bool viewpoint;
        bool snapline;
        bool skeleton;
		bool head;
		bool name;
		bool health;
		bool weapon;
		bool distance;
        bool spectators_list;
        bool recoil_crosshair;

        bool dormant;
        bool visible_only;
        bool visible_check;
        bool smoke_check;

        bool weapons_on_ground;
        bool hide_smokes;
        bool hide_blinds;
        bool gadgets;
        bool spike;
        bool orb;
	} visual;

	struct {
        bool players;
        bool children;
        float silohuette[4] = { 0.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.f };
        float center_edge[4] = { 0.f / 255.f, 51 / 255.f, 76.5f / 255.f, 1.f };
        float inner_edge[4] = { 0.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.f };
        float outer_edge[4] = { 0.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.f };
        int outline_type = 1;
        float outline_intensity = 1.f;

		bool fresnel;
		float fresnel_color[4] = { 0.f / 255.f, 204.f / 255.f, 204.f / 255.f, 255.f };
		float fresnel_intensity = 100.f;
		bool fresnel_rainbow = true;
		float fresnel_rainbow_speed = 2.5f;
	} outline;

	struct {
		bool enable;
		bool distance;
		bool auto_fire;
		float fov = 200.f;
		float smoothing = 1.f;
		float multiplier = 1.f;
		int hotkey;

		std::pair<int, bool> bone_type_ids[20] = {
			{8, true},
			{7, false},
			{36, false},
			{11, false},
			{56, false},
			{31, false},
			{57, false},
			{32, false},
			{6, false},
			{5, false},
			{4, false},
			{3, false},
			{77, false},
			{63, false},
			{90, false},
			{76, false},
			{61, false},
			{62, false},
			{83, false},
			{69, false},
		};

		std::vector<gui::multi_select_item> bone_type = {
			{ _("Head"), &bone_type_ids[0].second },
			{ _("Neck"), &bone_type_ids[1].second },
			{ _("Left Shoulder"), &bone_type_ids[2].second },
			{ _("Right Shoulder"), &bone_type_ids[3].second },
			{ _("Left Arm"), &bone_type_ids[4].second },
			{ _("Right Arm"), &bone_type_ids[5].second },
			{ _("Left Hand"), &bone_type_ids[6].second },
			{ _("Right Hand"), &bone_type_ids[7].second },
			{ _("Mid Brest"), &bone_type_ids[8].second },
			{ _("Chest"), &bone_type_ids[9].second },
			{ _("Lower Chest"), &bone_type_ids[10].second },
			{ _("Pelvis"), &bone_type_ids[11].second },
			{ _("Left Thigh"), &bone_type_ids[12].second },
			{ _("Right Thigh"), &bone_type_ids[13].second },
			{ _("Left Bottom Thigh"), &bone_type_ids[14].second },
			{ _("Right Bottom Thigh"), &bone_type_ids[15].second },
			{ _("Upper Left Knee"), &bone_type_ids[16].second },
			{ _("Upper Right Knee"), &bone_type_ids[17].second },
			{ _("Left Foot"), &bone_type_ids[18].second },
			{ _("Right Foot"), &bone_type_ids[19].second }
		};

		bool draw_fov;
		bool visible_check;
		bool smoke_check;
		bool recoil_control;
		bool native_control;
		bool spread_control;
		bool bullet_trace;
		int disable_on_spec = 5;
        bool hit_sound;
	} aimbot;

	struct {
		bool enable;
		float delay = 10.f;
		bool use_hotkey;
		bool fire_once;
		int hotkey;
	} triggerbot;

	struct {
		bool material;
		int material_type = 1;
		float material_color[4] = { 255.f / 255.f, 80.f / 255.f, 255.f / 255.f, 255.f };
		float material_intensity = 0.05f;
		bool material_rainbow = true;
		float material_rainbow_speed = 2.5f;

		bool fresnel;
		float fresnel_color[4] = { 0.f / 255.f, 204.f / 255.f, 204.f / 255.f, 255.f };
		float fresnel_intensity = 100.f;
		bool fresnel_rainbow = true;
		float fresnel_rainbow_speed = 2.5f;

        bool skip_tutorial;
		bool wireframe;
		bool fast_crouch;
		bool bunny_hop;
		bool big_gun;
		float gun_scale = 3.5f;
        bool fov_changer;
        float fov_value = 103.f;
        bool aspect_ratio;
        float ratio_value = 1.f;
        bool anti_afk;
	} self;

	struct {
		bool unlock_all;

		bool skin_changer;
		bool auto_apply_skin;
		bool custom_skins;
		int skin_id;

		bool buddy_changer;
		bool auto_apply_buddy;
		int buddy_id = 494;

        bool finisher;
        bool own_kill;
        bool last_only;
        bool randomizer;
        int finisher_id;
	} skins;

	struct {
		bool enable;
		bool auto_buy;
	} ai;

	struct {
		bool enable;
		bool roulette;
		int agent_id;
	} insta_lock;

	struct {
		bool enable;
		int room = 2;
		int hotkey;
		std::string message = "";
	} chat;

    struct {
        bool third_person;
        float third_distance = 3.5f;
        bool third_person_state;
        int third_person_hotkey;

        bool spinbot;
        float spinbot_speed = 1.5f;
        bool spinbot_server;
        int spinbot_hotkey;

        bool freecam;
        float freecam_speed = 15.f;
        int freecam_hotkey;
    } exploits;

	struct {
		bool enable;
		bool rainbow = true;
		int type = 2;

		float size = 40.f;
		float thickness = 2.5f;
		float color_speed = 10.f;
		float general_speed = 85.f;
	} crosshair;

    struct {
        float invisible_color[4] = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 0.55f };
        float visible_color[4] = { 0.f / 255.f, 250.f / 255.f, 180.f / 255.f, 0.55f };

        float main_fov_color[4] = { 60.f / 255.f, 125.f / 255.f, 250.f / 255.f, 1.0f };
        float secondary_fov_color[4] = { 130.f / 255.f, 0.f, 1.f, 1.0f };
        float fov_speed = 2.f;
        float fov_thickness = 2.5f;
    } colors;

    struct {
        bool unload;
    } settings;
} 

inline m_config;
inline config_mgr m_default_config;
