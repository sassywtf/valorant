#include "engine.hpp"

utexture2d* rendering::import_file_as_texture_2d(uobject* world_context, fstring file_name) {
	if (!this || !world_context) return nullptr;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetRenderingLibrary.ImportFileAsTexture2D"));

	struct {
		uobject* world_context;
		fstring file_name;
		utexture2d* return_value;
	} params = { world_context, file_name };

	process_event(fn, &params);
	return params.return_value;
}

//

fname string::string_to_name(fstring in_string) {
	if (!this) return {};

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetStringLibrary.Conv_StringToName"));

	struct {
		fstring in_string;
		fname return_value;
	} params = { in_string };

	process_event(fn, &params);
	return params.return_value;
}

bool string::starts_with(const wchar_t* source, const wchar_t* prefix, const enum_as_byte<search_case>& search_case) {
	if (!this) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetStringLibrary.StartsWith"));

	struct {
		fstring source;
		fstring prefix;
		std::uint8_t search_case;
		bool return_value;
	} params = { source, prefix, search_case.get() };

	process_event(fn, &params);
	return params.return_value;
}

bool string::contains(const wchar_t* search_in, const wchar_t* sub_string, bool use_case, bool search_from_end) {
	if (!this) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetStringLibrary.Contains"));

	struct {
		fstring search_in;
		fstring sub_string;
		bool use_case;
		bool search_from_end;
		bool return_value;
	} params = { search_in, sub_string, use_case, search_from_end };

	process_event(fn, &params);
	return params.return_value;
}

//

fstring text::text_to_string(ftext in_text) {
	if (!this) return {};

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetTextLibrary.Conv_TextToString"));

	struct {
		ftext in_text;
		fstring return_value;
	} params = { in_text };

	process_event(fn, &params);
	return params.return_value;
}

ftext text::string_to_text(fstring in_string) {
	if (!this) return {};

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetTextLibrary.Conv_StringToText"));

	struct {
		fstring in_string;
		ftext return_value;
	} params = { in_string };

	process_event(fn, &params);
	return params.return_value;
}

fstring text::name_to_string(fname in_name) {
	if (!this) return {};

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetStringLibrary.Conv_NameToString"));

	struct {
		fname in_name;
		fstring return_value;
	} params = { in_name };

	process_event(fn, &params);
	return params.return_value;
}

//

std::string system::get_object_name(uobject* object) {
	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetSystemLibrary.GetObjectName"));

	struct {
		uobject* object;
		fstring return_value;
	} params = { object };

	process_event(fn, &params);

	if (params.return_value.c_str() == 0) return _("None");

	std::string real_return = params.return_value.to_string();

	if (params.return_value.c_str() != 0)
		uobject::free_memory((__int64)params.return_value.c_str());

	return real_return;
}

fhit_result system::line_trace_single(uobject* world_context, apawn* local_actor, fvector start, fvector end) {
	if (!this) return fhit_result();

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetSystemLibrary.LineTraceSingle"));

	struct {
		uobject* world_context;
		fvector start;
		fvector end;
		ecollision_channel trace_channel;
		bool trace_complex;
		uint8_t pad_22[0x6];
		tarray<uint64_t> actors_to_ignore;
		uint8_t draw_debug_type;
		uint8_t pad_39[0x7];
		fhit_result out_hit;
		bool ignore_self;
		uint8_t pad_d9[0x3];
		flinearcolor trace_color;
		flinearcolor trace_hit_color;
		float draw_time;
		fstring in_trace_tag;
		bool return_value;
		uint8_t pad_111[0x7];
	} params{};

	tarray<uint64_t> objects;
	params.world_context = world_context;
	params.start = start;
	params.end = end;
	params.trace_channel = ecollision_channel::ecc_visibility;
	params.trace_complex = true;
	params.actors_to_ignore = objects;
	params.ignore_self = true;

	process_event(fn, &params);
	return params.out_hit;
}

//

frotator math::normalized_delta_rotator(frotator a, frotator b) {
	if (!this) return {};

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetMathLibrary.NormalizedDeltaRotator"));

	struct {
		fvector a;
		fvector b;
		frotator return_value;
	} params = { a, b };

	process_event(fn, &params);
	return params.return_value;
}

void math::to_vector_normalize(fvector in, fvector* out) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetMathLibrary.Conv_RotatorToVector"));

	struct {
		fvector in_rot;
		fvector out_rot;
	} params = { in };

	process_event(fn, &params);
	*out = params.out_rot;
}

void math::to_angle_normalize(fvector in, fvector* out) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetMathLibrary.Conv_VectorToRotator"));

	struct {
		fvector in_rot;
		fvector out_rot;
	} params = { in };

	process_event(fn, &params);
	*out = params.out_rot;
}

bool math::class_is_child_of(uobject* test_class, uobject* parent_class) {
	if (!this) return false;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetMathLibrary.ClassIsChildOf"));

	struct {
		uobject* test_class;
		uobject* parent_class;
		bool return_value;
	} params = { test_class, parent_class };

	process_event(fn, &params);
	return params.return_value;
}

//

void material_instance_dynamic::set_vector_parameter_value(fname parameter_name, flinearcolor value) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.MaterialInstanceDynamic.SetVectorParameterValue"));

	struct {
		fname parameter_name;
		flinearcolor value;
	} params = { parameter_name, value };

	process_event(fn, &params);
}

void material_instance_dynamic::set_texture_parameter_value(fname parameter_name, utexture2d* value) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.MaterialInstanceDynamic.SetTextureParameterValue"));

	struct {
		fname parameter_name;
		utexture2d* value;
	} params = { parameter_name, value };

	process_event(fn, &params);
}

void material_instance_dynamic::set_scalar_parameter_value(fname parameter_name, float value) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.MaterialInstanceDynamic.SetScalarParameterValue"));

	struct {
		fname parameter_name;
		float value;
	} params = { parameter_name, value };

	process_event(fn, &params);
}

//

material_instance_dynamic* material_library::create_dynamic_material_instance(uobject* world_context, uobject* parent, const fname& optional_name, emid_creation_flags creation_flags) {
	if (!this || !world_context || !parent) return nullptr;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.KismetMaterialLibrary.CreateDynamicMaterialInstance"));

	struct {
		uobject* world_context;
		uobject* parent;
		fname optional_name;
		emid_creation_flags creation_flags;
		material_instance_dynamic* return_value;
	} params = { world_context, parent, optional_name, creation_flags };

	process_event(fn, &params);
	return params.return_value;
}

//

uworld* ugame_viewport_client::world() {
	if (!this) return nullptr;

	return reinterpret_cast<uworld*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

ugame_instance* ugame_viewport_client::game_instance() {
	if (!this) return nullptr;

	return reinterpret_cast<ugame_instance*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uengine* ugame_viewport_client::engine() {
	if (!this) return nullptr;

	return reinterpret_cast<uengine*>(game_instance()->outer);
}

//

fvector uscene_component::relative_location() {
	if (!this) return fvector();


	return memory::read<fvector>(uintptr_t(this) + offset);
}

frotator uscene_component::relative_rotation() {
	if (!this) return frotator();

	return memory::read<frotator>(uintptr_t(this) + offset);
}

void uscene_component::set_relative_scale(fvector new_scale) {
	if (!this) return;

	memory::write<fvector>(uintptr_t(this) + offset, new_scale);
}

void uscene_component::k2_set_relative_rotation(frotator new_rotation) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.SceneComponent.K2_SetRelativeRotation"));

	struct {
		frotator new_rotation;
		bool sweep;
		uintptr_t* sweep_hit_result;
		bool teleport;
	} params = { new_rotation, false, nullptr, false };

	process_event(fn, &params);
}

void uscene_component::set_visibility(bool new_visibility, bool propagate_to_children) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.SceneComponent.SetVisibility"));

	struct {
		bool new_visibility;
		bool propagate_to_children;
	} params = { new_visibility, propagate_to_children };

	process_event(fn, &params);
}

//

eares_outline_mode uprimitive_component::get_ares_outline_mode() {
	if (!this) return eares_outline_mode::eares_outline_mode_max;

	return reinterpret_cast<eares_outline_mode>(memory::read<eares_outline_mode>(reinterpret_cast<long long>(this) + 0x330));
}

int32_t uprimitive_component::get_num_materials() {
	if (!this) return 0;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.PrimitiveComponent.GetNumMaterials"));

	struct {
		int32_t return_value;
	} params = { };

	process_event(fn, &params);
	return params.return_value;
}

void uprimitive_component::set_material(int32_t element_index, uobject* material) {
	if (!this || !material) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.PrimitiveComponent.SetMaterial"));

	struct {
		int32_t element_index;
		uobject* material;
	} params = { element_index, material };

	process_event(fn, &params);
}

void uprimitive_component::set_render_custom_depth(bool value) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.PrimitiveComponent.SetRenderCustomDepth"));

	struct {
		bool value;
	} params = { value };

	process_event(fn, &params);
}

void uprimitive_component::set_outline_mode(eares_outline_mode value, bool propagate_to_children) {
	if (!this) return;

	typedef void(__fastcall* set_outline_mode)(uprimitive_component*, eares_outline_mode, bool);
	auto set_outline_mode_fn = (set_outline_mode)(globals::base_address + globals::set_outline_mode);
	spoof_call(set_outline_mode_fn, this, value, propagate_to_children);
}

void uprimitive_component::mark_render_state_dirty() {
	if (!this) return;

	typedef void(__fastcall* mark_render_state_dirty)(uprimitive_component*);
	auto mark_render_state_dirty_fn = (mark_render_state_dirty)(globals::base_address + globals::mark_render_state_dirty);
	spoof_call(mark_render_state_dirty_fn, this);
}

//

uintptr_t uskinned_mesh_component::force_wireframe() {

	return offset;
}

//

fname uskeletal_mesh_component::get_bone_name(int32_t bone_index) {
	if (!this) return {};

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.SkinnedMeshComponent.GetBoneName"));

	struct {
		int32_t bone_index;
		fname return_value;
	} params = { bone_index };

	process_event(fn, &params);
	return params.return_value;
}

fvector uskeletal_mesh_component::bone_location(int index) {
	auto bone_array = memory::read<uintptr_t>(uintptr_t(this) + 0x5c8 + 0x10);
	if (!bone_array)
		bone_array = memory::read<uintptr_t>(uintptr_t(this) + 0x5c8);

	if (bone_array)
	{
		auto c2w = memory::read<ftransform>(uintptr_t(this) + 0x250);
		auto bone = memory::read<ftransform>(bone_array + (index * 0x30));
		auto matrix = MatrixMultiplication(bone.ToMatrixWithScale(), c2w.ToMatrixWithScale());
		return fvector(matrix._41, matrix._42, matrix._43);
	}

	return fvector();
}

int32_t uskeletal_mesh_component::num_bones() {
	if (!this) return 0;

	return memory::read<int32_t>(uintptr_t(this) + 0x5c8 + 0x8);
}

//

uscene_component* aactor::root_component() {
	if (!this) return nullptr;

	return reinterpret_cast<uscene_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

void aactor::get_actor_bounds(bool only_colliding_components, fvector* origin, fvector* box_extent, bool include_from_child_actors) {
	if (!this) return;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.Actor.GetActorBounds"));

	struct {
		bool only_colliding_components;
		fvector origin;
		fvector box_extent;
		bool include_from_child_actors;
	} params;

	params.only_colliding_components = only_colliding_components;
	params.include_from_child_actors = include_from_child_actors;

	process_event(fn, &params);
	if (origin) *origin = params.origin;
	if (box_extent) *box_extent = params.box_extent;
}

//

fstring aplayer_state::get_player_name() {
	if (!this) return nullptr;

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.PlayerState.GetPlayerName"));

	struct {
		fstring player_name;
	} params;

	process_event(fn, &params);
	return params.player_name;
}

//

aplayer_state* apawn::player_state() {
	if (!this) return nullptr;

	return reinterpret_cast<aplayer_state*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

//

uskeletal_mesh_component* acharacter::mesh() {
	if (!this) return nullptr;


	return reinterpret_cast<uskeletal_mesh_component*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

int32_t acharacter::jump_current_count() {
	if (!this) return 0;

	return memory::read<int32_t>(reinterpret_cast<long long>(this) + offset);
}

//

fvector acamera_manager::camera_location() {
	if (!this) return fvector();

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.PlayerCameraManager.GetCameraLocation"));

	struct {
		fvector return_value;
	} params;

	process_event(fn, &params);
	return params.return_value;
}

frotator acamera_manager::camera_rotation() {
	if (!this) return frotator();

	static uobject* fn = nullptr;
	if (!fn)
		fn = uobject::find_object(_(L"Engine.PlayerCameraManager.GetCameraRotation"));

	struct {
		frotator return_value;
	} params;

	process_event(fn, &params);
	return params.return_value;
}

void acamera_manager::set_default_aspect_ratio(float val) {
	if (!this) return;

	return memory::write<float>(reinterpret_cast<long long>(this) + offset, val);
}

//

void uplayer_input::input_key(fkey key, bool pressed) {
	if (!this) return;

	typedef void(__fastcall* input_key)(uplayer_input*, fkey, int, float, bool);
	auto input_key_fn = (input_key)(globals::base_address + globals::input_key);
	spoof_call(input_key_fn, this, key, pressed ? 0 : 1, 1.f, false);
}

//

apawn* acontroller::pawn() {
	if (!this) return nullptr;


	return reinterpret_cast<apawn*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

frotator acontroller::control_rotation() {
	if (!this) return frotator();

	return memory::read<frotator>(reinterpret_cast<long long>(this) + offset);
}

bool acontroller::line_of_sight_to(aactor* actor, fvector viewpoint) {
	if (!this || !actor) return false;

	typedef bool(__fastcall* line_of_sight_to)(acontroller*, aactor*, fvector, bool);
	auto line_of_sight_to_fn = (line_of_sight_to)(globals::base_address + globals::line_of_sight_to);
	return spoof_call(line_of_sight_to_fn, this, actor, viewpoint, false);
}

//

acamera_manager* aplayer_controller::camera_manager() {
	if (!this) return nullptr;


	return reinterpret_cast<acamera_manager*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

uplayer_input* aplayer_controller::player_input() {
	if (!this) return nullptr;

	return reinterpret_cast<uplayer_input*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

fvector2d aplayer_controller::world_to_screen_ret(fvector world_location) {
	if (!this) return fvector2d();

	fvector2d output{};
	if (world_to_screen(world_location, &output)) return output;
	return fvector2d();
}

bool aplayer_controller::world_to_screen(fvector world_location, fvector2d* screen_location) {
	if (!this) return false;

	typedef bool(__fastcall* world_to_screen)(aplayer_controller*, fvector, fvector2d*, bool, bool);
	auto world_to_screen_fn = (world_to_screen)(globals::base_address + globals::world_to_screen);
	return spoof_call(world_to_screen_fn, this, world_location, screen_location, false, false);
}

void aplayer_controller::add_pitch_input(float val) {
	if (!this) return;

	typedef void(__fastcall* add_pitch_input)(aplayer_controller*, float);
	auto add_pitch_input_fn = (add_pitch_input)(globals::base_address + globals::add_pitch_input);
	spoof_call(add_pitch_input_fn, this, val);
}

void aplayer_controller::add_yaw_input(float val) {
	if (!this) return;

	typedef void(__fastcall* add_yaw_input)(aplayer_controller*, float);
	auto add_yaw_input_fn = (add_yaw_input)(globals::base_address + globals::add_yaw_input);
	spoof_call(add_yaw_input_fn, this, val);
}

aplayer_controller* ulocal_player::player_controller() {
	if (!this) return nullptr;

	return reinterpret_cast<aplayer_controller*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

//

tarray<aactor*> ulevel::actors() {
	if (!this) return {};

	return memory::read<tarray<aactor*>>(reinterpret_cast<long long>(this) + 0xA0); // not reflected
}

//

tarray<ulocal_player*> ugame_instance::local_players() {
	if (!this) return {};

	return memory::read<tarray<ulocal_player*>>(reinterpret_cast<long long>(this) + offset);
}

//

agame_state* uworld::game_state() {
	if (!this) return nullptr;

	return reinterpret_cast<agame_state*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

ugame_instance* uworld::game_instance() {
	if (!this) return nullptr;


	return reinterpret_cast<ugame_instance*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

ulevel* uworld::persistent_level() {
	if (!this) return nullptr;


	return reinterpret_cast<ulevel*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

//

ugame_viewport_client* uengine::game_viewport() {
	if (!this) return nullptr;

	return reinterpret_cast<ugame_viewport_client*>(memory::read<long long>(reinterpret_cast<long long>(this) + offset));
}

//

fvector2d ucanvas::size() {
	return fvector2d(
		(float)spoof_call(import_(GetSystemMetrics), SM_CXSCREEN),
		(float)spoof_call(import_(GetSystemMetrics), SM_CYSCREEN)
	);
}

fvector2d ucanvas::text_size(const wchar_t* text, ufont* render_font, fvector2d scale) {
	if (!this) return {};

	fvector2d output{};
	typedef void(__fastcall* k2_text_size)(ucanvas*, fvector2d*, ufont*, fstring, fvector2d);
	auto k2_text_size_fn = (k2_text_size)(globals::base_address + globals::k2_text_size);
	spoof_call(k2_text_size_fn, this, &output, render_font, (fstring)text, scale);
	return output;
}

void ucanvas::draw_text(const wchar_t* text, fvector2d position, flinearcolor color, bool centerx, bool centery, bool outline, fvector2d scale, flinearcolor outline_color, ufont* render_font) {
	if (!this) return;

	typedef void(__fastcall* k2_draw_text)(ucanvas*, ufont*, fstring, fvector2d, fvector2d, flinearcolor, float, flinearcolor, fvector2d, bool, bool, bool, flinearcolor);
	auto k2_draw_text_fn = (k2_draw_text)(globals::base_address + globals::k2_draw_text);
	spoof_call(k2_draw_text_fn, this, render_font, (fstring)text, position, scale, color, 1.f, flinearcolor(), fvector2d(), centerx, centery, outline, outline_color);
}

void ucanvas::draw_line(fvector2d start, fvector2d end, flinearcolor color, float thickness) {
	if (!this) return;

	typedef void(__fastcall* k2_draw_line)(ucanvas*, fvector2d, fvector2d, float, flinearcolor);
	auto k2_draw_line_fn = (k2_draw_line)(globals::base_address + globals::k2_draw_line);
	spoof_call(k2_draw_line_fn, this, start, end, thickness, color);
}

void ucanvas::draw_box(fvector2d position, fvector2d size, flinearcolor color, float thickness) {
	if (!this) return;

	typedef void(__fastcall* k2_draw_box)(ucanvas*, fvector2d, fvector2d, float, flinearcolor);
	auto k2_draw_box_fn = (k2_draw_box)(globals::base_address + globals::k2_draw_box);
	spoof_call(k2_draw_box_fn, this, position, size, thickness, color);
}

void ucanvas::draw_circle(fvector2d position, int radius, flinearcolor color, int sides) {
	if (!this) return;

	float step = M_PI * 2.0 / sides;
	int count = 0;
	fvector2d v[128];

	for (float a = 0; a < M_PI * 2.0; a += step) {
		float x1 = radius * crt::cosf(a) + position.x;
		float y1 = radius * crt::sinf(a) + position.y;
		float x2 = radius * crt::cosf(a + step) + position.x;
		float y2 = radius * crt::sinf(a + step) + position.y;
		v[count].x = x1;
		v[count].y = y1;
		v[count + 1].x = x2;
		v[count + 1].y = y2;
		draw_line(fvector2d{ v[count].x, v[count].y }, fvector2d{ x2, y2 }, color, 1.0f);
	}
}