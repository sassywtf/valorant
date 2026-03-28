#pragma once
#include <windows.h>
#include <winternl.h>
#include <vector>

#include "..\sdks\engine.hpp"
#include "..\sdks\game.hpp"

class unet_random_component : public uobject {};

class ustability_component : public uobject
{
public:
	void get_spread_values(float* output);
	void get_spread_angles(uintptr_t seed, fvector* direction, float err_degrees, float err_power, int err_retries, uintptr_t spread_angles);
};

class ufiring_state_component : public uobject
{
public:
	ustability_component* stability_component();
	unet_random_component* net_random_component();

	float error_power();
	int error_retries();
	fvector error_angles(ashooter_character* actor);
};

// i always thought how retarded this is but linuxtechtips said if it works dont touch it.
class uequippable_skin_data_asset : public uobject
{
public:
	struct type {
		const wchar_t* search = nullptr;
		const char* config = nullptr;

		std::int32_t index = 0;

		const bool is_valid() const noexcept {
			return this->search != nullptr || this->config != nullptr;
		}
	};

	uequippable_skin_data_asset::type get_type();
	tarray<int> skin_levels();
};

class uequippable_skin_chroma_data_asset : public uobject {};
class uequippable_charm_data_asset : public uobject {};

class aares_item : public apawn {};

class aares_equippable : public aares_item
{
public:
	struct type {
		const wchar_t* search = nullptr;
		const char* config = nullptr;
		std::int32_t index = 0;

		const bool is_valid() const noexcept {
			return this->search != nullptr || this->config != nullptr;
		}
	};

	aares_equippable::type get_type();
	ufiring_state_component* firing_state();
	uskeletal_mesh_component* mesh_1p();
	uskeletal_mesh_component* mesh_3p();
	uskeletal_mesh_component* magazine_1p();
	uskeletal_mesh_component* secondary_magazine_1p();
	uskeletal_mesh_component* magazine_3p();
	uskeletal_mesh_component* cosmetic_mesh_1p();
	uequippable_skin_data_asset* skin_data_asset();
	uequippable_skin_chroma_data_asset* chroma_data_asset();
	uequippable_charm_data_asset* charm_data_asset();

	ftransform muzzle_transform();

	void set_skin_data_asset(uequippable_skin_data_asset* skin_data);
	void set_chroma_data_asset(uequippable_skin_data_asset* chroma_data);
	void set_charm_data_asset(uequippable_charm_data_asset* charm_data);
	void set_scale_on_ground(float val);
	bool can_be_dropped();
	bool only_allies_can_pickup();
	bool has_muzzle();
};

class ucontent_library : public uobject
{
public:
	static ucontent_library* static_class() {
		static ucontent_library* klass = 0;
		if (!klass)
			klass = (ucontent_library*)uobject::find_object(_(L"ShooterGame.ContentLibrary"));

		return klass;
	}

	void clear_weapon_components(uobject* parent);
	void apply_skin(uobject* parent, uobject* skin_asset, uobject* chroma_asset, int skin_level, uobject* charm_asset, int charm_level);
};

class uares_inventory : public uobject
{
public:
	aares_equippable* current_equippable();
};

class ubase_team_component : public uobject
{
public:
	static ubase_team_component* static_class() {
		static ubase_team_component* klass = 0;
		if (!klass)
			klass = (ubase_team_component*)uobject::find_object(_(L"ShooterGame.BaseTeamComponent"));

		return klass;
	}

	ubase_team_component* get_team_component(uobject* source_actor);
	bool is_ally(uobject* source_object, uobject* target_object);
};

class udamageable_component : public uobject
{
public:
	fhealth_value cached_life();
	bool is_alive();
};

class aares_player_state : public aplayer_state
{
public:
	tarray<aares_player_state*> get_players_spectating();
};

class ubase_inventory_model : public uobject
{
public:
	uequippable_skin_data_asset* data_asset();
	void unlock_item();
};

class uequippable_skin_lvl_inv_model : public ubase_inventory_model {};

class uequippable_attach_inv_model : public ubase_inventory_model {};

class uequippable_skin_chroma_inv_model : public ubase_inventory_model {};

class uequippable_skin_inv_model : public ubase_inventory_model
{
public:
	uequippable_skin_chroma_inv_model* equipped_chroma();
	tarray<uequippable_skin_chroma_inv_model*> possible_chromas();
	tarray<uequippable_skin_lvl_inv_model*> possible_levels();
};

class uequippable_charm_inst_inv_model : public ubase_inventory_model
{
public:
	uequippable_charm_data_asset* data_asset();
};

class uequippable_inv_model : public ubase_inventory_model
{
public:
	uequippable_skin_inv_model* equipped_model();
	uequippable_charm_inst_inv_model* charm_model();
	tarray<uequippable_skin_inv_model*> possible_skins();
	tarray<uequippable_attach_inv_model*> possible_attachments();
};

class uares_inventory_manager : public uobject
{
public:
	tarray<uequippable_inv_model*> equippable_models();
	tarray<tmap<uequippable_charm_data_asset*, uequippable_charm_inst_inv_model*>> data_asset_to_equippable_charm_map();
};


class uarsenal_view_model : public uobject
{
public:
	tarray<uequippable_inv_model*> gun_models();
};

class uarsenal_view_controller : public uobject
{
public:
	uarsenal_view_model* view_model();
};

class uthreaded_chat_manager : public uobject
{
public:
	void send_chat_message_v2(uint8_t room, ftext message);
};

class ugame_flow_state : public uobject
{
public:
	enum class egame_flow_state_type : uint8_t {
		initialization = 0,
		transition_to_main_menu = 1,
		main_menu = 2,
		transition_to_pregame = 3,
		pregame = 4,
		transition_to_ingame = 5,
		ingame = 6,
		transition_to_platform_faulted = 7,
		platform_faulted = 8,
		invalid = 9,
		count = 10,
		egame_flow_state_type_max = 11
	};

	egame_flow_state_type get_flow_state_type();
	void set_flow_state_type(egame_flow_state_type state_type);
};

class ugame_flow_state_manager : public uobject
{
public:
	ugame_flow_state* current_state();
};

class uares_cl_game_instance : public uobject
{
public:
	uares_inventory_manager* inventory_manager();
	uarsenal_view_controller* view_controller();
	uthreaded_chat_manager* threaded_chat_manager();
	ugame_flow_state_manager* game_flow_state_manager();
};

class upregame_view_model : public uobject
{
public:
	bool is_locked_in();
};

class apregame_view_controller : public uobject
{
public:
	upregame_view_model* view_model();
	void lock_character(uobject* character);
};

class abase_player_controller : public aplayer_controller {};

class aares_player_controller : public abase_player_controller
{
public:
	apregame_view_controller* pregame_view_controller();

	void toggle_shop();
	void disconnect_from_server();
	void set_fov(float val);
	bool is_shop_open();
};

class ublind_manager_component : public uobject
{
public:
	void client_cleanse_blinds();
	bool is_blinded();
};

class aeffect_container : public apawn {};

class ushooter_character_death_reaction_component : public apawn
{
public:
	void set_death_montage_effect_override(aeffect_container* effect);
	void set_death_montage_effect_override_context(uobject* context);
	void play_finisher_effect();
};

class ashooter_character : public acharacter
{
public:
	uares_inventory* inventory();
	udamageable_component* damage_handler();
	uskeletal_mesh_component* mesh_1p();
	uskeletal_mesh_component* mesh_overlay_1p();
	uskeletal_mesh_component* mesh_cosmetic_3p();
	ublind_manager_component* blind_manager_component();
	ashooter_character* simple_trace_from_camera(float distance, ecollision_channel channel);
	ushooter_character_death_reaction_component* death_reaction_component();
	tarray<uprimitive_component*> default_primitive_components();

	frotator view_rotation_no_recoil();
	frotator view_rotation_full_recoil();

	void set_vector_on_mids(fname name, flinearcolor value);
	void set_scalar_on_mids(fname name, float value);
	void reset_character_materials(uskeletal_mesh_component* mesh_to_reset);
	void set_3p_mesh_visible(bool val);
	void set_crouch_time(float val);
	void set_is_in_spawn_zone(bool is_in_spawn);
	bool is_still_firing();
	bool get_firing_data(fvector* location, fvector* rotation, bool apply_recoil);
	bool get_is_in_spawn_zone();
};

class uares_game_instance : public ugame_instance
{
public:
	uares_cl_game_instance* client_game_instance();
};

class uares_outline_rendering : public uobject
{
public:
	//Renderer.AresOutlineRendering
	static void set_outline_colors_for_render(uobject* world_context, flinearcolor ally_color, flinearcolor enemy_color);
};

class ares_characters_function_library : public uobject
{
public:
	static ares_characters_function_library* static_class() {
		static ares_characters_function_library* klass = 0;
		if (!klass)
			klass = (ares_characters_function_library*)uobject::find_object(_(L"Ares_Characters_FunctionLibrary.Ares_Characters_FunctionLibrary_C"));

		return klass;
	}

	bool has_line_of_sight_to_character(uobject* viewer, uobject* target, ecollision_channel trace_channel, uobject* world_context, fvector focus_point);
};

class agame_object : public apawn {};

class aplanted_bomb_c : public agame_object
{
public:
	bool bomb_has_exploded();
	bool bomb_has_been_defused();
	float defuse_progress();
	float time_remaining_to_explode();
};

class aares_on_ground_equippable : public agame_object
{
public:
	aares_equippable* my_equippable();
};

class ault_point_orb_c : public agame_object {};

class ushooter_blueprint_library : public uobject
{
public:
	static ushooter_blueprint_library* static_class() {
		static ushooter_blueprint_library* klass = 0;
		if (!klass)
			klass = (ushooter_blueprint_library*)uobject::find_object(_(L"ShooterGame.ShooterBlueprintLibrary"));

		return klass;
	}

	enum class eares_alliance : uint8_t {
		alliance_ally = 0,
		alliance_enemy = 1,
		alliance_neutral = 2,
		alliance_any = 3,
		alliance_count = 4,
		alliance_max = 5
	};

	bool is_bone_visible(fname bone_name, fvector camera_location, aactor* viewee, uskeletal_mesh_component* skeletal_mesh_component, ecollision_channel channel_type = ecollision_channel::ecc_visibility);

	template<typename type>
	tarray<type*> find_all_shooter_characters_with_alliance(uobject* world_context, uobject* viewer, ushooter_blueprint_library::eares_alliance alliance = eares_alliance::alliance_enemy, bool only_player_controlled = false, bool only_alive_players = true) {
		if (!this || !world_context || !viewer) return {};

		static uobject* fn = nullptr;
		if (!fn)
			fn = uobject::find_object(_(L"ShooterGame.ShooterBlueprintLibrary.FindAllShooterCharactersWithAlliance"));

		struct {
			uobject* world_context;
			uobject* viewer;
			ushooter_blueprint_library::eares_alliance alliance;
			bool only_player_connected;
			bool only_alive_players;
			tarray<type*> return_value;
		} params = { world_context, viewer, alliance, only_player_controlled, only_alive_players };

		process_event(fn, &params);
		return params.return_value;
	}

	tarray<agame_object*> find_all_game_objects(uobject* world_context) {
		if (!this || !world_context) return {};

		static uobject* fn = nullptr;
		if (!fn)
			fn = uobject::find_object(_(L"ShooterGame.ShooterBlueprintLibrary.FindAllGameObjects"));

		struct {
			uobject* world_context;
			tarray<agame_object*> return_value;
		} params = { world_context };

		process_event(fn, &params);
		return params.return_value;
	}
};

class udamage_response : public uobject
{
public:
	apawn* get_event_instigator_pawn();
};