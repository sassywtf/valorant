#pragma once
#include <windows.h>
#include <winternl.h>
#include <vector>
#include <intrin.h>

#include "..\spoofer\spoofer.hpp"
#include "..\globals.hpp"
#include "structs.hpp"

// classes
class uclass
{
public:
	BYTE _padding_0[0x40];
	uclass* super_class;
};

class uobject
{
public:
	void** vft;
	int32_t flags;
	int32_t index;
	uclass* class_private;
	fname name;
	char pad_4[0x4];
	uobject* outer;

	template<typename ReturnTy = void, typename ...ArgumentsTy>
	__forceinline decltype(auto) call_virtual(const std::size_t index, ArgumentsTy ...arguments) const {
		return static_cast<ReturnTy(*)(decltype(this), ArgumentsTy...)>(this->vft[index])(this, arguments...);
	}

	template<typename type = uobject>
	static inline type* find_object(const wchar_t* name, uobject* outer = (uobject*)-1, bool exact = false) {
		typedef type* (__fastcall* static_find_object)(std::nullptr_t, uobject*, const wchar_t*, bool);
		auto static_find_object_fn = (static_find_object)(globals::base_address + globals::static_find_object);
		return spoof_call(static_find_object_fn, nullptr, outer, name, exact);
	}

	template<typename type = uobject>
	static inline type* load_object(const wchar_t* name, uobject* outer = (uobject*)-1, uobject* object_class = nullptr, const wchar_t* file_name = nullptr, UINT32 load_flags = 0, uintptr_t sandbox = 0, bool allow_reconciliation = true, uintptr_t instancing_context = 0) {
		typedef type* (__fastcall* static_load_object)(uobject*, uobject*, const wchar_t*, const wchar_t*, UINT32, uintptr_t, bool, uintptr_t);
		auto static_load_object_fn = (static_load_object)(globals::base_address + globals::static_load_object);
		return spoof_call(static_load_object_fn, object_class, outer, name, file_name, load_flags, sandbox, allow_reconciliation, instancing_context);
	}

	void process_event(uobject* function, void* args) {
		typedef void(__fastcall* process_event)(uobject*, uobject*, void*);
		auto process_event_fn = (process_event)(globals::base_address + globals::process_event);
		return spoof_call(process_event_fn, this, function, args);
	}

	bool is_a(uobject* klass) {
		if (!this || !klass) return false;

		for (auto super_class = memory::read<uobject*>(uintptr_t(this) + 0x10); super_class; super_class = memory::read<uobject*>(uintptr_t(super_class) + 0x48))
			if (super_class == klass) 
				return true;

		return false;
	}
};

class utexture2d : public uobject {};

class rendering : public uobject
{
public:
	static rendering* static_class() {
		static rendering* klass = 0;
		if (!klass)
			klass = (rendering*)uobject::find_object(_(L"Engine.KismetRenderingLibrary"));

		return klass;
	}

	utexture2d* import_file_as_texture_2d(uobject* world_context, fstring file_name);
};

class string : public uobject
{
public:
	enum search_case : std::uint8_t {
		case_sensitive, ignore_case
	};

	static string* static_class() {
		static string* klass = 0;
		if (!klass)
			klass = (string*)uobject::find_object(_(L"Engine.KismetStringLibrary"));

		return klass;
	}

	fname string_to_name(fstring in_string);
	bool starts_with(const wchar_t* source, const wchar_t* prefix, const enum_as_byte<search_case>& search_case = string::ignore_case);
	bool contains(const wchar_t* search_in, const wchar_t* sub_string, bool use_case = false, bool search_from_end = false);
};

class text : public uobject
{
public:
	static text* static_class() {
		static text* klass = 0;
		if (!klass)
			klass = (text*)uobject::find_object(_(L"Engine.KismetTextLibrary"));

		return klass;
	}

	fstring text_to_string(ftext in_text);
	ftext string_to_text(fstring in_string);
	fstring name_to_string(fname in_name);
};

class system : public uobject
{
public:
	static system* static_class() {
		static system* klass = 0;
		if (!klass)
			klass = (system*)uobject::find_object(_(L"Engine.KismetSystemLibrary"));

		return klass;
	}

	std::string get_object_name(uobject* object);
	fhit_result line_trace_single(uobject* world_context, apawn* local_actor, fvector start, fvector end);
};

class math : public uobject
{
public:
	static math* static_class() {
		static math* klass = 0;
		if (!klass)
			klass = (math*)uobject::find_object(_(L"Engine.KismetMathLibrary"));

		return klass;
	}

	static float deg_to_rad(float degrees) {
		float radians;
		radians = degrees * (float)(M_PI / 180);
		return radians;
	}

	static void angle_to_vectors(const fvector& angles, fvector* forward) {
		float sp, sy, cp, cy;
		sy = sin(deg_to_rad(angles.y));
		cy = cos(deg_to_rad(angles.y));
		sp = sin(deg_to_rad(angles.x));
		cp = cos(deg_to_rad(angles.x));
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	frotator normalized_delta_rotator(frotator a, frotator b);
	void to_vector_normalize(fvector in, fvector* out);
	void to_angle_normalize(fvector in, fvector* out);
	bool class_is_child_of(uobject* test_class, uobject* parent_class);
};

class gameplay_statics : public uobject
{
public:
	static gameplay_statics* static_class() {
		static gameplay_statics* klass = 0;
		if (!klass)
			klass = (gameplay_statics*)uobject::find_object(_(L"Engine.GameplayStatics"));

		return klass;
	}

	template<typename type>
	tarray<type*> get_all_actors_of_class(uobject* world_context, uclass* filter) {
		if (!this || !world_context || !filter) return {};

		static uobject* fn = nullptr;
		if (!fn)
			fn = uobject::find_object(_(L"Engine.GameplayStatics.GetAllActorsOfClass"));

		struct {
			uobject* world_context;
			uclass* filter;
			tarray<type*> return_value;
		} params = { world_context, filter };

		process_event(fn, &params);
		return params.return_value;
	}
};

class material_instance_dynamic : public uobject 
{
public:
	void set_vector_parameter_value(fname parameter_name, flinearcolor value);
	void set_texture_parameter_value(fname parameter_name, utexture2d* value);
	void set_scalar_parameter_value(fname parameter_name, float value);
};

class material_library : public uobject {
public:
	static material_library* static_class() {
		static material_library* klass = 0;
		if (!klass)
			klass = (material_library*)uobject::find_object(_(L"Engine.KismetMaterialLibrary"));

		return klass;
	}

	enum class emid_creation_flags : uint8_t {
		none = 0,
		transient = 1,
		emid_creation_flags_max = 2
	};

	material_instance_dynamic* create_dynamic_material_instance(uobject* world_context, uobject* parent, const fname& optional_name, emid_creation_flags creation_flags);
};

class ugame_viewport_client : public uobject
{
public:
	uworld* world();
	ugame_instance* game_instance();
	uengine* engine();
};

class uscene_component : public uobject
{
public:
	fvector relative_location();
	frotator relative_rotation();

	void set_relative_scale(fvector new_scale);
	void k2_set_relative_rotation(frotator new_rotation);
	void set_visibility(bool new_visibility, bool propagate_to_children);
};

class uprimitive_component : public uscene_component
{
public:
	eares_outline_mode get_ares_outline_mode();
	int32_t get_num_materials();
	void set_material(int32_t element_index, uobject* material);
	void set_render_custom_depth(bool value);
	void set_outline_mode(eares_outline_mode value, bool propagate_to_children = true);
	void mark_render_state_dirty();
};

class umesh_component : public uprimitive_component {};

class uskinned_mesh_component : public umesh_component
{
public:
	static uintptr_t force_wireframe();
};

class uskeletal_mesh_component : public uskinned_mesh_component
{
public:
	fname get_bone_name(int32_t bone_index);
	fvector bone_location(int index);
	int32_t num_bones();
};

class aactor : public uobject
{
public:
	uscene_component* root_component();
	void get_actor_bounds(bool only_colliding_components, fvector* origin, fvector* box_extent, bool include_from_child_actors);
};

class aplayer_state : public aactor
{
public:
	fstring get_player_name();
};

class apawn : public aactor
{
public:
	aplayer_state* player_state();
};

class acharacter : public apawn
{
public:
	uskeletal_mesh_component* mesh();
	int32_t jump_current_count();
};

class acamera_manager : public uobject
{
public:
	fvector camera_location();
	frotator camera_rotation();
	void set_default_aspect_ratio(float val);
};

class uplayer_input : public uobject
{
public:
	void input_key(fkey key, bool pressed = true);
};

class acontroller : public aactor
{
public:
	apawn* pawn();
	frotator control_rotation();
	bool line_of_sight_to(aactor* actor, fvector viewpoint = fvector());
}; 

class aplayer_controller : public acontroller
{
public:
	acamera_manager* camera_manager();
	uplayer_input* player_input();
	fvector2d world_to_screen_ret(fvector world_location);
	bool world_to_screen(fvector world_location, fvector2d* screen_location);
	void add_pitch_input(float val);
	void add_yaw_input(float val);
};

class ulocal_player : public uobject
{
public:
	aplayer_controller* player_controller();
};

class ulevel : public uobject
{
public:
	tarray<aactor*> actors();
};

class ugame_instance : public uobject
{
public:
	tarray<ulocal_player*> local_players();
};

class agame_state : public uobject {};

class uworld : public uobject
{
public:
	static uworld* static_class() {
		// redact

		if (cache_uworld) return (uworld*)cache_uworld;
		return nullptr;
	}

	agame_state* game_state();
	ugame_instance* game_instance();
	ulevel* persistent_level();
};

class uengine : public uobject
{
public:
	static uengine* static_class() {
		static uengine* engine_klass = 0;
		if (!engine_klass) {
			auto engine_static = uworld::static_class();
			if (!engine_static) return nullptr;

			auto game_instance = engine_static->game_instance();
			if (!game_instance) return nullptr;

			engine_klass = (uengine*)game_instance->outer;
			if (!engine_klass) return nullptr;
		}

		return engine_klass;
	}

	ugame_viewport_client* game_viewport();
};

class ufont : public uobject
{
public:
	static ufont* get_default_font() {
		//return (ufont*)(*(uintptr_t*)(uintptr_t(0x0x0x0x0x00x);


		//deeeeeeref that hoe
	}
};

class ucanvas : public uobject
{
public:
	fvector2d size();
	fvector2d text_size(const wchar_t* text, ufont* render_font = ufont::get_default_font(), fvector2d scale = fvector2d(0.9f, 0.9f));

	void draw_text();
	void draw_line();
	void draw_box();
	void draw_circle();
};

inline aares_player_controller* local_controller = nullptr;
inline ucanvas* game_canvas = nullptr;
