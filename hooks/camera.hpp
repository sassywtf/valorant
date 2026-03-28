#pragma once
#include "../sdks/structs.hpp"
#include "../sdks/engine.hpp"
#include "../sdks/game.hpp"
#include "../config/config.hpp"



// yeee im coding dw w dev



static bool should_update_loc = true, freecam_state = false;
static fvector camera_loc = fvector();
static frotator camera_rot = frotator();

inline void(*oupdate_target_view)(acamera_manager*, ftview_target*, float);
inline void update_target_view(acamera_manager* camera, ftview_target* vt, float delta_time)
{
	if (!camera || !vt) return;

	spoof_call(oupdate_target_view, camera, vt, delta_time);

	auto pawn = (ashooter_character*)vt->target;
	if (!pawn) {
		should_update_loc = true;
		return;
	}

	if (m_config.exploits.freecam)
	{
		if (should_update_loc) {
			camera_loc = vt->pov.location;
			camera_rot = vt->pov.rotation;
			should_update_loc = false;
		}

		if (spoof_call(import_(GetAsyncKeyState), m_config.exploits.freecam_hotkey) & 1)
			freecam_state = !freecam_state;

		if (freecam_state) {
			spoof_call(oupdate_target_view, camera, vt, delta_time);
			auto current_rot = vt->pov.rotation;

			if (spoof_call(import_(GetAsyncKeyState), (int)'W')) {
				camera_loc += current_rot.forward() * m_config.exploits.freecam_speed;
			}
			if (spoof_call(import_(GetAsyncKeyState), (int)'S')) {
				camera_rot = { -current_rot.pitch, current_rot.yaw + 180.f, 0.f };
				camera_loc += camera_rot.forward() * m_config.exploits.freecam_speed;
			}
			if (spoof_call(import_(GetAsyncKeyState), (int)'A')) {
				camera_rot = { 0.f, current_rot.yaw + 270.f, 0.f };
				camera_loc += camera_rot.forward() * m_config.exploits.freecam_speed;
			}
			if (spoof_call(import_(GetAsyncKeyState), (int)'D')) {
				camera_rot = { 0.f,current_rot.yaw + 90.f,0.f };
				camera_loc += camera_rot.forward() * m_config.exploits.freecam_speed;
			}
			if (spoof_call(import_(GetAsyncKeyState), VK_SPACE)) {
				camera_loc.z += m_config.exploits.freecam_speed / 2.f;
			}
			if (spoof_call(import_(GetAsyncKeyState), VK_SHIFT)) {
				camera_loc.z -= m_config.exploits.freecam_speed / 2.f;
			}

			spoof_call(oupdate_target_view, camera, vt, delta_time);
			vt->pov.location = camera_loc;
		}
	}
	else if (!freecam_state) freecam_state = true;

	auto mesh = pawn->mesh();
	if (!mesh) return;

	auto damage_handler = pawn->damage_handler();
	if (!damage_handler) return;

	if (!damage_handler->is_alive()) return;

	if (m_config.exploits.spinbot && !m_config.exploits.spinbot_server)
	{
		static float i = 0;

		if (m_config.exploits.spinbot_hotkey == 0)
		{
			i += m_config.exploits.spinbot_speed;
			if (i > 359) i = 0;

			mesh->k2_set_relative_rotation(frotator(1, i, 1));
		}
		else
		{
			if (spoof_call(import_(GetAsyncKeyState), m_config.exploits.spinbot_hotkey))
			{
				i += m_config.exploits.spinbot_speed;
				if (i > 359) i = 0;

				mesh->k2_set_relative_rotation(frotator(1, i, 1));
			}
			else
			{
				mesh->k2_set_relative_rotation(frotator(1, i, 1));
			}
		}
	}

	if (m_config.exploits.spinbot && m_config.exploits.spinbot_server)
	{
		if (spoof_call(import_(GetAsyncKeyState), m_config.exploits.spinbot_hotkey))
		{
			static float i = 0;

			i += m_config.exploits.spinbot_speed;
			if (i > 359) i = 0;

			local_controller->add_yaw_input(i / -m_config.exploits.spinbot_speed);
			return;
		}
	}

	fvector output_angle = fvector();

	if (m_config.exploits.third_person)
	{
		spoof_call(oupdate_target_view, camera, vt, delta_time);
		math::angle_to_vectors(fvector(-vt->pov.rotation.pitch, vt->pov.rotation.yaw, vt->pov.rotation.roll), &output_angle);

		output_angle *= 350;
		output_angle.z -= m_config.exploits.third_distance;
	}

	if (m_config.exploits.third_person)
	{
		fvector head_pos = mesh->bone_location(8);

		if (!m_config.exploits.third_person_hotkey) {
			pawn->set_3p_mesh_visible(true);

			spoof_call(oupdate_target_view, camera, vt, delta_time);
			vt->pov.location = head_pos - output_angle;
		}
		else {
			if (spoof_call(import_(GetAsyncKeyState), m_config.exploits.third_person_hotkey) & 1)
				m_config.exploits.third_person_state = !m_config.exploits.third_person_state;

			if (m_config.exploits.third_person_state) {
				spoof_call(oupdate_target_view, camera, vt, delta_time);
				vt->pov.location = head_pos - output_angle;
			}
			pawn->set_3p_mesh_visible(m_config.exploits.third_person_state);
		}
	}
	else pawn->set_3p_mesh_visible(false);
}