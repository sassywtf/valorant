#include "game.hpp"

void ustability_component::get_spread_values(float* output) {
	if (!this) return;

	typedef void(__fastcall* get_spread_values)(ustability_component*, float*);
	auto get_spread_values_fn = (get_spread_values)(globals::base_address + globals::get_spread_values);
	spoof_call(get_spread_values_fn, this, output);
}

void ustability_component::get_spread_angles(uintptr_t seed, fvector* direction, float err_degrees, float err_power, int err_retries, uintptr_t spread_angles) {
	if (!this) return;

	typedef void(__fastcall* get_spread_angles)(uintptr_t, fvector*, float, float, int, int, uintptr_t);
	auto get_spread_angles_fn = (get_spread_angles)(globals::base_address + globals::get_spread_angles);
	spoof_call(get_spread_angles_fn, seed, direction, err_degrees, err_power, err_retries, 1, spread_angles);
}

//

ustability_component* ufiring_state_component::stability_component() {
	if (!this) return nullptr;

	return reinterpret_cast<ustability_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

unet_random_component* ufiring_state_component::net_random_component() {
	if (!this) return nullptr;

	return reinterpret_cast<unet_random_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

float ufiring_state_component::error_power() {
	if (!this) return 0.f;

	return memory::read<float>(reinterpret_cast<long long>(this) + 0x0); // why you remove unreflect it :(
}

int ufiring_state_component::error_retries() {
	if (!this) return 0;

	return memory::read<int>(reinterpret_cast<long long>(this) + offset);
}

fvector ufiring_state_component::error_angles(ashooter_character* actor) {
	if (!this || !actor) return {};

	uint8_t error_values[4096] = { 0 };
	uint8_t seed_data_snapshot[4096] = { 0 };
	uint8_t spread_angles[4096] = { 0 };
	uint8_t out_spread_angles[4096] = { 0 };

	memset(error_values, 0, sizeof(error_values));
	memset(seed_data_snapshot, 0, sizeof(seed_data_snapshot));
	memset(spread_angles, 0, sizeof(spread_angles));
	memset(out_spread_angles, 0, sizeof(out_spread_angles));

	*(uint64_t*)(&out_spread_angles[0]) = (uint64_t)&spread_angles[0];
	*(int*)(&out_spread_angles[0] + 8) = 1;
	*(int*)(&out_spread_angles[0] + 12) = 1;

	auto stability = stability_component();
	if (!stability) return {};

	stability->get_spread_values((float*)(&error_values[0]));

	auto seed = net_random_component();
	memcpy((void*)seed_data_snapshot, (void*)seed, sizeof(seed_data_snapshot));

	fvector tmp1{}, tmp2{}, tmp3{}, tmp4{};
	actor->get_firing_data(&tmp1, &tmp3, false);

	math::static_class()->to_vector_normalize(tmp3, &tmp2);
	math::static_class()->to_angle_normalize(tmp2, &tmp1);
	tmp3 = tmp1;

	tmp1.x += *(float*)(&error_values[0] + 12);
	tmp1.y += *(float*)(&error_values[0] + 16);

	math::static_class()->to_vector_normalize(tmp1, &tmp4);

	auto err_degrees = *(float*)(&error_values[0] + 8) + *(float*)(&error_values[0] + 4);
	auto err_power = error_power();
	auto err_retries = error_retries();

	*(bool*)(((uintptr_t)&seed_data_snapshot[0]) + 0xE8 + 0x20) = false;

	for (int i = 0; i < 7; i++) {
		*(char*)(((uintptr_t)&seed_data_snapshot[0]) + 0xE8 + 0x21 + i) = 0; // lolz
	}

	// powa = stability+0x14

	stability->get_spread_angles(((uintptr_t)&seed_data_snapshot[0]) + 0xE8 /* ReusableHitScanProjectileComponent - struct FProjectileTuning ProjectileTuning; // 0xe8(0x28) */, &tmp4, err_degrees, err_power, err_retries, (uintptr_t)&out_spread_angles[0]);

	auto spread_value = *(fvector*)(&spread_angles[0]);
	math::static_class()->to_angle_normalize(spread_value, &tmp4);

	return tmp4 - tmp3;
}

//

uequippable_skin_data_asset::type uequippable_skin_data_asset::get_type() {
	std::string name = system::static_class()->get_object_name(this);
	std::wstring wname(name.begin(), name.end());

	uequippable_skin_data_asset::type type;

	if (string::static_class()->contains(wname.c_str(), _(L"default__basepistol"))) {
		type = { _(L"default__basepistol"), _("classic"), 0 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__sawedoffshotgun"))) {
		type = { _(L"default__sawedoffshotgun"), _("shorty"), 1 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__autopistol"))) {
		type = { _(L"default__autopistol"), _("frenzy"), 2 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__luger"))) {
		type = { _(L"default__luger"), _("ghost"), 3 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__revolver"))) {
		type = { _(L"default__revolver"), _("sheriff"), 4 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__vector"))) {
		type = { _(L"default__vector"), _("stinger"), 5 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__mp5"))) {
		type = { _(L"default__mp5"), _("spectre"), 6 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__pumpshotgun"))) {
		type = { _(L"default__pumpshotgun"), _("bucky"), 7 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__automaticshotgun"))) {
		type = { _(L"default__automaticshotgun"), _("judge"), 8 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__burst"))) {
		type = { _(L"default__burst"), _("bulldog"), 9 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__dmr"))) {
		type = { _(L"default__dmr"), _("guardian"), 10 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__assaultrifle_acr"))) {
		type = { _(L"default__assaultrifle_acr"), _("phantom"), 11 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__ak"))) {
		type = { _(L"default__ak"), _("vandal"), 12 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__leversniperrifle"))) {
		type = { _(L"default__leversniperrifle"), _("marshal"), 13 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__ds_gun"))) {
		type = { _(L"default__ds_gun"), _("outlaw"), 14 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__boltsniper"))) {
		type = { _(L"default__boltsniper"), _("operator"), 15 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__lmg"))) {
		type = { _(L"default__lmg"), _("ares"), 16 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__heavymachinegun"))) {
		type = { _(L"default__heavymachinegun"), _("odin"), 17 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__melee"))) {
		type = { _(L"default__melee"), _("knife"), 18 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__carbine"))) {
		type = { _(L"default__carbine"), _("phantom"), 11 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__automaticpistol"))) {
		type = { _(L"default__automaticpistol"), _("frenzy"), 2 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"default__subMachinegun_mp5"))) {
		type = { _(L"default__subMachinegun_mp5"), _("spectre"), 6 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"Default__Slim"))) {
		type = { _(L"Default__Slim"), _("shorty"), 1 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"Default__hmg"))) {
		type = { _(L"Default__hmg"), _("odin"), 17 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"Default__lightmachinegun"))) {
		type = { _(L"Default__lightmachinegun"), _("odin"), 16 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"Default__assaultrifle_ak"))) {
		type = { _(L"Default__assaultrifle_ak"), _("vandal"), 12 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"Default__AssaultRifle_Burst"))) {
		type = { _(L"Default__AssaultRifle_Burst"), _("bulldog"), 9 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"Default__AutoShotgun"))) {
		type = { _(L"Default__AutoShotgun"), _("judge"), 8 };
	}
	else if (string::static_class()->contains(wname.c_str(), _(L"Default__LeverSniper"))) {
		type = { _(L"Default__LeverSniper"), _("marshal"), 13 };
	}
	else {
		return {};
	}

	return type;
}

tarray<int> uequippable_skin_data_asset::skin_levels() {
	if (!this) return {};

	return memory::read<tarray<int>>(reinterpret_cast<long long>(this) + offset);
}

//

aares_equippable::type aares_equippable::get_type() {
	if (!this) return {};

	std::string name = system::static_class()->get_object_name(this);
	std::wstring wname(name.begin(), name.end());

	aares_equippable::type type;

	if (string::static_class()->starts_with(wname.c_str(), _(L"basepistol"))) {
		type = { _(L"basepistol"), _("classic"), 0 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"sawedoffshotgun"))) {
		type = { _(L"sawedoffshotgun"), _("shorty"), 1 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"automaticpistol"))) {
		type = { _(L"automaticpistol"), _("frenzy"), 2 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"lugerpistol"))) {
		type = { _(L"lugerpistol"), _("ghost"), 3 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"revolverpistol"))) {
		type = { _(L"revolverpistol"), _("sheriff"), 4 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"vector"))) {
		type = { _(L"vector"), _("stinger"), 5 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"submachinegun_mp5"))) {
		type = { _(L"submachinegun_mp5"), _("spectre"), 6 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"pumpshotgun"))) {
		type = { _(L"pumpshotgun"), _("bucky"), 7 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"automaticshotgun"))) {
		type = { _(L"automaticshotgun"), _("judge"), 8 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"assaultrifle_burst"))) {
		type = { _(L"assaultrifle_burst"), _("bulldog"), 9 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"dmr"))) {
		type = { _(L"dmr"), _("guardian"), 10 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"assaultrifle_acr"))) {
		type = { _(L"assaultrifle_acr"), _("phantom"), 11 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"assaultrifle_ak"))) {
		type = { _(L"assaultrifle_ak"), _("vandal"), 12 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"leversniperrifle"))) {
		type = { _(L"leversniperrifle"), _("marshal"), 13 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"ds_gun"))) {
		type = { _(L"ds_gun"), _("outlaw"), 14 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"boltsniper"))) {
		type = { _(L"boltsniper"), _("operator"), 15 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"lightmachinegun"))) {
		type = { _(L"lightmachinegun"), _("ares"), 16 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"heavymachinegun"))) {
		type = { _(L"heavymachinegun"), _("odin"), 17 };
	}
	else if (string::static_class()->starts_with(wname.c_str(), _(L"ability_melee_base"))) {
		type = { _(L"ability_melee_base"), _("knife"), 18 };
	}
	else {
		return {};
	}

	return type;
}

ufiring_state_component* aares_equippable::firing_state() {
	if (!this) return nullptr;

	return reinterpret_cast<ufiring_state_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uskeletal_mesh_component* aares_equippable::mesh_1p() {
	if (!this) return nullptr;

	return reinterpret_cast<uskeletal_mesh_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uskeletal_mesh_component* aares_equippable::mesh_3p() {
	if (!this) return nullptr;

	return reinterpret_cast<uskeletal_mesh_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uskeletal_mesh_component* aares_equippable::magazine_1p() {
	if (!this) return nullptr;

	return reinterpret_cast<uskeletal_mesh_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uskeletal_mesh_component* aares_equippable::secondary_magazine_1p() {
	if (!this) return nullptr;

	return reinterpret_cast<uskeletal_mesh_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uskeletal_mesh_component* aares_equippable::magazine_3p() {
	if (!this) return nullptr;

	return reinterpret_cast<uskeletal_mesh_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uskeletal_mesh_component* aares_equippable::cosmetic_mesh_1p() {
	if (!this) return nullptr;

	return reinterpret_cast<uskeletal_mesh_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uequippable_skin_data_asset* aares_equippable::skin_data_asset() {
	if (!this) return nullptr;

	return reinterpret_cast<uequippable_skin_data_asset*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uequippable_skin_chroma_data_asset* aares_equippable::chroma_data_asset() {
	if (!this) return nullptr;

	return reinterpret_cast<uequippable_skin_chroma_data_asset*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uequippable_charm_data_asset* aares_equippable::charm_data_asset() {
	if (!this) return nullptr;

	return reinterpret_cast<uequippable_charm_data_asset*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

ftransform aares_equippable::muzzle_transform() {
	if (!this) return {};

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.AresEquippable.GetMuzzleTransform"));

	struct {
		ftransform return_value;
	} params;

	process_event(fn, &params);
	return params.return_value;
}

void aares_equippable::set_skin_data_asset(uequippable_skin_data_asset* skin_data) {
	if (!this || !skin_data) return;

	memory::write<long long>(reinterpret_cast<long long>(this) + offset, reinterpret_cast<long long>(skin_data));
}

void aares_equippable::set_chroma_data_asset(uequippable_skin_data_asset* chroma_data) {
	if (!this || !chroma_data) return;

	memory::write<long long>(reinterpret_cast<long long>(this) + offset, reinterpret_cast<long long>(chroma_data));
}

void aares_equippable::set_charm_data_asset(uequippable_charm_data_asset* charm_data) {
	if (!this || !charm_data) return;

	memory::write<long long>(reinterpret_cast<long long>(this) + offset, reinterpret_cast<long long>(charm_data));
}

void aares_equippable::set_scale_on_ground(float val) {
	if (!this) return;

	memory::write<float>(reinterpret_cast<long long>(this) + offset, val);
}

bool aares_equippable::can_be_dropped() {
	if (!this) return false;

	return memory::read<bool>(reinterpret_cast<long long>(this) + offset);
}

bool aares_equippable::only_allies_can_pickup() {
	if (!this) return false;

	return memory::read<bool>(reinterpret_cast<long long>(this) + offset);
}

bool aares_equippable::has_muzzle() {
	if (!this) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.AresEquippable.HasMuzzle"));

	struct {
		bool return_value;
	} params;

	process_event(fn, &params);
	return params.return_value;
}

//

void ucontent_library::clear_weapon_components(uobject* parent) {
	if (!this || !parent) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ContentLibrary.ClearWeaponComponents"));

	struct {
		uobject* parent;
	} params = { parent };

	process_event(fn, &params);
}

void ucontent_library::apply_skin(uobject* parent, uobject* skin_asset, uobject* chroma_asset, int skin_level, uobject* charm_asset, int charm_level) {
	if (!this || !parent || !skin_asset || !chroma_asset) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ContentLibrary.ApplySkin"));

	struct {
		uobject* parent;
		uobject* skin_asset;
		uobject* chroma_asset;
		int skin_level;
		uobject* charm_asset;
		int charm_level;
	} params = { parent, skin_asset, chroma_asset, skin_level, charm_asset, charm_level };

	process_event(fn, &params);
}

//

aares_equippable* uares_inventory::current_equippable() {
	if (!this) return nullptr;

	return reinterpret_cast<aares_equippable*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

//

ubase_team_component* ubase_team_component::get_team_component(uobject* source_actor) {
	if (!this || !source_actor) return nullptr;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.BaseTeamComponent.GetTeamComponent"));

	struct {
		uobject* source_actor;
		ubase_team_component* return_value;
	} params = { source_actor };

	process_event(fn, &params);
	return params.return_value;
}

bool ubase_team_component::is_ally(uobject* source_object, uobject* target_object) {
	if (!this || !source_object || !target_object) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.BaseTeamComponent.IsAlly"));

	struct {
		uobject* source_object;
		uobject* target_object;
		bool return_value;
	} params = { source_object, target_object };

	process_event(fn, &params);
	return params.return_value;
}

//

fhealth_value udamageable_component::cached_life() {
	if (!this) return {};

	return memory::read<fhealth_value>(reinterpret_cast<long long>(this) + offset);
}

bool udamageable_component::is_alive() {
	if (!this) return false;

	return memory::read<bool>(reinterpret_cast<long long>(this) + offset);
}

tarray<aares_player_state*> aares_player_state::get_players_spectating() {
	if (!this) return {};

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ShooterPlayerState.GetPlayersSpectating"));

	struct {
		bool omit_coaches;
		tarray<aares_player_state*> return_value;
	} params = { false };

	process_event(fn, &params);
	return params.return_value;
}

//

uequippable_skin_data_asset* ubase_inventory_model::data_asset() {
	if (!this) return nullptr;

	return reinterpret_cast<uequippable_skin_data_asset*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

void ubase_inventory_model::unlock_item() {
	if (!this) return;

	memory::write<bool>(reinterpret_cast<long long>(this) + offset, true);
	memory::write<bool>(reinterpret_cast<long long>(this) + offset2, true);
	memory::write<bool>(reinterpret_cast<long long>(this) + offset3, true);
}

//

uequippable_skin_chroma_inv_model* uequippable_skin_inv_model::equipped_chroma() {
	if (!this) return nullptr;

	return reinterpret_cast<uequippable_skin_chroma_inv_model*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

tarray<uequippable_skin_chroma_inv_model*> uequippable_skin_inv_model::possible_chromas() {
	if (!this) return {};

	return memory::read<tarray<uequippable_skin_chroma_inv_model*>>(reinterpret_cast<long long>(this) + offset);
}

tarray<uequippable_skin_lvl_inv_model*> uequippable_skin_inv_model::possible_levels() {
	if (!this) return {};

	return memory::read<tarray<uequippable_skin_lvl_inv_model*>>(reinterpret_cast<long long>(this) + offset);
}

//

uequippable_charm_data_asset* uequippable_charm_inst_inv_model::data_asset() {
	if (!this) return nullptr;

	return reinterpret_cast<uequippable_charm_data_asset*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

//

uequippable_skin_inv_model* uequippable_inv_model::equipped_model() {
	if (!this) return nullptr;

	return reinterpret_cast<uequippable_skin_inv_model*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uequippable_charm_inst_inv_model* uequippable_inv_model::charm_model() {
	if (!this) return nullptr;

	return reinterpret_cast<uequippable_charm_inst_inv_model*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

tarray<uequippable_skin_inv_model*> uequippable_inv_model::possible_skins() {
	if (!this) return {};


	return memory::read<tarray<uequippable_skin_inv_model*>>(reinterpret_cast<long long>(this) + offset);
}

tarray<uequippable_attach_inv_model*> uequippable_inv_model::possible_attachments() {
	if (!this) return {};


	return memory::read<tarray<uequippable_attach_inv_model*>>(reinterpret_cast<long long>(this) + offset);
}

//

tarray<uequippable_inv_model*> uares_inventory_manager::equippable_models() {
	if (!this) return {};


	return memory::read<tarray<uequippable_inv_model*>>(reinterpret_cast<long long>(this) + offset);
}

tarray<tmap<uequippable_charm_data_asset*, uequippable_charm_inst_inv_model*>> uares_inventory_manager::data_asset_to_equippable_charm_map() {
	if (!this) return {};

	return memory::read<tarray<tmap<uequippable_charm_data_asset*, uequippable_charm_inst_inv_model*>>>(reinterpret_cast<long long>(this) + offset);
}

//

tarray<uequippable_inv_model*> uarsenal_view_model::gun_models() {
	if (!this) return {};


	return memory::read<tarray<uequippable_inv_model*>>(reinterpret_cast<long long>(this) + offset);
}

//

uarsenal_view_model* uarsenal_view_controller::view_model() {
	if (!this) return nullptr;

	return reinterpret_cast<uarsenal_view_model*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

//

void uthreaded_chat_manager::send_chat_message_v2(uint8_t room, ftext message) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ThreadedChatManager.SendChatMessageV2"));

	struct {
		uint8_t room;
		ftext message;
	} params = { room, message };

	process_event(fn, &params);
}

//

ugame_flow_state::egame_flow_state_type ugame_flow_state::get_flow_state_type() {
	if (!this) return {};

	return memory::read<egame_flow_state_type>(reinterpret_cast<long long>(this) + 0x80);
}

void ugame_flow_state::set_flow_state_type(egame_flow_state_type state_type) {
	if (!this) return;

	return memory::write<egame_flow_state_type>(reinterpret_cast<long long>(this) + 0x80, state_type);
}

//

ugame_flow_state* ugame_flow_state_manager::current_state() {
	if (!this) return nullptr;


	return reinterpret_cast<ugame_flow_state*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

//

uares_inventory_manager* uares_cl_game_instance::inventory_manager() {
	if (!this) return nullptr;

	return reinterpret_cast<uares_inventory_manager*>(memory::read<long long>(reinterpret_cast<long long>(this) + 0x3D8));
}

uarsenal_view_controller* uares_cl_game_instance::view_controller() {
	if (!this) return nullptr;

	return reinterpret_cast<uarsenal_view_controller*>(memory::read<long long>(reinterpret_cast<long long>(this) + 0x560));
}

uthreaded_chat_manager* uares_cl_game_instance::threaded_chat_manager() {
	if (!this) return nullptr;

	return reinterpret_cast<uthreaded_chat_manager*>(memory::read<long long>(reinterpret_cast<long long>(this) + 0x308));
}

ugame_flow_state_manager* uares_cl_game_instance::game_flow_state_manager() {
	if (!this) return nullptr;

	return reinterpret_cast<ugame_flow_state_manager*>(memory::read<long long>(reinterpret_cast<long long>(this) + 0x360));
}

//

bool upregame_view_model::is_locked_in() {
	if (!this) return false;

	return memory::read<bool>(reinterpret_cast<long long>(this) + offset);
}

//

upregame_view_model* apregame_view_controller::view_model() {
	if (!this) return nullptr;


	return reinterpret_cast<upregame_view_model*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

void apregame_view_controller::lock_character(uobject* character) {
	if (!this || !character) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.PregameViewController.LockCharacter"));

	struct {
		uobject* character;
	} params = { character };

	process_event(fn, &params);
}


//

apregame_view_controller* aares_player_controller::pregame_view_controller() {
	if (!this) return nullptr;

	return reinterpret_cast<apregame_view_controller*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

void aares_player_controller::toggle_shop() {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.AresPlayerController.ToggleShop"));

	process_event(fn, nullptr);
}

void aares_player_controller::disconnect_from_server() {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.AresPlayerController.DisconnectFromServer"));

	process_event(fn, nullptr);
}

void aares_player_controller::set_fov(float val) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.AresPlayerController.SetFOV"));

	process_event(fn, &val);
}

bool aares_player_controller::is_shop_open() {
	if (!this) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.AresPlayerController.IsShopOpen"));

	struct {
		bool return_value;
	} params;

	process_event(fn, &params);
	return params.return_value;
}

//

void ublind_manager_component::client_cleanse_blinds() {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.BlindManagerComponent.ClientCleanseBlinds"));

	process_event(fn, nullptr);
}

bool ublind_manager_component::is_blinded() {
	if (!this) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.BlindManagerComponent.IsBlinded"));

	struct {
		bool return_value;
	} params;

	process_event(fn, &params);
	return params.return_value;
}

//


void ushooter_character_death_reaction_component::set_death_montage_effect_override(aeffect_container* effect) {
	if (!this) return;

	memory::write<aeffect_container*>(reinterpret_cast<long long>(this) + offset, effect);
}

void ushooter_character_death_reaction_component::set_death_montage_effect_override_context(uobject* context) {
	if (!this) return;

	memory::write<uobject*>(reinterpret_cast<long long>(this) + offset, context);
}

void ushooter_character_death_reaction_component::play_finisher_effect() {
	if (!this) return;

	typedef void(__fastcall* play_finisher_effect)(ushooter_character_death_reaction_component*);
	auto play_finisher_effect_fn = (play_finisher_effect)(globals::base_address + globals::play_finisher_effect);
	spoof_call(play_finisher_effect_fn, this);
}

//

uares_inventory* ashooter_character::inventory() {
	if (!this) return nullptr;


	return reinterpret_cast<uares_inventory*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

udamageable_component* ashooter_character::damage_handler() {
	if (!this) return nullptr;

	return reinterpret_cast<udamageable_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uskeletal_mesh_component* ashooter_character::mesh_1p() {
	if (!this) return nullptr;

	return reinterpret_cast<uskeletal_mesh_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uskeletal_mesh_component* ashooter_character::mesh_overlay_1p() {
	if (!this) return nullptr;

	return reinterpret_cast<uskeletal_mesh_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uskeletal_mesh_component* ashooter_character::mesh_cosmetic_3p() {
	if (!this) return nullptr;

	return reinterpret_cast<uskeletal_mesh_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

ublind_manager_component* ashooter_character::blind_manager_component() {
	if (!this) return nullptr;

	return reinterpret_cast<ublind_manager_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

ashooter_character* ashooter_character::simple_trace_from_camera(float distance, ecollision_channel channel) {
	if (!this) return nullptr;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ShooterCharacter.SimpleTraceFromCamera"));

	struct {
		float distance;
		ecollision_channel channel;
		fvector hit_point;
		bool blocking_hit;
		ashooter_character* return_value;
	} params = { distance, channel };

	process_event(fn, &params);
	return params.return_value;
}

ushooter_character_death_reaction_component* ashooter_character::death_reaction_component() {
	if (!this) return nullptr;

	return reinterpret_cast<ushooter_character_death_reaction_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

tarray<uprimitive_component*> ashooter_character::default_primitive_components() {
	if (!this) return {};

	return memory::read<tarray<uprimitive_component*>>(reinterpret_cast<long long>(this) + offset);
}

frotator ashooter_character::view_rotation_no_recoil() {
	if (!this) return {};

	frotator output{};
	typedef frotator* (__fastcall* view_rotation_no_recoil)(ashooter_character*, frotator*);
	auto view_rotation_no_recoil_fn = (view_rotation_no_recoil)(globals::base_address + globals::view_rotation_no_recoil);
	auto return_value = spoof_call(view_rotation_no_recoil_fn, this, &output);
	return *return_value;
}

frotator ashooter_character::view_rotation_full_recoil() {
	if (!this) return {};

	frotator output{};
	typedef frotator* (__fastcall* view_rotation_full_recoil)(ashooter_character*, frotator*);
	auto view_rotation_full_recoil_fn = (view_rotation_full_recoil)(globals::base_address + globals::view_rotation_full_recoil);
	auto return_value = spoof_call(view_rotation_full_recoil_fn, this, &output);
	return *return_value;
}

void ashooter_character::set_vector_on_mids(fname name, flinearcolor value) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ShooterCharacter.SetVectorOnMIDs"));

	struct {
		fname name;
		flinearcolor value;
	} params = { name, value };

	process_event(fn, &params);
}

void ashooter_character::set_scalar_on_mids(fname name, float value) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ShooterCharacter.SetScalarOnMIDs"));

	struct {
		fname name;
		float value;
	} params = { name, value };

	process_event(fn, &params);
}

void ashooter_character::reset_character_materials(uskeletal_mesh_component* mesh_to_reset) {
	if (!this || !mesh_to_reset) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ShooterCharacter.ResetCharacterMaterials"));

	struct {
		uskeletal_mesh_component* mesh_to_reset;
	} params = { mesh_to_reset };

	process_event(fn, &params);
}

void ashooter_character::set_3p_mesh_visible(bool val) {
	if (!this) return;

	memory::write<char>(reinterpret_cast<long long>(this) + 0xF20, val);
	if (memory::read<char>(reinterpret_cast<long long>(this) + 0xF22))
		memory::write<char>(reinterpret_cast<long long>(this) + 0xF22, 0);
}

void ashooter_character::set_crouch_time(float val) {
	if (!this) return;

	memory::write<float>(reinterpret_cast<long long>(this) + offset, val);
}

void ashooter_character::set_is_in_spawn_zone(bool is_in_spawn) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ShooterCharacter.SetIsInSpawnZone"));

	struct {
		bool is_in_spawn;
	} params = { is_in_spawn };

	process_event(fn, &params);
}

bool ashooter_character::is_still_firing() {
	if (!this) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ShooterCharacter.IsStillFiring"));

	struct {
		bool return_value;
	} params;

	process_event(fn, &params);
	return params.return_value;
}

bool ashooter_character::get_firing_data(fvector* location, fvector* rotation, bool apply_recoil) {
	if (!this) return false;

	typedef bool(__fastcall* get_firing_location)(ashooter_character*, fvector*, fvector*, bool);
	auto get_firing_location_fn = (get_firing_location)(globals::base_address + globals::get_firing_location);
	return spoof_call(get_firing_location_fn, this, location, rotation, apply_recoil);
}

bool ashooter_character::get_is_in_spawn_zone() {
	if (!this) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ShooterCharacter.GetIsInSpawnZone"));

	struct {
		bool return_value;
	} params;

	process_event(fn, &params);
	return params.return_value;
}

//

uares_cl_game_instance* uares_game_instance::client_game_instance() {
	if (!this) return nullptr;

	return reinterpret_cast<uares_cl_game_instance*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

//

void uares_outline_rendering::set_outline_colors_for_render(uobject* world_context, flinearcolor ally_color, flinearcolor enemy_color) {
	if (!world_context) return;

	typedef void(__fastcall* set_outline_colors_for_render)(uobject*, flinearcolor, flinearcolor);
	auto set_outline_colors_for_render_fn = (set_outline_colors_for_render)(globals::base_address + globals::set_outline_colors_for_render);
	spoof_call(set_outline_colors_for_render_fn, world_context, ally_color, enemy_color);
}

//

bool ares_characters_function_library::has_line_of_sight_to_character(uobject* viewer, uobject* target, ecollision_channel trace_channel, uobject* world_context, fvector focus_point) {
	if (!this || !viewer || !target || !world_context) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Ares_Characters_FunctionLibrary.Ares_Characters_FunctionLibrary_C.HasLineOfSightToCharacter"));

	struct {
		uobject* viewer;
		uobject* target;
		ecollision_channel trace_channel;
		uobject* world_context;
		bool return_value;
		fvector focus_point;
	} params = { viewer, target, trace_channel, world_context };

	process_event(fn, &params);
	return params.return_value;
}

//

bool aplanted_bomb_c::bomb_has_exploded() {
	if (!this) return false;


	return memory::read<bool>(reinterpret_cast<long long>(this) + offset);
}

bool aplanted_bomb_c::bomb_has_been_defused() {
	if (!this) return false;


	return memory::read<bool>(reinterpret_cast<long long>(this) + offset);
}

float aplanted_bomb_c::defuse_progress() {
	if (!this) return 0.0f;


	return memory::read<float>(reinterpret_cast<long long>(this) + offset);
}

float aplanted_bomb_c::time_remaining_to_explode() {
	if (!this) return 0.0f;

	return memory::read<float>(reinterpret_cast<long long>(this) + offset);
}

//

aares_equippable* aares_on_ground_equippable::my_equippable() {
	if (!this) return nullptr;


	return memory::read<aares_equippable*>(reinterpret_cast<long long>(this) + offset);
}

//

bool ushooter_blueprint_library::is_bone_visible(fname bone_name, fvector camera_location, aactor* viewee, uskeletal_mesh_component* skeletal_mesh_component, ecollision_channel channel_type) {
	if (!this || !viewee || !skeletal_mesh_component) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.ShooterBlueprintLibrary.IsBoneVisible"));

	struct {
		fname bone_name;
		fvector camera_location;
		aactor* viewee;
		uskeletal_mesh_component* skeletal_mesh_component;
		ecollision_channel channel_type;
		bool return_value;
	} params = { bone_name, camera_location, viewee, skeletal_mesh_component, channel_type };

	process_event(fn, &params);
	return params.return_value;
}

//

apawn* udamage_response::get_event_instigator_pawn() {
	if (!this) return nullptr;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"ShooterGame.DamageResponse.GetEventInstigatorPawn"));

	struct {
		apawn* return_value;
	} params;

	process_event(fn, &params);
	return params.return_value;
}