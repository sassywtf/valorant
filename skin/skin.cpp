#include "skin.hpp"

void skin_mgr::unlock_all(uworld* world) {
	auto game_instance = (uares_game_instance*)world->game_instance();
	if (!game_instance) return;

	auto client_game_instance = game_instance->client_game_instance();
	if (!client_game_instance) return;

	auto inventory_manager = client_game_instance->inventory_manager();
	if (!inventory_manager) return;

	auto equippable_models = inventory_manager->equippable_models();
	for (int i = 0; i < equippable_models.size(); i++) {
		if (auto model = equippable_models[i]) {
			// skins related
			auto skins = model->possible_skins();
			for (int k = 0; k < skins.size(); k++) {
				if (auto skin = skins[k]) {
					// unlock levels
					auto possible_levels = skin->possible_levels();
					for (int a = 0; a < possible_levels.size(); a++)
						if (auto level = possible_levels[a])
							level->unlock_item();

					// unlock chromas
					auto possible_chromas = skin->possible_chromas();
					for (int a = 0; a < possible_chromas.size(); a++)
						if (auto chroma = possible_chromas[a])
							chroma->unlock_item();

					// unlock skin
					skin->unlock_item();
				}
			}

			// attachments related
			auto attachments = model->possible_attachments();
			for (int k = 0; k < attachments.size(); k++) {
				if (auto attachment = attachments[k]) {
					// unlock attachment
					attachment->unlock_item();
				}
			}
		}
	}
}

uequippable_inv_model* skin_mgr::get_gun_model(uares_game_instance* game_instance, int index) {
	auto client_game_instance = game_instance->client_game_instance();
	if (!client_game_instance) return nullptr;

	auto view_controller = client_game_instance->view_controller();
	if (!view_controller) return nullptr;

	auto view_model = view_controller->view_model();
	if (!view_model) return nullptr;

	auto gun_models = view_model->gun_models();
	for (int i = 0; i < gun_models.size(); i++) {
		auto model = gun_models[i];
		if (!model) continue;

		auto equipped_model = model->equipped_model();
		if (!equipped_model) continue;

		auto data_asset = equipped_model->data_asset();
		if (!data_asset) continue;

		if (auto type = data_asset->get_type(); type.is_valid())
			if (type.index == index)
				return model;
	}

	return nullptr;
}

void skin_mgr::skin_changer(uares_game_instance* game_instance, ashooter_character* pawn) {
	if (!m_config.skins.skin_changer) return;
	if (!spoof_call(import_(GetAsyncKeyState), VK_F7) && !m_config.skins.auto_apply_skin) return;

	auto client_game_instance = game_instance->client_game_instance();
	if (!client_game_instance) return;

	auto flow_state_manager = client_game_instance->game_flow_state_manager();
	if (!flow_state_manager) return;

	auto inventory = pawn->inventory();
	if (!inventory) return;

	auto current_equippable = inventory->current_equippable();
	if (!current_equippable) return;

	auto equippable_type = current_equippable->get_type();
	if (!equippable_type.is_valid()) return;

	auto skin_data = current_equippable->skin_data_asset();
	if (!skin_data) return;

	auto model = get_gun_model(game_instance, equippable_type.index);
	if (!model) return;

	auto skin_model = model->equipped_model();
	if (!skin_model) return;

	auto skin_asset = skin_model->data_asset();
	auto chroma_asset = skin_model->equipped_chroma()->data_asset();
	auto max_level = skin_model->data_asset()->skin_levels().size();
	auto charm_asset = current_equippable->charm_data_asset();

	if (!skin_asset || !chroma_asset) return;

	std::string real_skin_name = system::static_class()->get_object_name(skin_data);
	std::wstring wreal_skin_name(real_skin_name.begin(), real_skin_name.end());

	if (skin_asset == current_equippable->skin_data_asset() ||
		!string::static_class()->contains(wreal_skin_name.c_str(), _(L"standard")))
		return;

	current_equippable->set_skin_data_asset(skin_asset);
	current_equippable->set_chroma_data_asset(chroma_asset);

	auto current_state = flow_state_manager->current_state();
	if (!current_state) return;

	auto old_flow_state = current_state->get_flow_state_type();
	current_state->set_flow_state_type(ugame_flow_state::egame_flow_state_type::main_menu);

	ucontent_library::static_class()->clear_weapon_components(current_equippable);
	ucontent_library::static_class()->apply_skin(current_equippable, skin_asset, chroma_asset, max_level, charm_asset, equippable_type.index == 18 ? -1 : 1);

	current_state->set_flow_state_type(old_flow_state);

	if (!m_config.skins.custom_skins) return;

	auto material = uobject::load_object(_(L"/Game/Equippables/Guns/Rifles/AK/Winter/Materials/AK_Winter_MI.AK_Winter_MI"));
	if (!material) return;

	utexture2d* imported_texture = nullptr;

	if (m_config.skins.skin_id == 0)
		imported_texture = rendering::static_class()->import_file_as_texture_2d((uobject*)globals::cached_world, L"C:\\valorant1.png");
	else if (m_config.skins.skin_id == 1)
		imported_texture = rendering::static_class()->import_file_as_texture_2d((uobject*)globals::cached_world, L"C:\\valorant2.png");
	else if (m_config.skins.skin_id == 2)
		imported_texture = rendering::static_class()->import_file_as_texture_2d((uobject*)globals::cached_world, L"C:\\valorant3.png");

	if (!imported_texture) return;

	std::string equippable_name = system::static_class()->get_object_name(current_equippable);
	std::wstring wequippable_name(equippable_name.begin(), equippable_name.end());

	if (!string::static_class()->contains(wequippable_name.c_str(), _(L"ak"))) return;

	auto custom = material_library::static_class()->create_dynamic_material_instance((uobject*)globals::cached_world, material, fname(), material_library::emid_creation_flags::none);
	if (!custom) return;

	custom->set_texture_parameter_value(string::static_class()->string_to_name(_(L"Image 1")), imported_texture);
	custom->set_texture_parameter_value(string::static_class()->string_to_name(_(L"Image 2")), imported_texture);

	if (auto cosmetic_mesh_1p = current_equippable->cosmetic_mesh_1p()) {
		auto cosmetic_mesh_1p_materials = cosmetic_mesh_1p->get_num_materials();
		for (int k = 0; k < cosmetic_mesh_1p_materials; k++) {
			cosmetic_mesh_1p->set_material(k, custom);
		}
	}

	if (auto mesh_1p = current_equippable->mesh_1p()) {
		auto mesh_1p_materials = mesh_1p->get_num_materials();
		for (int k = 0; k < mesh_1p_materials; k++) {
			mesh_1p->set_material(k, custom);
		}
	}

	if (auto mesh_3p = current_equippable->mesh_3p()) {
		auto mesh_3p_materials = mesh_3p->get_num_materials();
		for (int k = 0; k < mesh_3p_materials; k++) {
			mesh_3p->set_material(k, custom);
		}
	}

	if (auto magazine_1p = current_equippable->magazine_1p()) {
		auto magazine_1p_materials = magazine_1p->get_num_materials();
		for (int k = 0; k < magazine_1p_materials; k++) {
			magazine_1p->set_material(k, custom);
		}
	}

	if (auto secondary_magazine_1p = current_equippable->secondary_magazine_1p()) {
		auto secondary_magazine_1p_materials = secondary_magazine_1p->get_num_materials();
		for (int k = 0; k < secondary_magazine_1p_materials; k++) {
			secondary_magazine_1p->set_material(k, custom);
		}
	}

	if (auto magazine_3p = current_equippable->magazine_3p()) {
		auto magazine_3p_materials = magazine_3p->get_num_materials();
		for (int k = 0; k < magazine_3p_materials; k++) {
			magazine_3p->set_material(k, custom);
		}
	}
}

void skin_mgr::buddy_changer(uares_game_instance* game_instance, ashooter_character* pawn) {
	if (!m_config.skins.buddy_changer) return;
	if (!spoof_call(import_(GetAsyncKeyState), VK_F7) && !m_config.skins.auto_apply_buddy) return;

	auto client_game_instance = game_instance->client_game_instance();
	if (!client_game_instance) return;

	auto inventory_manager = client_game_instance->inventory_manager();
	if (!inventory_manager) return;

	auto flow_state_manager = client_game_instance->game_flow_state_manager();
	if (!flow_state_manager) return;

	auto inventory = pawn->inventory();
	if (!inventory) return;

	auto current_equippable = inventory->current_equippable();
	if (!current_equippable) return;

	auto equippable_type = current_equippable->get_type();
	if (!equippable_type.is_valid()) return;

	auto charm_data = current_equippable->charm_data_asset();

	auto model = get_gun_model(game_instance, equippable_type.index);
	if (!model) return;

	auto charm_model = model->charm_model();

	auto skin_asset = current_equippable->skin_data_asset();
	auto chroma_asset = current_equippable->chroma_data_asset();
	auto max_level = current_equippable->skin_data_asset()->skin_levels().size();
	auto charm_asset = charm_model->data_asset();

	if (!skin_asset || !chroma_asset) return;

	auto data_asset_to_equippable_charm_map = inventory_manager->data_asset_to_equippable_charm_map();
	if (!data_asset_to_equippable_charm_map.size()) return;

	charm_asset = data_asset_to_equippable_charm_map[m_config.skins.buddy_id].key;

	if (charm_asset == current_equippable->charm_data_asset())
		return;

	current_equippable->set_charm_data_asset(charm_asset);

	auto current_state = flow_state_manager->current_state();
	if (!current_state) return;

	auto old_flow_state = current_state->get_flow_state_type();
	current_state->set_flow_state_type(ugame_flow_state::egame_flow_state_type::main_menu);

	ucontent_library::static_class()->clear_weapon_components(current_equippable);
	ucontent_library::static_class()->apply_skin(current_equippable, skin_asset, chroma_asset, max_level, charm_asset, equippable_type.index == 18 ? -1 : 1);

	current_state->set_flow_state_type(old_flow_state);
}