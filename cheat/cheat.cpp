#include "cheat.hpp"

auto bone_to_screen(aplayer_controller* controller, uskeletal_mesh_component* mesh, int id) {
	fvector2d location2d{};
	if (!controller->world_to_screen(mesh->bone_location(id), &location2d))
		return fvector2d();

	return location2d;
};

void cheat_mgr::boxes2d(aplayer_controller* controller, uskeletal_mesh_component* mesh, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	auto color = *reinterpret_cast<flinearcolor*>(&m_config.colors.invisible_color);
	if (m_config.visual.visible_check && visible)
		color = *reinterpret_cast<flinearcolor*>(&m_config.colors.visible_color);

	auto draw_box = [](fvector2d root, fvector2d head, flinearcolor color) {
		// calculate size
		float height = root.y - head.y;
		float width = height / 1.8f;

		fvector2d top_left = fvector2d(head.x - (width / 2), head.y - (height / 5));
		fvector2d bottom_right = fvector2d(head.x + (width / 2), root.y);

		// up
		game_canvas->draw_line(top_left, fvector2d(bottom_right.x, top_left.y), color);

		// left
		game_canvas->draw_line(fvector2d(top_left.x, bottom_right.y), top_left, color);

		// right
		game_canvas->draw_line(fvector2d(bottom_right.x, top_left.y), bottom_right, color);

		// bottom
		game_canvas->draw_line(bottom_right, fvector2d(top_left.x, bottom_right.y), color);
		};

	auto head = bone_to_screen(controller, mesh, 8);
	auto root = bone_to_screen(controller, mesh, 0);

	if (head.x && head.y && root.x && root.y)
		draw_box(fvector2d(root.x, root.y), fvector2d(head.x, head.y), color);
}

void cheat_mgr::boxes3d(aplayer_controller* controller, ashooter_character* actor, uskeletal_mesh_component* mesh, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	auto color = *reinterpret_cast<flinearcolor*>(&m_config.colors.invisible_color);
	if (m_config.visual.visible_check && visible)
		color = *reinterpret_cast<flinearcolor*>(&m_config.colors.visible_color);

	auto draw_boxes3d = [](fvector2d s1, fvector2d s2, fvector2d s3, fvector2d s4, fvector2d s5, fvector2d s6, fvector2d s7, fvector2d s8, flinearcolor color) {
		// draw normally for each vertex
		game_canvas->draw_line(fvector2d(s1.x, s1.y), fvector2d(s2.x, s2.y), color);
		game_canvas->draw_line(fvector2d(s2.x, s2.y), fvector2d(s3.x, s3.y), color);
		game_canvas->draw_line(fvector2d(s3.x, s3.y), fvector2d(s4.x, s4.y), color);
		game_canvas->draw_line(fvector2d(s4.x, s4.y), fvector2d(s1.x, s1.y), color);

		game_canvas->draw_line(fvector2d(s5.x, s5.y), fvector2d(s6.x, s6.y), color);
		game_canvas->draw_line(fvector2d(s6.x, s6.y), fvector2d(s7.x, s7.y), color);
		game_canvas->draw_line(fvector2d(s7.x, s7.y), fvector2d(s8.x, s8.y), color);
		game_canvas->draw_line(fvector2d(s8.x, s8.y), fvector2d(s5.x, s5.y), color);

		game_canvas->draw_line(fvector2d(s1.x, s1.y), fvector2d(s5.x, s5.y), color);
		game_canvas->draw_line(fvector2d(s2.x, s2.y), fvector2d(s6.x, s6.y), color);
		game_canvas->draw_line(fvector2d(s3.x, s3.y), fvector2d(s7.x, s7.y), color);
		game_canvas->draw_line(fvector2d(s4.x, s4.y), fvector2d(s8.x, s8.y), color);
		};

	// world locations
	fvector origin{}, extends{};
	actor->get_actor_bounds(true, &origin, &extends, true);

	// box extension
	extends.x += 34.f;
	extends.y += 34.f;
	extends.z += 100.f;

	origin -= extends / 2.f;
	origin.y += 5.f;

	fvector one = origin;
	fvector two = origin; two.x += extends.x;
	fvector three = origin; three.x += extends.x; three.y += extends.y;
	fvector four = origin; four.y += extends.y;

	fvector five = one; five.z += extends.z;
	fvector six = two; six.z += extends.z;
	fvector seven = three; seven.z += extends.z;
	fvector eight = four; eight.z += extends.z;

	// w2s
	fvector2d s1{}, s2{}, s3{}, s4{}, s5{}, s6{}, s7{}, s8{};

	if (!controller->world_to_screen(one, &s1) ||
		!controller->world_to_screen(two, &s2) ||
		!controller->world_to_screen(three, &s3) ||
		!controller->world_to_screen(four, &s4))
		return;

	if (!controller->world_to_screen(five, &s5) ||
		!controller->world_to_screen(six, &s6) ||
		!controller->world_to_screen(seven, &s7) ||
		!controller->world_to_screen(eight, &s8))
		return;

	// drawing
	draw_boxes3d(s1, s2, s3, s4, s5, s6, s7, s8, color);
}

void cheat_mgr::boxes_corner(aplayer_controller* controller, uskeletal_mesh_component* mesh, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	auto color = *reinterpret_cast<flinearcolor*>(&m_config.colors.invisible_color);
	if (m_config.visual.visible_check && visible)
		color = *reinterpret_cast<flinearcolor*>(&m_config.colors.visible_color);

	auto draw_corner = [](fvector2d root, fvector2d head, flinearcolor color) {
		// calculate size
		float height = root.y - head.y;
		float width = height / 1.8f;

		fvector2d top_left = fvector2d(head.x - (width / 2), head.y - (height / 5));
		fvector2d bottom_right = fvector2d(head.x + (width / 2), root.y);

		// up
		game_canvas->draw_line(fvector2d(top_left.x, top_left.y), fvector2d(top_left.x + (width / 3), top_left.y), color);
		game_canvas->draw_line(fvector2d(bottom_right.x, top_left.y), fvector2d(bottom_right.x - (width / 3), top_left.y), color);

		// left
		game_canvas->draw_line(fvector2d(top_left.x, top_left.y), fvector2d(top_left.x, top_left.y + (height / 3)), color);
		game_canvas->draw_line(fvector2d(top_left.x, bottom_right.y), fvector2d(top_left.x, bottom_right.y - (height / 3)), color);

		// right
		game_canvas->draw_line(fvector2d(bottom_right.x, top_left.y), fvector2d(bottom_right.x, top_left.y + (height / 3)), color);
		game_canvas->draw_line(fvector2d(bottom_right.x, bottom_right.y), fvector2d(bottom_right.x, bottom_right.y - (height / 3)), color);

		// bottom
		game_canvas->draw_line(fvector2d(top_left.x, bottom_right.y), fvector2d(top_left.x + (width / 3), bottom_right.y), color);
		game_canvas->draw_line(fvector2d(bottom_right.x, bottom_right.y), fvector2d(bottom_right.x - (width / 3), bottom_right.y), color);
		};

	auto head = bone_to_screen(controller, mesh, 8);
	auto root = bone_to_screen(controller, mesh, 0);

	if (head.x && head.y && root.x && root.y)
		draw_corner(fvector2d(root.x, root.y), fvector2d(head.x, head.y), color);
}

void cheat_mgr::viewpoint(aplayer_controller* controller, uskeletal_mesh_component* mesh, uscene_component* root, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	auto color = *reinterpret_cast<flinearcolor*>(&m_config.colors.invisible_color);
	if (m_config.visual.visible_check && visible)
		color = *reinterpret_cast<flinearcolor*>(&m_config.colors.visible_color);

	auto view_angle = root->relative_rotation();
	auto view_angle_vec = fvector(view_angle.pitch, view_angle.yaw, view_angle.roll);
	auto head_pos = mesh->bone_location(8);

	fvector output_angle{};
	math::angle_to_vectors(view_angle_vec, &output_angle);
	output_angle *= 125.f;

	auto final_angle = head_pos + output_angle;

	fvector2d start_pos{}, end_pos{};
	if (!controller->world_to_screen(head_pos, &start_pos)) return;
	if (!controller->world_to_screen(final_angle, &end_pos)) return;

	game_canvas->draw_line(start_pos, end_pos, color);
}

void cheat_mgr::snapline(aplayer_controller* controller, uskeletal_mesh_component* mesh, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	auto color = *reinterpret_cast<flinearcolor*>(&m_config.colors.invisible_color);
	if (m_config.visual.visible_check && visible)
		color = *reinterpret_cast<flinearcolor*>(&m_config.colors.visible_color);

	auto head = bone_to_screen(controller, mesh, 8);

	float x3, y3;
	float radius = 50.f;
	auto a = head.y - (float)game_canvas->size().y / 2.f;
	auto b = head.x - (float)game_canvas->size().x / 2.f;
	auto angle = atan(a / b);

	if (b >= 0) {
		x3 = radius * cos(angle) + (float)game_canvas->size().x / 2.f;
		y3 = radius * sin(angle) + (float)game_canvas->size().y / 2.f;
	}
	else {
		x3 = radius * (-cos(angle)) + (float)game_canvas->size().x / 2.f;
		y3 = radius * (-sin(angle)) + (float)game_canvas->size().y / 2.f;
	}

	if (sqrt(a * a + b * b) >= radius && head.x && head.y) {
		game_canvas->draw_line(fvector2d(x3, y3), head, color);
	}
}

void cheat_mgr::skeleton(aplayer_controller* controller, uskeletal_mesh_component* mesh, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	bool should_pass = true;

	int head_pos_id{}, neck_pos_id{};
	int l_upper_arm_pos_id{}, r_upper_arm_pos_id{};
	int l_fore_arm_pos_id{}, r_fore_arm_pos_id{};
	int l_hand_pos_id{}, r_hand_pos_id{};
	int l_thigh_pos_id{}, r_thigh_pos_id{};
	int l_knee_pos_id{}, r_knee_pos_id{};
	int l_foot_pos_id{}, r_foot_pos_id{};

	switch (mesh->num_bones())
	{
	case 104: // male
		head_pos_id = 8;
		neck_pos_id = 21;

		l_upper_arm_pos_id = 23;
		l_fore_arm_pos_id = 24;
		l_hand_pos_id = 25;

		r_upper_arm_pos_id = 49;
		r_fore_arm_pos_id = 50;
		r_hand_pos_id = 51;

		l_thigh_pos_id = 77;
		l_knee_pos_id = 78;
		l_foot_pos_id = 80;

		r_thigh_pos_id = 84;
		r_knee_pos_id = 85;
		r_foot_pos_id = 87;
		break;
	case 101: // female
		head_pos_id = 8;
		neck_pos_id = 21;

		l_upper_arm_pos_id = 23;
		l_fore_arm_pos_id = 24;
		l_hand_pos_id = 25;

		r_upper_arm_pos_id = 49;
		r_fore_arm_pos_id = 50;
		r_hand_pos_id = 51;

		l_thigh_pos_id = 75;
		l_knee_pos_id = 76;
		l_foot_pos_id = 78;

		r_thigh_pos_id = 82;
		r_knee_pos_id = 83;
		r_foot_pos_id = 85;
		break;
	case 103: // bot
		head_pos_id = 8;
		neck_pos_id = 9;

		l_upper_arm_pos_id = 33;
		l_fore_arm_pos_id = 30;
		l_hand_pos_id = 32;

		r_upper_arm_pos_id = 58;
		r_fore_arm_pos_id = 55;
		r_hand_pos_id = 57;

		l_thigh_pos_id = 63;
		l_knee_pos_id = 65;
		l_foot_pos_id = 69;

		r_thigh_pos_id = 77;
		r_knee_pos_id = 79;
		r_foot_pos_id = 83;
		break;
	default:
		should_pass = false;
		break;
	}

	if (!should_pass) return;

	auto head_pos = mesh->bone_location(head_pos_id);
	auto head_screen = controller->world_to_screen_ret(head_pos);

	auto neck_pos = mesh->bone_location(neck_pos_id);
	auto neck_screen = controller->world_to_screen_ret(neck_pos);

	auto l_upper_arm_pos = mesh->bone_location(l_upper_arm_pos_id);
	auto l_upper_arm_screen = controller->world_to_screen_ret(l_upper_arm_pos);

	auto l_fore_arm_pos = mesh->bone_location(l_fore_arm_pos_id);
	auto l_fore_arm_screen = controller->world_to_screen_ret(l_fore_arm_pos);

	auto l_hand_pos = mesh->bone_location(l_hand_pos_id);
	auto l_hand_screen = controller->world_to_screen_ret(l_hand_pos);

	auto r_upper_arm_pos = mesh->bone_location(r_upper_arm_pos_id);
	auto r_upper_arm_screen = controller->world_to_screen_ret(r_upper_arm_pos);

	auto r_fore_arm_pos = mesh->bone_location(r_fore_arm_pos_id);
	auto r_fore_arm_screen = controller->world_to_screen_ret(r_fore_arm_pos);

	auto r_hand_pos = mesh->bone_location(r_hand_pos_id);
	auto r_hand_screen = controller->world_to_screen_ret(r_hand_pos);

	auto l_thigh_pos = mesh->bone_location(l_thigh_pos_id);
	auto l_thigh_screen = controller->world_to_screen_ret(l_thigh_pos);

	auto l_knee_pos = mesh->bone_location(l_knee_pos_id);
	auto l_knee_screen = controller->world_to_screen_ret(l_knee_pos);

	auto l_foot_pos = mesh->bone_location(l_foot_pos_id);
	auto l_foot_screen = controller->world_to_screen_ret(l_foot_pos);

	auto r_thigh_pos = mesh->bone_location(r_thigh_pos_id);
	auto r_thigh_screen = controller->world_to_screen_ret(r_thigh_pos);

	auto r_knee_pos = mesh->bone_location(r_knee_pos_id);
	auto r_knee_screen = controller->world_to_screen_ret(r_knee_pos);

	auto r_foot_pos = mesh->bone_location(r_foot_pos_id);
	auto r_foot_screen = controller->world_to_screen_ret(r_foot_pos);

	auto pelvis_pos = mesh->bone_location(3);
	auto pelvis_screen = controller->world_to_screen_ret(pelvis_pos);

	const auto visible_check = [](aplayer_controller* controller, fvector first_pos, fvector second_pos) {
		auto color = *reinterpret_cast<flinearcolor*>(&m_config.colors.invisible_color);

		if (!m_config.visual.visible_check) return color;

		auto local_pawn = controller->pawn();
		if (!local_pawn) return color;

		auto camera_manager = controller->camera_manager();
		if (!camera_manager) return color;

		auto camera_location = camera_manager->camera_location();

		auto first_result = system::static_class()->line_trace_single((uobject*)globals::cached_world, local_pawn, camera_location, first_pos);
		auto second_result = system::static_class()->line_trace_single((uobject*)globals::cached_world, local_pawn, camera_location, second_pos);

		if (!first_result.actor && !first_result.blocking_hit && !second_result.actor && !second_result.blocking_hit)
			color = *reinterpret_cast<flinearcolor*>(&m_config.colors.visible_color);

		return color;
		};

	if (neck_screen.x && neck_screen.y && l_upper_arm_screen.x && l_upper_arm_screen.y) {
		auto color = visible_check(controller, neck_pos, l_upper_arm_pos);
		game_canvas->draw_line(neck_screen, l_upper_arm_screen, color);
	}

	if (neck_screen.x && neck_screen.y && r_upper_arm_screen.x && r_upper_arm_screen.y) {
		auto color = visible_check(controller, neck_pos, r_upper_arm_pos);
		game_canvas->draw_line(neck_screen, r_upper_arm_screen, color);
	}

	if (l_fore_arm_screen.x && l_fore_arm_screen.y && l_upper_arm_screen.x && l_upper_arm_screen.y) {
		auto color = visible_check(controller, l_fore_arm_pos, l_upper_arm_pos);
		game_canvas->draw_line(l_fore_arm_screen, l_upper_arm_screen, color);
	}

	if (r_fore_arm_screen.x && r_fore_arm_screen.y && r_upper_arm_screen.x && r_upper_arm_screen.y) {
		auto color = visible_check(controller, r_fore_arm_pos, r_upper_arm_pos);
		game_canvas->draw_line(r_fore_arm_screen, r_upper_arm_screen, color);
	}

	if (l_fore_arm_screen.x && l_fore_arm_screen.y && l_hand_screen.x && l_hand_screen.y) {
		auto color = visible_check(controller, l_fore_arm_pos, l_hand_pos);
		game_canvas->draw_line(l_fore_arm_screen, l_hand_screen, color);
	}

	if (r_fore_arm_screen.x && r_fore_arm_screen.y && r_hand_screen.x && r_hand_screen.y) {
		auto color = visible_check(controller, r_fore_arm_pos, r_hand_pos);
		game_canvas->draw_line(r_fore_arm_screen, r_hand_screen, color);
	}

	if (neck_screen.x && neck_screen.y && pelvis_screen.x && pelvis_screen.y) {
		auto color = visible_check(controller, neck_pos, pelvis_pos);
		game_canvas->draw_line(neck_screen, pelvis_screen, color);
	}

	if (r_thigh_screen.x && r_thigh_screen.y && pelvis_screen.x && pelvis_screen.y) {
		auto color = visible_check(controller, r_thigh_pos, pelvis_pos);
		game_canvas->draw_line(r_thigh_screen, pelvis_screen, color);
	}

	if (l_thigh_screen.x && l_thigh_screen.y && pelvis_screen.x && pelvis_screen.y) {
		auto color = visible_check(controller, l_thigh_pos, pelvis_pos);
		game_canvas->draw_line(l_thigh_screen, pelvis_screen, color);
	}

	if (l_thigh_screen.x && l_thigh_screen.y && l_knee_screen.x && l_knee_screen.y) {
		auto color = visible_check(controller, l_thigh_pos, l_knee_pos);
		game_canvas->draw_line(l_thigh_screen, l_knee_screen, color);
	}

	if (r_thigh_screen.x && r_thigh_screen.y && r_knee_screen.x && r_knee_screen.y) {
		auto color = visible_check(controller, r_thigh_pos, r_knee_pos);
		game_canvas->draw_line(r_thigh_screen, r_knee_screen, color);
	}

	if (l_foot_screen.x && l_foot_screen.y && l_knee_screen.x && l_knee_screen.y) {
		auto color = visible_check(controller, l_knee_pos, l_knee_pos);
		game_canvas->draw_line(l_foot_screen, l_knee_screen, color);
	}

	if (r_foot_screen.x && r_foot_screen.y && r_knee_screen.x && r_knee_screen.y) {
		auto color = visible_check(controller, r_knee_pos, r_knee_pos);
		game_canvas->draw_line(r_foot_screen, r_knee_screen, color);
	}
}

void cheat_mgr::head(aplayer_controller* controller, uskeletal_mesh_component* mesh, uscene_component* root, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	auto color = flinearcolor(m_config.colors.invisible_color[0], m_config.colors.invisible_color[1], m_config.colors.invisible_color[2], 0.025f);
	if (m_config.visual.visible_check && visible)
		color = flinearcolor(m_config.colors.visible_color[0], m_config.colors.visible_color[1], m_config.colors.visible_color[2], 0.025f);

	auto head_position = bone_to_screen(controller, mesh, 8);
	if (head_position.x && head_position.y) {
		fvector local_relative{}, enemy_relative{};

		auto local_pawn = (ashooter_character*)controller->pawn();
		if (local_pawn) {
			auto local_root = local_pawn->root_component();
			if (local_root) {
				local_relative = local_root->relative_location();
			}
		}

		enemy_relative = root->relative_location();

		game_canvas->draw_circle(head_position, 9000.f / local_relative.distance(enemy_relative), color);
	}
}

void cheat_mgr::name(aplayer_controller* controller, ashooter_character* actor, uskeletal_mesh_component* mesh, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	auto color = flinearcolor(m_config.colors.invisible_color[0], m_config.colors.invisible_color[1], m_config.colors.invisible_color[2], 1.0f);
	if (m_config.visual.visible_check && visible)
		color = flinearcolor(m_config.colors.visible_color[0], m_config.colors.visible_color[1], m_config.colors.visible_color[2], 1.0f);

	auto player_state = actor->player_state();
	if (!player_state) return;

	auto name = player_state->get_player_name().c_str();
	auto head = bone_to_screen(controller, mesh, 8);
	auto root = bone_to_screen(controller, mesh, 0);

	auto box_height = root.y - head.y;
	auto box_width = box_height / 1.8f;
	auto top_center = fvector2d(head.x, head.y - (box_width / 2.f));

	if (box_height)
		game_canvas->draw_text(name, fvector2d(top_center.x, top_center.y - 30.f), color, true, false, false);
}

void cheat_mgr::health(aplayer_controller* controller, uskeletal_mesh_component* mesh, float life, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	auto color = flinearcolor(m_config.colors.invisible_color[0], m_config.colors.invisible_color[1], m_config.colors.invisible_color[2], 1.0f);
	if (m_config.visual.visible_check && visible)
		color = flinearcolor(m_config.colors.visible_color[0], m_config.colors.visible_color[1], m_config.colors.visible_color[2], 1.0f);

	auto health = std::wstring(std::to_wstring((int)life).c_str()).append(_(L"HP"));
	auto head = bone_to_screen(controller, mesh, 8);
	auto root = bone_to_screen(controller, mesh, 0);

	auto box_height = root.y - head.y;
	auto box_width = box_height / 1.8f;
	auto top_center = fvector2d(head.x, head.y - (box_width / 2.f));

	if (box_height)
		game_canvas->draw_text(health.c_str(), fvector2d(top_center.x, top_center.y - (m_config.visual.name ? 45.f : 30.f)), color, true, false, false);
}

void cheat_mgr::weapon(aplayer_controller* controller, ashooter_character* actor, uskeletal_mesh_component* mesh, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	auto color = flinearcolor(m_config.colors.invisible_color[0], m_config.colors.invisible_color[1], m_config.colors.invisible_color[2], 1.0f);
	if (m_config.visual.visible_check && visible)
		color = flinearcolor(m_config.colors.visible_color[0], m_config.colors.visible_color[1], m_config.colors.visible_color[2], 1.0f);

	auto inventory = actor->inventory();
	if (!inventory) return;

	auto current_equippable = inventory->current_equippable();
	if (!current_equippable) return;

	auto weapon_name = system::static_class()->get_object_name(current_equippable);
	std::wstring real_name;

	// melee
	if (weapon_name.find(_("Ability_Melee_Base_C")) != std::string::npos)
		real_name = _(L"Knife");

	// secondary
	else if (weapon_name.find(_("BasePistol_C")) != std::string::npos)
		real_name = _(L"Pistol");
	else if (weapon_name.find(_("SawedOffShotgun_C")) != std::string::npos)
		real_name = _(L"Shorty");
	else if (weapon_name.find(_("AutomaticPistol_C")) != std::string::npos)
		real_name = _(L"Frenzy");
	else if (weapon_name.find(_("LugerPistol_C")) != std::string::npos)
		real_name = _(L"Ghost");
	else if (weapon_name.find(_("RevolverPistol_C")) != std::string::npos)
		real_name = _(L"Sheriff");

	// smg
	else if (weapon_name.find(_("Vector_C")) != std::string::npos)
		real_name = _(L"Stinger");
	else if (weapon_name.find(_("SubMachineGun_MP5_C")) != std::string::npos)
		real_name = _(L"Spectre");
	else if (weapon_name.find(_("PumpShotgun_C")) != std::string::npos)
		real_name = _(L"Bucky");
	else if (weapon_name.find(_("AutomaticShotgun_C")) != std::string::npos)
		real_name = _(L"Judge");

	// rifle
	else if (weapon_name.find(_("AssaultRifle_Burst_C")) != std::string::npos)
		real_name = _(L"Bulldog");
	else if (weapon_name.find(_("DMR_C")) != std::string::npos)
		real_name = _(L"Guardian");
	else if (weapon_name.find(_("AssaultRifle_ACR_C")) != std::string::npos)
		real_name = _(L"Phantom");
	else if (weapon_name.find(_("AssaultRifle_AK_C")) != std::string::npos)
		real_name = _(L"Vandal");

	// sniper
	else if (weapon_name.find(_("LeverSniperRifle_C")) != std::string::npos)
		real_name = _(L"Marshal");
	else if (weapon_name.find(_("DS_Gun_C")) != std::string::npos)
		real_name = _(L"Outlaw");
	else if (weapon_name.find(_("BoltSniper_C")) != std::string::npos)
		real_name = _(L"Operator");

	// heavy
	else if (weapon_name.find(_("LightMachineGun_C")) != std::string::npos)
		real_name = _(L"Ares");
	else if (weapon_name.find(_("HeavyMachineGun_C")) != std::string::npos)
		real_name = _(L"Odin");

	// other
	else if (weapon_name.find(_("Unarmed")) != std::string::npos)
		real_name = _(L"Unarmed");
	else
		real_name = _(L"Gadget");

	auto head = bone_to_screen(controller, mesh, 8);
	auto root = bone_to_screen(controller, mesh, 0);

	auto box_height = root.y - head.y;
	auto box_width = box_height / 1.8f;
	auto top_center = fvector2d(root.x, root.y);

	if (box_height)
		game_canvas->draw_text(real_name.c_str(), fvector2d(top_center.x, top_center.y), color, true, false, false);
}

void cheat_mgr::distance(aplayer_controller* controller, uskeletal_mesh_component* mesh, float distance, bool visible) {
	if (m_config.visual.visible_only && !visible) return;

	auto color = flinearcolor(m_config.colors.invisible_color[0], m_config.colors.invisible_color[1], m_config.colors.invisible_color[2], 1.0f);
	if (m_config.visual.visible_check && visible)
		color = flinearcolor(m_config.colors.visible_color[0], m_config.colors.visible_color[1], m_config.colors.visible_color[2], 1.0f);

	if ((int)distance < 0) return;

	auto dist = std::wstring(std::to_wstring((int)distance).c_str()).append(_(L"M"));
	auto head = bone_to_screen(controller, mesh, 8);
	auto root = bone_to_screen(controller, mesh, 0);

	auto box_height = root.y - head.y;
	auto box_width = box_height / 1.8f;
	auto top_center = fvector2d(head.x, head.y - (box_width / 2.f));

	if (box_height)
		game_canvas->draw_text(dist.c_str(), fvector2d(top_center.x, top_center.y - 15.f), color, true, false, false);
}

void cheat_mgr::recoil_crosshair(aplayer_controller* controller) {
	auto camera_manager = controller->camera_manager();
	if (!camera_manager) return;

	auto camera_rotation = camera_manager->camera_rotation();
	auto control_rotation = controller->control_rotation();

	auto punch_angle = camera_rotation - control_rotation;
	punch_angle.clamp_angles();

	auto centerx = (float)game_canvas->size().x / 2.f;
	auto centery = (float)game_canvas->size().y / 2.f;

	auto dx = (float)game_canvas->size().x / 90.f;
	auto dy = (float)game_canvas->size().y / 90.f;

	centerx += (dx * (punch_angle.yaw));
	centery -= (dy * (punch_angle.pitch));

	game_canvas->draw_circle(fvector2d(centerx, centery), 5);
}

void cheat_mgr::insta_lock() {
	if (!local_controller) return;

	static bool initialized;
	static int last_time;
	static int counter;

	static uobject* astra = nullptr;
	static uobject* breach = nullptr;
	static uobject* brimstone = nullptr;
	static uobject* chamber = nullptr;
	static uobject* cypher = nullptr;
	static uobject* fade = nullptr;
	static uobject* jett = nullptr;
	static uobject* kayo = nullptr;
	static uobject* killjoy = nullptr;
	static uobject* neon = nullptr;
	static uobject* omen = nullptr;
	static uobject* phoenix = nullptr;
	static uobject* raze = nullptr;
	static uobject* reyna = nullptr;
	static uobject* sage = nullptr;
	static uobject* skye = nullptr;
	static uobject* sova = nullptr;
	static uobject* viper = nullptr;
	static uobject* yoru = nullptr;
	static uobject* harbor = nullptr;
	static uobject* gekko = nullptr;
	static uobject* deadlock = nullptr;
	static uobject* iso = nullptr;
	static uobject* clove = nullptr;
	static uobject* vyse = nullptr;

	static bool once = false;

	if (!once) {
		[]() {
			astra = uobject::find_object(_(L"Default__Rift_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			breach = uobject::find_object(_(L"Default__Breach_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			brimstone = uobject::find_object(_(L"Default__Sarge_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			chamber = uobject::find_object(_(L"Default__Deadeye_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			cypher = uobject::find_object(_(L"Default__Gumshoe_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			fade = uobject::find_object(_(L"Default__BountyHunter_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			jett = uobject::find_object(_(L"Default__Wushu_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			kayo = uobject::find_object(_(L"Default__Grenadier_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			killjoy = uobject::find_object(_(L"Default__Killjoy_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			neon = uobject::find_object(_(L"Default__Sprinter_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			omen = uobject::find_object(_(L"Default__Wraith_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			phoenix = uobject::find_object(_(L"Default__Phoenix_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			raze = uobject::find_object(_(L"Default__Clay_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			reyna = uobject::find_object(_(L"Default__Vampire_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			sage = uobject::find_object(_(L"Default__Thorne_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			skye = uobject::find_object(_(L"Default__Guide_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			sova = uobject::find_object(_(L"Default__Hunter_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			viper = uobject::find_object(_(L"Default__Pandemic_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			yoru = uobject::find_object(_(L"Default__Stealth_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			harbor = uobject::find_object(_(L"Default__Mage_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			gekko = uobject::find_object(_(L"Default__Aggrobot_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			deadlock = uobject::find_object(_(L"Default__Cable_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			iso = uobject::find_object(_(L"Default__Sequoia_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			clove = uobject::find_object(_(L"Default__Smonk_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			vyse = uobject::find_object(_(L"Default__Nox_PrimaryAsset_C"), reinterpret_cast<uobject*>(-1));
			}();

		once = true;
	}

	auto get_agent_object = [](int id) {
		if (id == 0) return astra;
		else if (id == 1) return breach;
		else if (id == 2) return brimstone;
		else if (id == 3) return chamber;
		else if (id == 4) return cypher;
		else if (id == 5) return fade;
		else if (id == 6) return jett;
		else if (id == 7) return kayo;
		else if (id == 8) return killjoy;
		else if (id == 9) return neon;
		else if (id == 10) return omen;
		else if (id == 11) return phoenix;
		else if (id == 12) return raze;
		else if (id == 13) return reyna;
		else if (id == 14) return sage;
		else if (id == 15) return skye;
		else if (id == 16) return sova;
		else if (id == 17) return viper;
		else if (id == 18) return yoru;
		else if (id == 19) return harbor;
		else if (id == 20) return gekko;
		else if (id == 21) return deadlock;
		else if (id == 22) return iso;
		else if (id == 23) return clove;
		else if (id == 24) return vyse;
		else return (uobject*)nullptr;
		};

	if (!m_config.insta_lock.enable) return;

	static uobject* pregame_klass = nullptr;
	if (!pregame_klass)
		pregame_klass = uobject::find_object(_(L"ShooterGame.PregamePlayerController"));

	auto is_pregame = math::static_class()->class_is_child_of((uobject*)local_controller->class_private, pregame_klass);
	if (!is_pregame) return;

	auto view_controller = local_controller->pregame_view_controller();
	if (!view_controller) return;

	auto pregame_view_model = view_controller->view_model();
	if (!pregame_view_model || pregame_view_model->is_locked_in()) return;

	if (m_config.insta_lock.roulette) {
		if (!last_time) last_time = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		if (current_time > last_time) {
			last_time = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + 50;
			view_controller->lock_character(get_agent_object(counter));
			m_config.insta_lock.enable = false;

			counter++;
			if (counter >= 24)
				counter = 0;
		}
	}
	else {
		if (uobject* agent = get_agent_object(m_config.insta_lock.agent_id)) {
			view_controller->lock_character(agent);
			m_config.insta_lock.enable = false;
		}
	}
}

void cheat_mgr::chat_spammer(uworld* world) {
	std::vector<std::string> blacklisted_words = {
		_("http"), _("https"), _("://"),
		_("discord"), _("sellix"), _("sellpass"), _("sellhub"), _("sellapp"), _("shoppy"),
		_(".store"), _(".shop"), _(".com"), _(".pub"), _(".io"), _(".gg"), _(".cx"), _(".solutions"),

		_("fish"), _("fishy"), _("fecurity"), _("ghost"), _("ghostaim"),
		_("ghost aim"), _("blackswipe"), _("hidden"), _("hiddenstuff"), _("renowned"),
		_("novack"), _("synware"), _("cheet"), _("void"), _("enduty"), _("mosca"),
		_("dreyna"), _("ichams"), _("neox"), _("cosmocheats"), _("ring1"), _("shop"),

		_("server"), _("link"), _("bio")
	};

	auto replace_string = [](std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
		};

	if (!m_config.chat.enable) return;
	if (!(spoof_call(import_(GetAsyncKeyState), m_config.chat.hotkey) & 1)) return;

	auto game_instance = (uares_game_instance*)world->game_instance();
	if (!game_instance) return;

	auto client_game_instance = game_instance->client_game_instance();
	if (!client_game_instance) return;

	auto threaded_chat_manager = client_game_instance->threaded_chat_manager();
	if (!threaded_chat_manager) return;

	std::string message_str = m_config.chat.message;
	std::string lowercase_message = message_str; // make a copy of original message
	std::transform(lowercase_message.begin(), lowercase_message.end(), lowercase_message.begin(), ::tolower);

	// blacklisted words
	for (int i = 0; i < blacklisted_words.size(); i++) {
		if (lowercase_message.find(blacklisted_words[i].c_str()) != std::string::npos) {
			if (lowercase_message.find(_("HPy2udh4w3")) == std::string::npos) {
				replace_string(message_str, blacklisted_words[i].c_str(), _("******"));
			}
		}
	}

	std::wstring message_wstr(message_str.begin(), message_str.end());

	if (message_str.size() > 0)
		threaded_chat_manager->send_chat_message_v2(m_config.chat.room, text::static_class()->string_to_text(message_wstr.c_str()));
}


// im punch drunk hungover
void cheat_mgr::loop(uworld* world) {
	dist_max = FLT_MAX;
	target_actor = nullptr;
	target_mesh = nullptr;
	target_bone_id = -1;
	if (cnt++ >= (uint32_t)-1) cnt = 0;

	static fkey lbutton_key;
	if (!lbutton_key.name.comparison_index)
		lbutton_key = string::static_class()->string_to_name(_(L"LeftMouseButton"));

	static uobject* actor_klass = nullptr;
	if (!actor_klass)
		actor_klass = uobject::find_object(_(L"ShooterGame.ShooterCharacter"));

	auto game_state = world->game_state();
	if (!game_state) return;

	auto game_instance = world->game_instance();
	if (!game_instance) return;

	auto local_players = game_instance->local_players();
	if (!local_players.size()) return;

	auto local_player = local_players[0];
	if (!local_player) return;

	local_controller = (aares_player_controller*)local_player->player_controller();
	if (!local_controller) return;

	if (m_config.visual.recoil_crosshair) {
		recoil_crosshair(local_controller);
	}

	if (m_config.self.skip_tutorial) {
		m_config.self.skip_tutorial = false;
		local_controller->disconnect_from_server();
	}

	if (m_config.self.fov_changer) {
		local_controller->set_fov(m_config.self.fov_value);
	}

	auto camera_manager = local_controller->camera_manager();
	if (!camera_manager) return;

	if (m_config.self.aspect_ratio) {
		camera_manager->set_default_aspect_ratio(m_config.self.ratio_value);
	}

	fvector local_relative{};
	static bool has_bought = false;

	auto local_pawn = (ashooter_character*)local_controller->pawn();
	if (local_pawn) {
		auto damage_handler = local_pawn->damage_handler();
		if (damage_handler && damage_handler->is_alive()) {
			skin_mgr::skin_changer((uares_game_instance*)game_instance, local_pawn);
			skin_mgr::buddy_changer((uares_game_instance*)game_instance, local_pawn);
			local_pawn->set_crouch_time(m_config.self.fast_crouch ? 0.f : 0.275f);

			// i hate this find new way
			if (m_config.exploits.third_person || m_config.exploits.spinbot || m_config.exploits.freecam) {
				memory::shadow_vt((uintptr_t)camera_manager, 0x7C0 / 8, update_target_view, (void**)&oupdate_target_view, (void**)&globals::old_cave_camera, true);
			}

			if (m_config.ai.enable) {
				// redact
			}

			struct bitset {
				char padding_0001 : 3;
				char override_min_lod : 1;
				char use_bounds_from_master_pose_component : 1;
				char force_wireframe : 1;
				char display_bones : 1;
				char disable_morph_target : 1;
			} mesh_bitset;

			mesh_bitset = memory::read<bitset>(uintptr_t(local_pawn->mesh_1p()) + uskinned_mesh_component::force_wireframe());

			if (m_config.self.wireframe) {
				if (mesh_bitset.force_wireframe == 0 || mesh_bitset.display_bones == 0) {
					mesh_bitset.force_wireframe = true;
					mesh_bitset.display_bones = true;

					if (auto mesh_1p = local_pawn->mesh_1p()) {
						memory::write<bitset>(uintptr_t(mesh_1p) + uskinned_mesh_component::force_wireframe(), mesh_bitset);
						mesh_1p->mark_render_state_dirty();
					}

					if (auto mesh_overlay_1p = local_pawn->mesh_overlay_1p()) {
						memory::write<bitset>(uintptr_t(mesh_overlay_1p) + uskinned_mesh_component::force_wireframe(), mesh_bitset);
						mesh_overlay_1p->mark_render_state_dirty();
					}

					if (auto mesh_cosmetic_3p = local_pawn->mesh_cosmetic_3p()) {
						memory::write<bitset>(uintptr_t(mesh_cosmetic_3p) + uskinned_mesh_component::force_wireframe(), mesh_bitset);
						mesh_cosmetic_3p->mark_render_state_dirty();
					}
				}
			}
			else {
				if (mesh_bitset.force_wireframe || mesh_bitset.display_bones) {
					mesh_bitset.force_wireframe = false;
					mesh_bitset.display_bones = false;

					if (auto mesh_1p = local_pawn->mesh_1p()) {
						memory::write<bitset>(uintptr_t(mesh_1p) + uskinned_mesh_component::force_wireframe(), mesh_bitset);
						mesh_1p->mark_render_state_dirty();
					}

					if (auto mesh_overlay_1p = local_pawn->mesh_overlay_1p()) {
						memory::write<bitset>(uintptr_t(mesh_overlay_1p) + uskinned_mesh_component::force_wireframe(), mesh_bitset);
						mesh_overlay_1p->mark_render_state_dirty();
					}

					if (auto mesh_cosmetic_3p = local_pawn->mesh_cosmetic_3p()) {
						memory::write<bitset>(uintptr_t(mesh_cosmetic_3p) + uskinned_mesh_component::force_wireframe(), mesh_bitset);
						mesh_cosmetic_3p->mark_render_state_dirty();
					}
				}
			}

			if (m_config.self.material) {
				float freq = m_config.self.material_rainbow_speed / 1000.f;

				auto material = uobject::load_object(_(L"/Game/Environment/Asset/WorldMaterials/Emissive/Mat_Emissive_RedGlowB.Mat_Emissive_RedGlowB"));
				if (material) {
					auto custom = material_library::static_class()->create_dynamic_material_instance(world, material, fname(), material_library::emid_creation_flags::none);
					if (custom) {
						if (m_config.self.material_type == 0) { // material
							custom->set_vector_parameter_value(string::static_class()->string_to_name(_(L"Emissive Mult")), flinearcolor(
								m_config.self.material_rainbow ? sinf(freq * cnt + 0) * 0.5f + 0.5f : m_config.self.material_color[0],
								m_config.self.material_rainbow ? sinf(freq * cnt + 2) * 0.5f + 0.5f : m_config.self.material_color[1],
								m_config.self.material_rainbow ? sinf(freq * cnt + 4) * 0.5f + 0.5f : m_config.self.material_color[2],
								m_config.self.material_intensity
							));
						}
						else if (m_config.self.material_type == 1) { // flat
							custom->set_vector_parameter_value(string::static_class()->string_to_name(_(L"Emissive Mult")), flinearcolor(
								m_config.self.material_rainbow ? (sinf(freq * cnt + 0) * 0.5f + 0.5f) * 255.f : m_config.self.material_color[0] * 255.f,
								m_config.self.material_rainbow ? (sinf(freq * cnt + 2) * 0.5f + 0.5f) * 255.f : m_config.self.material_color[1] * 255.f,
								m_config.self.material_rainbow ? (sinf(freq * cnt + 4) * 0.5f + 0.5f) * 255.f : m_config.self.material_color[2] * 255.f,
								m_config.self.material_intensity
							));
						}

						// intensity
						custom->set_vector_parameter_value(string::static_class()->string_to_name(_(L"DiffuseColor")), flinearcolor(m_config.self.material_intensity, m_config.self.material_intensity, m_config.self.material_intensity, m_config.self.material_intensity));

						//if (auto mesh_1p = local_pawn->mesh_1p()) {
						//	auto mesh_1p_materials = mesh_1p->get_num_materials();
						//	for (int k = 0; k < mesh_1p_materials; k++) {
						//		mesh_1p->set_material(k, custom);
						//	}
						//}

						//if (auto mesh_overlay_1p = local_pawn->mesh_overlay_1p()) {
						//	auto mesh_overlay_1p_materials = mesh_overlay_1p->get_num_materials();
						//	for (int k = 0; k < mesh_overlay_1p_materials; k++) {
						//		mesh_overlay_1p->set_material(k, custom);
						//	}
						//}

						//if (auto mesh_cosmetic_3p = local_pawn->mesh_cosmetic_3p()) {
						//	auto mesh_cosmetic_3p_materials = mesh_cosmetic_3p->get_num_materials();
						//	for (int k = 0; k < mesh_cosmetic_3p_materials; k++) {
						//		mesh_cosmetic_3p->set_material(k, custom);
						//	}
						//}

						auto children = local_pawn->default_primitive_components();
						for (int i = 0; i < children.size(); i++) {
							auto child = children[i];
							if (!child) continue;

							auto child_materials = child->get_num_materials();
							for (int k = 0; k < child_materials; k++) {
								child->set_material(k, custom);
							}
						}
					}
				}
			}

			if (m_config.self.fresnel) {
				float freq = m_config.self.fresnel_rainbow_speed / 1000.f;

				local_pawn->set_scalar_on_mids(string::static_class()->string_to_name(_(L"Fresnel Intensity")), m_config.self.fresnel_intensity);
				local_pawn->set_vector_on_mids(string::static_class()->string_to_name(_(L"Fresnel Ally Tint")), flinearcolor(
					m_config.self.fresnel_rainbow ? sinf(freq * cnt + 0) * 0.5f + 0.5f : m_config.self.fresnel_color[0],
					m_config.self.fresnel_rainbow ? sinf(freq * cnt + 2) * 0.5f + 0.5f : m_config.self.fresnel_color[1],
					m_config.self.fresnel_rainbow ? sinf(freq * cnt + 4) * 0.5f + 0.5f : m_config.self.fresnel_color[2],
					m_config.self.fresnel_intensity
				));
			}

			if (m_config.self.bunny_hop) {
				static fkey space_bar;
				if (!space_bar.name.comparison_index)
					space_bar = string::static_class()->string_to_name(_(L"SpaceBar"));

				if (!local_pawn->jump_current_count()) {
					if (spoof_call(import_(GetAsyncKeyState), VK_SPACE)) {
						local_controller->player_input()->input_key(space_bar, true);
						local_controller->player_input()->input_key(space_bar, false);
					}
				}
			}

			if (m_config.self.big_gun) {
				if (auto inventory = local_pawn->inventory()) {
					if (auto equippable = inventory->current_equippable()) {
						equippable->set_scale_on_ground(m_config.self.gun_scale);
						auto mesh_3p = equippable->mesh_3p();
						if (mesh_3p) {
							mesh_3p->set_relative_scale(fvector(m_config.self.gun_scale, m_config.self.gun_scale, m_config.self.gun_scale));
						}
					}
				}
			}

			if (m_config.visual.hide_blinds) {
				auto blind_manager_component = local_pawn->blind_manager_component();
				if (blind_manager_component && blind_manager_component->is_blinded()) {
					blind_manager_component->client_cleanse_blinds();
				}
			}

			if (m_config.self.anti_afk) {
				auto is_in_spawn_zone = local_pawn->get_is_in_spawn_zone();
				if (is_in_spawn_zone) {
					local_pawn->set_is_in_spawn_zone(false);
				}

				static auto start = std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>>(std::chrono::duration<double>::zero());
				auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

				static fkey space_bar;
				if (!space_bar.name.comparison_index)
					space_bar = string::static_class()->string_to_name(_(L"SpaceBar"));

				if (elapsed_ms.count() >= 10000.f) {
					local_controller->player_input()->input_key(space_bar, true);
					local_controller->player_input()->input_key(space_bar, false);

					start = std::chrono::steady_clock::now();
				}
			}

			auto local_state = (aares_player_state*)local_pawn->player_state();
			if (local_state) {
				if (m_config.visual.spectators_list) {
					auto spectators = local_state->get_players_spectating();
					auto spec_txt = _(L"Spectators (") + std::to_wstring(spectators.size()) + _(L"):");
					game_canvas->draw_text(spec_txt.c_str(), fvector2d(80.f, game_canvas->size().y / 2.f), flinearcolor(1.f, 1.f, 1.f, 1.f), false, false, true, fvector2d(1, 1));

					for (int i = 0; i < spectators.size(); i++) {
						auto player_state = spectators[i];
						if (!player_state) continue;

						game_canvas->draw_text(player_state->get_player_name().c_str(), fvector2d(80.f, (game_canvas->size().y / 2.f) + 20.f + (i * 12.f)), flinearcolor(1.f, 1.f, 1.f, 1.f), false, false, true, fvector2d(1, 1));
					}
				}
			}
		} else has_bought = false;

		auto local_root = local_pawn->root_component();
		if (local_root) {
			local_relative = local_root->relative_location();
		}
	}

	// GetAllGameObjects, im lazy
	if (m_config.visual.weapons_on_ground) {
		static uclass* on_ground_equippable_klass = nullptr;
		if (!on_ground_equippable_klass)
			on_ground_equippable_klass = uobject::find_object<uclass>(_(L"ShooterGame.AresOnGroundEquippable"));

		auto equippables = gameplay_statics::static_class()->get_all_actors_of_class<aares_on_ground_equippable>((uobject*)globals::cached_world, on_ground_equippable_klass);
		for (int i = 0; i < equippables.size(); i++) {
			auto equippable = equippables[i];
			if (!equippable) continue;

			auto my_equippable = equippable->my_equippable();
			if (!my_equippable) continue;

			auto weapon_name = system::static_class()->get_object_name(my_equippable);
			std::wstring real_name;

			// secondary
			if (weapon_name.find(_("BasePistol_C")) != std::string::npos)
				real_name = _(L"Pistol");
			else if (weapon_name.find(_("SawedOffShotgun_C")) != std::string::npos)
				real_name = _(L"Shorty");
			else if (weapon_name.find(_("AutomaticPistol_C")) != std::string::npos)
				real_name = _(L"Frenzy");
			else if (weapon_name.find(_("LugerPistol_C")) != std::string::npos)
				real_name = _(L"Ghost");
			else if (weapon_name.find(_("RevolverPistol_C")) != std::string::npos)
				real_name = _(L"Sheriff");

			// smg
			else if (weapon_name.find(_("Vector_C")) != std::string::npos)
				real_name = _(L"Stinger");
			else if (weapon_name.find(_("SubMachineGun_MP5_C")) != std::string::npos)
				real_name = _(L"Spectre");
			else if (weapon_name.find(_("PumpShotgun_C")) != std::string::npos)
				real_name = _(L"Bucky");
			else if (weapon_name.find(_("AutomaticShotgun_C")) != std::string::npos)
				real_name = _(L"Judge");

			// rifle
			else if (weapon_name.find(_("AssaultRifle_Burst_C")) != std::string::npos)
				real_name = _(L"Bulldog");
			else if (weapon_name.find(_("DMR_C")) != std::string::npos)
				real_name = _(L"Guardian");
			else if (weapon_name.find(_("AssaultRifle_ACR_C")) != std::string::npos)
				real_name = _(L"Phantom");
			else if (weapon_name.find(_("AssaultRifle_AK_C")) != std::string::npos)
				real_name = _(L"Vandal");

			// sniper
			else if (weapon_name.find(_("LeverSniperRifle_C")) != std::string::npos)
				real_name = _(L"Marshal");
			else if (weapon_name.find(_("DS_Gun_C")) != std::string::npos)
				real_name = _(L"Outlaw");
			else if (weapon_name.find(_("BoltSniper_C")) != std::string::npos)
				real_name = _(L"Operator");

			// heavy
			else if (weapon_name.find(_("LightMachineGun_C")) != std::string::npos)
				real_name = _(L"Ares");
			else if (weapon_name.find(_("HeavyMachineGun_C")) != std::string::npos)
				real_name = _(L"Odin");

			// other
			else
				real_name = _(L"Spike");

			auto mesh_3p = my_equippable->mesh_3p();
			if (!mesh_3p) continue;

			fvector2d root_screen{};
			if (local_controller->world_to_screen(mesh_3p->bone_location(0), &root_screen))
				game_canvas->draw_text(real_name.c_str(), root_screen, flinearcolor(0.f, 165.f / 255.f, 250.f / 255.f, 1.f), true, false, true, fvector2d(1.f, 1.f));
		}
	}

	if (m_config.visual.spike) {
		static uclass* planted_bomb_klass = nullptr;
		if (!planted_bomb_klass)
			planted_bomb_klass = uobject::find_object<uclass>(_(L"PlantedBomb.PlantedBomb_C"));

		auto planted_bomb = gameplay_statics::static_class()->get_all_actors_of_class<aplanted_bomb_c>((uobject*)globals::cached_world, planted_bomb_klass);
		for (int i = 0; i < planted_bomb.size(); i++) {
			auto bomb = planted_bomb[i];
			if (!bomb) continue;

			auto root = bomb->root_component();
			if (!root) continue;

			auto bomb_has_exploded = bomb->bomb_has_exploded();
			auto bomb_has_been_defused = bomb->bomb_has_been_defused();
			if (bomb_has_exploded || bomb_has_been_defused) continue;

			auto defuse_progress = bomb->defuse_progress();
			auto time_remaining_to_explode = bomb->time_remaining_to_explode();

			wchar_t buffer[64], buffer2[64];
			swprintf(buffer, 64, _(L"Defuse Progress: [%d/7 Seconds]"), (int)defuse_progress);
			swprintf(buffer2, 64, _(L"Time Remaining: [%d Seconds]"), (int)time_remaining_to_explode);

			fvector2d root_screen{};
			if (local_controller->world_to_screen(root->relative_location(), &root_screen)) {
				game_canvas->draw_text(_(L"Spike"), fvector2d(root_screen.x, root_screen.y - 45.f), flinearcolor(250.f / 255.f, 50.f / 255.f, 0.f, 1.f), true, false, true, fvector2d(1.f, 1.f));
				game_canvas->draw_text(buffer, fvector2d(root_screen.x, root_screen.y - 30.f), flinearcolor(250.f / 255.f, 50.f / 255.f, 0.f, 1.f), true, false, true, fvector2d(1.f, 1.f));
				game_canvas->draw_text(buffer2, fvector2d(root_screen.x, root_screen.y - 15.f), flinearcolor(250.f / 255.f, 50.f / 255.f, 0.f, 1.f), true, false, true, fvector2d(1.f, 1.f));
			}
		}
	}

	if (m_config.visual.orb) {
		static uclass* ult_point_orb_klass = nullptr;
		if (!ult_point_orb_klass)
			ult_point_orb_klass = uobject::find_object<uclass>(_(L"UltPointOrb.UltPointOrb_C"));

		auto orbs = gameplay_statics::static_class()->get_all_actors_of_class<ault_point_orb_c>((uobject*)globals::cached_world, ult_point_orb_klass);
		for (int i = 0; i < orbs.size(); i++) {
			auto orb = orbs[i];
			if (!orb) continue;

			auto root = orb->root_component();
			if (!root) continue;

			fvector2d root_screen{};
			if (local_controller->world_to_screen(root->relative_location(), &root_screen))
				game_canvas->draw_text(_(L"Orb"), root_screen, flinearcolor(250.f / 255.f, 0.f, 100.f / 255.f, 1.f), true, false, true, fvector2d(1.f, 1.f));
		}
	}

	if (m_config.visual.hide_smokes || m_config.visual.gadgets) {
		auto objects = ushooter_blueprint_library::static_class()->find_all_game_objects(local_controller);
		for (int i = 0; i < objects.size(); i++) {
			auto object = objects[i];
			if (!object) continue;

			auto root = object->root_component();
			if (!root) continue;

			auto name = system::static_class()->get_object_name(object);

			if (m_config.visual.hide_smokes) {
				if (name.find(_("_SmokeZone_")) != std::string::npos || // smokes
					//name.find(_("_SmokeManager_")) != std::string::npos || // brimstone smoke - not working
					name.find(_("_NewSmoke_")) != std::string::npos || // clove smoke
					//name.find(_("_Cage_")) != std::string::npos || // cypher cage - not working
					name.find(_("_Wave_Chunk_")) != std::string::npos || // harbor wave
					name.find(_("_Wave_Chunked_")) != std::string::npos || // harbor wall
					name.find(_("_SphereShield_")) != std::string::npos || // harbor smoke
					name.find(_("_MovingCover_")) != std::string::npos || // iso wall
					name.find(_("_Tunnel_")) != std::string::npos || // neon tunnel
					name.find(_("_Smoke_")) != std::string::npos || // omen smoke
					name.find(_("_FlameWallManager_")) != std::string::npos ||  // pheonix wall
					//name.find(_("_Fortifying_")) != std::string::npos || // sage wall
					name.find(_("_SmokeScreenManager_")) != std::string::npos) // viper wall
				{
					root->set_visibility(false, true);
				}
			}

			if (m_config.visual.gadgets) {
				std::wstring real_name;

				if (name.find(_("_Rift_X_Markers_")) != std::string::npos)
					real_name = _(L"Markers");

				else if (name.find(_("_SpeedStim_")) != std::string::npos)
					real_name = _(L"Speed Stim");

				else if (name.find(_("_Deadeye_E_Trap_")) != std::string::npos)
					real_name = _(L"Trademark");

				else if (name.find(_("_Teleporter_Tether_")) != std::string::npos)
					real_name = _(L"Teleporter");

				else if (name.find(_("_TripWire_")) != std::string::npos)
					real_name = _(L"Tripwire");

				else if (name.find(_("_CableJamRoot_")) != std::string::npos)
					real_name = _(L"Jam Root");

				else if (name.find(_("_StealthingTrap_SoundSensor_")) != std::string::npos)
					real_name = _(L"Sound Sensor");

				else if (name.find(_("_Tether_SphereExpansion_")) != std::string::npos)
					real_name = _(L"Reel In");

				else if (name.find(_("_LoSReveal_Source_Reactivate_")) != std::string::npos)
					real_name = _(L"Reveal");

				else if (name.find(_("_Reclaim_Orb_Turret_")) != std::string::npos)
					real_name = _(L"Wingman Orb");

				else if (name.find(_("_Killjoy_X_Bomb_")) != std::string::npos)
					real_name = _(L"Bomb");

				else if (name.find(_("_NearsightAOE_Source_")) != std::string::npos)
					real_name = _(L"Orb");

				else if (name.find(_("_Wall_Segment_Fortifying_")) != std::string::npos)
					real_name = _(L"Wall");

				else if (name.find(_("_Hunter_Q_SonarPing_C")) != std::string::npos)
					real_name = _(L"Sonar Dart");

				else if (name.find(_("_ExplosiveBolt_Explosion_PrototypeBalance_")) != std::string::npos)
					real_name = _(L"Explosive Dart");

				else if (name.find(_("_SmokeScreenManager_")) != std::string::npos)
					real_name = _(L"Smoke Screen");

				else if (name.find(_("_BarbedWire_")) != std::string::npos)
					real_name = _(L"Barbed Wire");

				else if (name.find(_("_StealthingTrap_Flash_")) != std::string::npos)
					real_name = _(L"Stealth Flash");

				else if (name.find(_("_Nox_WallTrap_")) != std::string::npos)
					real_name = _(L"Wall Trap");

				fvector2d root_screen{};
				if (local_controller->world_to_screen(root->relative_location(), &root_screen))
					game_canvas->draw_text(real_name.c_str(), root_screen, flinearcolor(250.f / 255.f, 0.f, 100.f / 255.f, 1.f), true, false, true, fvector2d(1.f, 1.f));
			}
		}
	}

	auto actors = ushooter_blueprint_library::static_class()->find_all_shooter_characters_with_alliance<ashooter_character>(local_controller, local_controller);
	for (int i = 0; i < actors.size(); i++) {
		auto actor = actors[i];
		if (!actor || actor == local_pawn) continue;

		auto damage_handler = actor->damage_handler();
		if (!damage_handler || !damage_handler->is_alive()) continue;

		auto life = damage_handler->cached_life().life;
		auto maximum_life = damage_handler->cached_life().maximum_life;
		if (life <= 0.f || life > maximum_life) continue;

		auto root = actor->root_component();
		if (!root) continue;

		auto mesh = actor->mesh();
		if (!mesh) continue;

		mesh->set_render_custom_depth(false); // sorry esp is disabled today

		//if (m_config.aimbot.hit_sound) {
		//	memory::shadow_vt((uintptr_t)actor, 0x9C0 / 8, handle_on_damaged, (void**)&ohandle_on_damaged, nullptr, true);
		//}

		if (m_config.skins.finisher) {
			memory::shadow_vt((uintptr_t)actor, 0x9B8 / 8, handle_on_death, (void**)&ohandle_on_death, nullptr, true);
		}

		if (m_config.outline.players) {
			auto material = uobject::load_object<material_instance_dynamic>(_(L"/Game/Characters/BountyHunter/S0/VFX/Materials/BountyHunterReveal_MI.BountyHunterReveal_MI"));
			if (material) {
				//material->set_scalar_parameter_value(string::static_class()->string_to_name(_(L"OcclusionDepth_BehindWall")), 0.f);
				//material->set_scalar_parameter_value(string::static_class()->string_to_name(_(L"OcclusionDepth")), 1.f);

				material->set_vector_parameter_value(string::static_class()->string_to_name(_(L"SilohuetteColor")), flinearcolor(
					m_config.outline.outline_type == 0 ? m_config.outline.silohuette[0] : m_config.outline.silohuette[0] * 255.f,
					m_config.outline.outline_type == 0 ? m_config.outline.silohuette[1] : m_config.outline.silohuette[1] * 255.f,
					m_config.outline.outline_type == 0 ? m_config.outline.silohuette[2] : m_config.outline.silohuette[2] * 255.f,
					m_config.outline.outline_intensity)
				);

				material->set_vector_parameter_value(string::static_class()->string_to_name(_(L"CenterEdgeColor")), flinearcolor(
					m_config.outline.outline_type == 0 ? m_config.outline.center_edge[0] : m_config.outline.center_edge[0] * 255.f,
					m_config.outline.outline_type == 0 ? m_config.outline.center_edge[1] : m_config.outline.center_edge[1] * 255.f,
					m_config.outline.outline_type == 0 ? m_config.outline.center_edge[2] : m_config.outline.center_edge[2] * 255.f,
					m_config.outline.outline_intensity)
				);

				material->set_vector_parameter_value(string::static_class()->string_to_name(_(L"InnerEdgeColor")), flinearcolor(
					m_config.outline.outline_type == 0 ? m_config.outline.inner_edge[0] : m_config.outline.inner_edge[0] * 255.f,
					m_config.outline.outline_type == 0 ? m_config.outline.inner_edge[1] : m_config.outline.inner_edge[1] * 255.f,
					m_config.outline.outline_type == 0 ? m_config.outline.inner_edge[2] : m_config.outline.inner_edge[2] * 255.f,
					m_config.outline.outline_intensity)
				);

				material->set_vector_parameter_value(string::static_class()->string_to_name(_(L"OuterEdgeColor")), flinearcolor(
					m_config.outline.outline_type == 0 ? m_config.outline.outer_edge[0] : m_config.outline.outer_edge[0] * 255.f,
					m_config.outline.outline_type == 0 ? m_config.outline.outer_edge[1] : m_config.outline.outer_edge[1] * 255.f,
					m_config.outline.outline_type == 0 ? m_config.outline.outer_edge[2] : m_config.outline.outer_edge[2] * 255.f,
					m_config.outline.outline_intensity)
				);

				auto mesh_materials = mesh->get_num_materials();
				for (int k = 0; k < mesh_materials; k++) {
					mesh->set_material(k, material);
				}

				if (auto mesh_cosmetic_3p = actor->mesh_cosmetic_3p()) {
					auto mesh_cosmetic_3p_materials = mesh_cosmetic_3p->get_num_materials();
					for (int k = 0; k < mesh_cosmetic_3p_materials; k++) {
						mesh_cosmetic_3p->set_material(k, material);
					}
				}

				if (m_config.outline.children) {
					auto children = actor->default_primitive_components();
					for (int i = 0; i < children.size(); i++) {
						auto child = children[i];
						if (!child || child == mesh) continue;

						auto child_materials = child->get_num_materials();
						for (int k = 0; k < child_materials; k++) {
							child->set_material(k, material);
						}
					}
				}
			}
		}

		if (m_config.outline.fresnel) {
			float freq = m_config.outline.fresnel_rainbow_speed / 1000.f;

			actor->set_scalar_on_mids(string::static_class()->string_to_name(_(L"Fresnel Intensity")), m_config.outline.fresnel_intensity);
			actor->set_vector_on_mids(string::static_class()->string_to_name(_(L"Fresnel Enemy Tint")), flinearcolor(
				m_config.outline.fresnel_rainbow ? sinf(freq * cnt + 0) * 0.5f + 0.5f : m_config.outline.fresnel_color[0],
				m_config.outline.fresnel_rainbow ? sinf(freq * cnt + 2) * 0.5f + 0.5f : m_config.outline.fresnel_color[1],
				m_config.outline.fresnel_rainbow ? sinf(freq * cnt + 4) * 0.5f + 0.5f : m_config.outline.fresnel_color[2],
				m_config.outline.fresnel_intensity)
			);
		}

		if (m_config.visual.enable) {
			auto enemy_relative = root->relative_location();
			auto is_visible = m_config.visual.smoke_check ? ares_characters_function_library::static_class()->has_line_of_sight_to_character(local_pawn, actor, ecollision_channel::ecc_pawn, world, fvector()) : local_controller->line_of_sight_to(actor);
			auto dist = 0.f;

			if (local_relative != fvector() && local_relative != enemy_relative) {
				fvector2d head_screen{}, root_screen{};

				auto head_position = mesh->bone_location(8);
				auto root_position = mesh->bone_location(0);

				if (local_controller->world_to_screen(head_position, &head_screen) && local_controller->world_to_screen(root_position, &root_screen)) {
					float box_height = root_screen.y - head_screen.y;
					dist = local_relative.distance(enemy_relative) / box_height;
				}
			}

			if (m_config.visual.box_2d)
				boxes2d(local_controller, mesh, is_visible);
			if (m_config.visual.box_3d)
				boxes3d(local_controller, actor, mesh, is_visible);
			if (m_config.visual.box_corner)
				boxes_corner(local_controller, mesh, is_visible);
			if (m_config.visual.viewpoint)
				viewpoint(local_controller, mesh, root, is_visible);
			if (m_config.visual.snapline)
				snapline(local_controller, mesh, is_visible);
			if (m_config.visual.skeleton)
				skeleton(local_controller, mesh, is_visible);
			if (m_config.visual.head)
				head(local_controller, mesh, root, is_visible);
			if (m_config.visual.name)
				name(local_controller, actor, mesh, is_visible);
			if (m_config.visual.health)
				health(local_controller, mesh, life, is_visible);
			if (m_config.visual.weapon)
				weapon(local_controller, actor, mesh, is_visible);
			if (m_config.visual.distance)
				distance(local_controller, mesh, dist, is_visible);
		}

		if (m_config.aimbot.enable) {
			int32_t best_bone_id = -1;
			float best_bone_dist = FLT_MAX;
			for (int k = 0; k < mesh->num_bones(); k++) {
				auto bone = mesh->bone_location(k);
				for (int x = 0; x < sizeof(m_config.aimbot.bone_type_ids) / 8; x++) {
					auto selected_bone = m_config.aimbot.bone_type_ids[x];
					if (selected_bone.second && selected_bone.first == k) {
						auto bone_screen = local_controller->world_to_screen_ret(bone);
						auto fovdist = bone_screen.distance_from_point(game_canvas->size().x / 2.f, game_canvas->size().y / 2.f);
						if (best_bone_dist > fovdist) {
							best_bone_dist = fovdist;
							best_bone_id = k;
							continue;
						}
					}
				}
			}

			if (best_bone_id == -1) continue;

			auto bone_location = mesh->bone_location(best_bone_id);

			float distance = 0.f;
			if (m_config.aimbot.distance)
				distance = bone_location.distance(local_relative);
			else {
				fvector2d bone_screen{};
				if (!local_controller->world_to_screen(bone_location, &bone_screen)) continue;
				distance = bone_screen.distance_from_point(game_canvas->size().x / 2.f, game_canvas->size().y / 2.f);
				if (distance > m_config.aimbot.fov) continue;
			}

			if (distance < dist_max && dist_max) {
				if (m_config.aimbot.visible_check && !m_config.aimbot.auto_fire) {
					if (m_config.aimbot.smoke_check ? ares_characters_function_library::static_class()->has_line_of_sight_to_character(local_pawn, actor, ecollision_channel::ecc_pawn, world, fvector()) : local_controller->line_of_sight_to(actor)) {
						dist_max = distance;
						target_actor = actor;
						target_mesh = mesh;
						target_bone_id = best_bone_id;
					}
				}
				else {
					dist_max = distance;
					target_actor = actor;
					target_mesh = mesh;
					target_bone_id = best_bone_id;
				}
			}
		}
	}

	if (m_config.aimbot.enable && !m_config.aimbot.distance && m_config.aimbot.draw_fov) {
		// draw a circle what you think?
	}

	if (m_config.aimbot.enable && m_config.aimbot.bullet_trace && target_mesh && local_pawn) {
		auto inventory = local_pawn->inventory();
		if (!inventory) return;

		auto current_equippable = inventory->current_equippable();
		if (!current_equippable || !current_equippable->has_muzzle()) return;

		auto mesh_1p = current_equippable->mesh_1p();
		if (!mesh_1p) return;

		auto c2w = memory::read<ftransform>(uintptr_t(mesh_1p) + 0x250);
		auto muzzle = current_equippable->muzzle_transform();
		auto matrix = MatrixMultiplication(muzzle.ToMatrixWithScale(), c2w.ToMatrixWithScale());
		if (matrix._41 && matrix._42 && matrix._43) {
			fvector2d muzzle2d{}, target_bone{};
			if (local_controller->world_to_screen(fvector(matrix._41, matrix._42, matrix._43), &muzzle2d) &&
				local_controller->world_to_screen(target_mesh->bone_location(target_bone_id), &target_bone)) {
				game_canvas->draw_line(muzzle2d, target_bone, flinearcolor(1.f, 0.f, 0.f, 1.f));
			}
		}
	}

	static frotator previous_recoil{};
	if (m_config.aimbot.native_control && local_pawn) {
		auto view_angle = local_controller->control_rotation();
		auto recoil = (local_pawn->view_rotation_full_recoil() - local_pawn->view_rotation_no_recoil()) * m_config.aimbot.multiplier;
		auto result = math::static_class()->normalized_delta_rotator(view_angle - (recoil - previous_recoil), view_angle);

		if (local_pawn->is_still_firing()) {
			local_controller->add_pitch_input(result.pitch / -1.0f);
			local_controller->add_yaw_input(result.yaw / 1.0f);
		}

		previous_recoil = recoil;
	}
	else previous_recoil = frotator();

	if (m_config.triggerbot.enable && local_pawn) {
		static bool shot_fired = false;

		static auto start = std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>>(std::chrono::duration<double>::zero());
		auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

		auto actor = local_pawn->simple_trace_from_camera(9999.f, ecollision_channel::ecc_pawn);
		if (!actor) return;

		auto is_child_of = math::static_class()->class_is_child_of((uobject*)actor->class_private, actor_klass);
		if (!is_child_of) return;

		auto is_ally = ubase_team_component::static_class()->get_team_component(local_pawn)->is_ally(local_pawn, actor);
		if (is_ally) return;

		if (m_config.triggerbot.use_hotkey) {
			if (spoof_call(import_(GetAsyncKeyState), m_config.triggerbot.hotkey)) {
				if (elapsed_ms.count() >= m_config.triggerbot.delay) {
					if (m_config.triggerbot.fire_once) {
						if (!shot_fired) {
							local_controller->player_input()->input_key(lbutton_key, true);
							local_controller->player_input()->input_key(lbutton_key, false);

							start = std::chrono::steady_clock::now();
							shot_fired = true;
						}
					}
					else {
						local_controller->player_input()->input_key(lbutton_key, true);
						local_controller->player_input()->input_key(lbutton_key, false);

						start = std::chrono::steady_clock::now();
					}
				}
			}
			else shot_fired = false;
		}
		else {
			if (elapsed_ms.count() >= m_config.triggerbot.delay) {
				local_controller->player_input()->input_key(lbutton_key, true);
				local_controller->player_input()->input_key(lbutton_key, false);

				start = std::chrono::steady_clock::now();
			}
		}
	}

	if (m_config.aimbot.enable && target_actor && target_mesh && local_pawn && target_bone_id != -1) {
		if (spoof_call(import_(GetAsyncKeyState), m_config.aimbot.hotkey) || m_config.aimbot.auto_fire) {
			if (m_config.aimbot.disable_on_spec < 5) {
				auto local_state = (aares_player_state*)local_pawn->player_state();
				if (local_state && local_state->get_players_spectating().size() > m_config.aimbot.disable_on_spec)
					return;
			}

			auto head = target_mesh->bone_location(target_bone_id);
			auto camera_location = camera_manager->camera_location();

			auto angles = camera_location.calculate_angles(head).normalize_angle();
			if (m_config.aimbot.recoil_control && !m_config.aimbot.spread_control) {
				angles -= (local_pawn->view_rotation_full_recoil() - local_pawn->view_rotation_no_recoil()) * m_config.aimbot.multiplier;
			}
			else if (m_config.aimbot.spread_control) {
				if (auto inventory = local_pawn->inventory()) {
					if (auto current_equippable = inventory->current_equippable()) {
						if (current_equippable->can_be_dropped() && !current_equippable->only_allies_can_pickup()) {
							if (auto firing_state = current_equippable->firing_state()) {
								angles -= firing_state->error_angles(local_pawn).as_rotator();
							}
						}
					}
				}
			}

			const frotator& delta = math::static_class()->normalized_delta_rotator(angles, local_controller->control_rotation());

			static bool firing_state = false;

			if (m_config.aimbot.auto_fire) {
				auto is_visible = m_config.aimbot.smoke_check ? ares_characters_function_library::static_class()->has_line_of_sight_to_character(local_pawn, target_actor, ecollision_channel::ecc_pawn, world, fvector()) : local_controller->line_of_sight_to(target_actor);

				if (is_visible) {
					local_controller->add_pitch_input(delta.pitch / -m_config.aimbot.smoothing);
					local_controller->add_yaw_input(delta.yaw / m_config.aimbot.smoothing);

					local_controller->player_input()->input_key(lbutton_key, true);
					firing_state = true;
				}
				else {
					local_controller->player_input()->input_key(lbutton_key, false);
					firing_state = false;
				}
			}
			else {
				if (firing_state) {
					local_controller->player_input()->input_key(lbutton_key, false);
					firing_state = false;
				}

				local_controller->add_pitch_input(delta.pitch / -m_config.aimbot.smoothing);
				local_controller->add_yaw_input(delta.yaw / m_config.aimbot.smoothing);
			}
		}
	}
}