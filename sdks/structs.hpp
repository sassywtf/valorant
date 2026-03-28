#pragma once
#include <map>
#include <set>
#include <iostream>
#include <corecrt_math_defines.h>
#include "..\spoofer\spoofer.hpp"

// im lazy

// defines
class aactor;
class aplayer_state;
class apawn;
class aplayer_controller;
class ulocal_player;
class ugame_instance;
class uworld;
class uengine;
class aares_player_controller;
class ashooter_character;

class ftextdata {
public:
	char pad_0x0000[0x28];  //0x0000
	wchar_t* Name;          //0x0028 
	__int32 Length;         //0x0030 
};

struct ftext {
	ftextdata* Data;
	char UnknownData[0x10];

	wchar_t* Get() const {
		if (Data)
			return Data->Name;

		return nullptr;
	}
};

class fname {
public:
	fname() : comparison_index(std::uint32_t()), number(std::int32_t()) {};
	fname(std::uint32_t index) : comparison_index(index), number(std::int32_t()) {};

	bool operator==(const fname& other) const noexcept {
		return this->comparison_index == other.comparison_index;
	};

public:
	std::uint32_t comparison_index;
	std::int32_t number;
	std::int32_t shift; // 0xc
};

class fkey {
public:
	fkey() : name() {}
	fkey(fname name) : name(name) {}

public:
	fname name;
	std::uint8_t details[24] = {};
};

template<class type> class enum_as_byte {
public:
	enum_as_byte() { }
	enum_as_byte(type value) : value(static_cast<std::uint8_t>(value)) { }

	explicit enum_as_byte(std::int32_t value) : value(static_cast<std::uint8_t>(value)) { }
	explicit enum_as_byte(std::uint8_t value) : value(value) { }

	operator type() const { return type(value); }
	type get() const { return type(value); }

private:
	std::uint8_t value;
};

template<class k, class e>
class tmap
{
public:
	k key;
	e element;
	char __pad0x[0x8];
};

template<class type> struct tarray {
public: friend struct fstring;
	  tarray() : data(nullptr), count(std::int32_t()), maxx(std::int32_t()) { }
	  tarray(type* data, std::int32_t count, std::int32_t maxx) : data(data), count(count), maxx(maxx) { }

	  const bool is_valid() const noexcept
	  {
		  return !(this->data == nullptr);
	  }

	  const std::int32_t size() const noexcept
	  {
		  return this->count;
	  }

	  type& operator[](std::int32_t index) noexcept
	  {
		  return this->data[index];
	  }

	  const type& operator[](std::int32_t index) const noexcept
	  {
		  return this->data[index];
	  }

	  bool is_valid_index(std::int32_t index) const noexcept
	  {
		  return index < this->size();
	  }

protected:
	type* data;
	std::int32_t count;
	std::int32_t maxx;
};

struct fstring : public tarray<wchar_t> {
	fstring() {}
	fstring(const wchar_t* other) {
		if (this->maxx = this->count = *other ? static_cast<std::int32_t>(std::wcslen(other)) + 1 : 0)
			this->data = const_cast<wchar_t*>(other);
	};

	const wchar_t* c_str() const {
		return this->data;
	}

	std::string to_string() const {
		auto length = std::wcslen(data);
		std::string str(length, '\0');
		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(data, data + length, '?', &str[0]);
		return str;
	}
};


struct fmatrix {
	float m[4][4];

	fmatrix multiply(fmatrix matrix)
	{
		fmatrix pOut;
		pOut.m[0][0] = m[0][0] * matrix.m[0][0] + m[0][1] * matrix.m[1][0] + m[0][2] * matrix.m[2][0] + m[0][3] * matrix.m[3][0];
		pOut.m[0][1] = m[0][0] * matrix.m[0][1] + m[0][1] * matrix.m[1][1] + m[0][2] * matrix.m[2][1] + m[0][3] * matrix.m[3][1];
		pOut.m[0][2] = m[0][0] * matrix.m[0][2] + m[0][1] * matrix.m[1][2] + m[0][2] * matrix.m[2][2] + m[0][3] * matrix.m[3][2];
		pOut.m[0][3] = m[0][0] * matrix.m[0][3] + m[0][1] * matrix.m[1][3] + m[0][2] * matrix.m[2][3] + m[0][3] * matrix.m[3][3];
		pOut.m[1][0] = m[1][0] * matrix.m[0][0] + m[1][1] * matrix.m[1][0] + m[1][2] * matrix.m[2][0] + m[1][3] * matrix.m[3][0];
		pOut.m[1][1] = m[1][0] * matrix.m[0][1] + m[1][1] * matrix.m[1][1] + m[1][2] * matrix.m[2][1] + m[1][3] * matrix.m[3][1];
		pOut.m[1][2] = m[1][0] * matrix.m[0][2] + m[1][1] * matrix.m[1][2] + m[1][2] * matrix.m[2][2] + m[1][3] * matrix.m[3][2];
		pOut.m[1][3] = m[1][0] * matrix.m[0][3] + m[1][1] * matrix.m[1][3] + m[1][2] * matrix.m[2][3] + m[1][3] * matrix.m[3][3];
		pOut.m[2][0] = m[2][0] * matrix.m[0][0] + m[2][1] * matrix.m[1][0] + m[2][2] * matrix.m[2][0] + m[2][3] * matrix.m[3][0];
		pOut.m[2][1] = m[2][0] * matrix.m[0][1] + m[2][1] * matrix.m[1][1] + m[2][2] * matrix.m[2][1] + m[2][3] * matrix.m[3][1];
		pOut.m[2][2] = m[2][0] * matrix.m[0][2] + m[2][1] * matrix.m[1][2] + m[2][2] * matrix.m[2][2] + m[2][3] * matrix.m[3][2];
		pOut.m[2][3] = m[2][0] * matrix.m[0][3] + m[2][1] * matrix.m[1][3] + m[2][2] * matrix.m[2][3] + m[2][3] * matrix.m[3][3];
		pOut.m[3][0] = m[3][0] * matrix.m[0][0] + m[3][1] * matrix.m[1][0] + m[3][2] * matrix.m[2][0] + m[3][3] * matrix.m[3][0];
		pOut.m[3][1] = m[3][0] * matrix.m[0][1] + m[3][1] * matrix.m[1][1] + m[3][2] * matrix.m[2][1] + m[3][3] * matrix.m[3][1];
		pOut.m[3][2] = m[3][0] * matrix.m[0][2] + m[3][1] * matrix.m[1][2] + m[3][2] * matrix.m[2][2] + m[3][3] * matrix.m[3][2];
		pOut.m[3][3] = m[3][0] * matrix.m[0][3] + m[3][1] * matrix.m[1][3] + m[3][2] * matrix.m[2][3] + m[3][3] * matrix.m[3][3];
		return pOut;
	}
};

struct frotator {
	float pitch = 0.f;
	float yaw = 0.f;
	float roll = 0.f;

	frotator() : pitch(0.f), yaw(0.f), roll(0.f) {}
	frotator(float pitch, float yaw) : pitch(pitch), yaw(yaw), roll(0.f) {}
	frotator(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll) {}

	frotator operator + (const frotator& other) const { return { this->pitch + other.pitch, this->yaw + other.yaw, this->roll + other.roll }; }
	frotator operator - (const frotator& other) const { return { this->pitch - other.pitch, this->yaw - other.yaw, this->roll - other.roll }; }
	frotator operator * (float scalar) const { return { this->pitch * scalar, this->yaw * scalar, this->roll * scalar }; }
	frotator operator / (float divide) const { return { this->pitch / divide, this->yaw / divide, this->roll / divide }; }

	frotator& operator *= (const float other) { this->yaw *= other; this->pitch *= other; this->roll *= other; return *this; }
	frotator& operator /= (const float other) { this->yaw /= other; this->pitch /= other; this->roll /= other; return *this; }

	frotator& operator =  (const frotator& other) { this->pitch = other.pitch; this->yaw = other.yaw; this->roll = other.roll; return *this; }
	frotator& operator += (const frotator& other) { this->pitch += other.pitch; this->yaw += other.yaw; this->roll += other.roll; return *this; }
	frotator& operator -= (const frotator& other) { this->pitch -= other.pitch; this->yaw -= other.yaw; this->roll -= other.roll; return *this; }
	frotator& operator *= (const frotator& other) { this->pitch *= other.pitch; this->yaw *= other.yaw; this->roll *= other.roll; return *this; }
	frotator& operator /= (const frotator& other) { this->pitch /= other.pitch; this->yaw /= other.yaw; this->roll /= other.roll; return *this; }

	friend bool operator==(const frotator& first, const frotator& second) {
		return first.pitch == second.pitch && first.yaw == second.yaw && first.roll == second.roll;
	}

	friend bool operator!=(const frotator& first, const frotator& second) {
		return !(first == second);
	}

	inline void clamp_angles() {
		if (pitch > 75.f) pitch = 75.f;
		else if (pitch < -75.f) pitch = -75.f;
		if (roll < -180) roll += 360.0f;
		else if (roll > 180) roll -= 360.0f;

		yaw = 0.f;
	}

	inline frotator forward() const {
		float sp{}, sy{}, cp{}, cy{};
		float angle{};

		angle = yaw * (M_PI / 180.0f);
		sy = crt::sinf(angle);
		cy = crt::cosf(angle);

		angle = -pitch * (M_PI / 180.0f);
		sp = crt::sinf(angle);
		cp = crt::cosf(angle);

		return { cp * cy, cp * sy, -sp };
	}

	inline fmatrix matrix(frotator origin = { 0, 0, 0 }) {
		float radPitch = (float)(pitch * float(M_PI) / 180.f);
		float radYaw = (float)(yaw * float(M_PI) / 180.f);
		float radRoll = (float)(roll * float(M_PI) / 180.f);

		float SP = crt::sinf(radPitch);
		float CP = crt::cosf(radPitch);
		float SY = crt::sinf(radYaw);
		float CY = crt::cosf(radYaw);
		float SR = crt::sinf(radRoll);
		float CR = crt::cosf(radRoll);

		fmatrix ret;

		ret.m[0][0] = CP * CY;
		ret.m[0][1] = CP * SY;
		ret.m[0][2] = SP;
		ret.m[0][3] = 0.f;

		ret.m[1][0] = SR * SP * CY - CR * SY;
		ret.m[1][1] = SR * SP * SY + CR * CY;
		ret.m[1][2] = -SR * CP;
		ret.m[1][3] = 0.f;

		ret.m[2][0] = -(CR * SP * CY + SR * SY);
		ret.m[2][1] = CY * SR - CR * SP * SY;
		ret.m[2][2] = CR * CP;
		ret.m[2][3] = 0.f;

		ret.m[3][0] = (float)origin.pitch;
		ret.m[3][1] = (float)origin.yaw;
		ret.m[3][2] = (float)origin.roll;
		ret.m[3][3] = 1.f;

		return ret;
	}

	inline frotator normalize_angle() {
		frotator ret = *this;

		if (ret.pitch > 89)
			ret.pitch = 89;

		if (ret.pitch < -89)
			ret.pitch = -89;

		while (ret.yaw > 180)
			ret.yaw -= 360;

		while (ret.yaw < -180)
			ret.yaw += 360;

		ret.roll = 0;

		return ret;
	}
};

struct fvector2d {
	float x = 0.f;
	float y = 0.f;

	fvector2d() : x(0.f), y(0.f) {}
	fvector2d(float x, float y) : x(x), y(y) {}

	fvector2d operator + (const fvector2d& other) const { return { this->x + other.x, this->y + other.y }; }
	fvector2d operator - (const fvector2d& other) const { return { this->x - other.x, this->y - other.y }; }
	fvector2d operator * (float scalar) const { return { this->x * scalar, this->y * scalar }; }
	fvector2d operator / (float divide) const { return { this->x / divide, this->y / divide }; }

	fvector2d& operator *= (const float other) { this->x *= other; this->y *= other; return *this; }
	fvector2d& operator /= (const float other) { this->x /= other; this->y /= other; return *this; }

	fvector2d& operator =  (const fvector2d& other) { this->x = other.x; this->y = other.y; return *this; }
	fvector2d& operator += (const fvector2d& other) { this->x += other.x; this->y += other.y; return *this; }
	fvector2d& operator -= (const fvector2d& other) { this->x -= other.x; this->y -= other.y; return *this; }
	fvector2d& operator *= (const fvector2d& other) { this->x *= other.x; this->y *= other.y; return *this; }
	fvector2d& operator /= (const fvector2d& other) { this->x /= other.x; this->y /= other.y; return *this; }

	friend bool operator==(const fvector2d& first, const fvector2d& second)
	{
		return first.x == second.x && first.y == second.y;
	}

	friend bool operator!=(const fvector2d& first, const fvector2d& second)
	{
		return !(first == second);
	}

	inline float distance_from_point(float x1, float y1) {
		return (abs(x - x1) + abs(y - y1));
	}
};

struct fvector {
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	fvector() : x(0.f), y(0.f), z(0.f) {}
	fvector(float x, float y, float z) : x(x), y(y), z(z) {}
	fvector(const frotator& rotator) : x(rotator.pitch), y(rotator.yaw), z(rotator.roll) {}

	fvector operator + (const fvector& other) const { return { this->x + other.x, this->y + other.y, this->z + other.z }; }
	fvector operator - (const fvector& other) const { return { this->x - other.x, this->y - other.y, this->z - other.z }; }
	fvector operator * (float scalar) const { return { this->x * scalar, this->y * scalar, this->z * scalar }; }
	fvector operator / (float divide) const { return { this->x / divide, this->y / divide, this->z / divide }; }

	fvector& operator *= (const float other) { this->x *= other; this->y *= other; this->z *= other; return *this; }
	fvector& operator /= (const float other) { this->x /= other; this->y /= other; this->z /= other; return *this; }

	fvector& operator =  (const fvector& other) { this->x = other.x; this->y = other.y; this->z = other.z; return *this; }
	fvector& operator += (const fvector& other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
	fvector& operator -= (const fvector& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
	fvector& operator *= (const fvector& other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
	fvector& operator /= (const fvector& other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }

	friend bool operator==(const fvector& first, const fvector& second) {
		return first.x == second.x && first.y == second.y && first.z == second.z;
	}

	friend bool operator!=(const fvector& first, const fvector& second)
	{
		return !(first == second);
	}

	inline float length() {
		return crt::sqrtf((x * x) + (y * y) + (z * z));
	}

	inline float dot(fvector v) {
		return x * v.x + y * v.y + z * v.z;
	}

	inline void clamp() {
		if (x > 75.f) x = 75.f;
		else if (x < -75.f) x = -75.f;
		if (z < -180) z += 360.0f;
		else if (z > 180) z -= 360.0f;

		y = 0.f;
	}

	inline float distance(const fvector& vector) {
		float xCoord = vector.x - x;
		float yCoord = vector.y - y;
		float zCoord = vector.z - z;
		return crt::sqrtf((xCoord * xCoord) + (yCoord * yCoord) + (zCoord * zCoord));
	}

	inline frotator calculate_angles(fvector target_location) {
		fvector vector_pos = target_location - *this;
		float distance = (float)(crt::sqrtf(vector_pos.x * vector_pos.x + vector_pos.y * vector_pos.y + vector_pos.z * vector_pos.z));

		frotator rot;
		rot.pitch = -((crt::acosf(vector_pos.z / distance) * (float)(180.0f / M_PI)) - 90.f);
		rot.yaw = crt::atan2f(vector_pos.y, vector_pos.x) * (float)(180.0f / M_PI);

		return rot;
	}

	inline frotator as_rotator() {
		return frotator(x, y, z);
	}
};

struct flinearcolor {
	float red;
	float green;
	float blue;
	float alpha;
};

struct fquat {
	float x;
	float y;
	float z;
	float w;
};

struct fmatrix2 {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
};

struct ftransform {
	fquat rot;
	fvector translation;
	char pad_0001[4];
	fvector scale;
	char pad_0002[4];

	fmatrix2 ToMatrixWithScale()
	{
		fmatrix2 m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

__forceinline fmatrix2 MatrixMultiplication(fmatrix2 pM1, fmatrix2 pM2) {
	fmatrix2 pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

enum class ecollision_channel : uint8_t {
	ecc_world_static = 0,
	ecc_world_dynamic = 1,
	ecc_pawn = 2,
	ecc_visibility = 3,
	ecc_camera = 4,
	ecc_physics_body = 5,
	ecc_vehicle = 6,
	ecc_destructible = 7,
	ecc_engine_trace_channel1 = 8,
	ecc_engine_trace_channel2 = 9,
	ecc_engine_trace_channel3 = 10,
	ecc_engine_trace_channel4 = 11,
	ecc_engine_trace_channel5 = 12,
	ecc_engine_trace_channel6 = 13,
	ecc_game_trace_channel1 = 14,
	ecc_game_trace_channel2 = 15,
	ecc_game_trace_channel3 = 16,
	ecc_game_trace_channel4 = 17,
	ecc_game_trace_channel5 = 18,
	ecc_game_trace_channel6 = 19,
	ecc_game_trace_channel7 = 20,
	ecc_game_trace_channel8 = 21,
	ecc_game_trace_channel9 = 22,
	ecc_game_trace_channel10 = 23,
	ecc_game_trace_channel11 = 24,
	ecc_game_trace_channel12 = 25,
	ecc_game_trace_channel13 = 26,
	ecc_game_trace_channel14 = 27,
	ecc_game_trace_channel15 = 28,
	ecc_game_trace_channel16 = 29,
	ecc_game_trace_channel17 = 30,
	ecc_game_trace_channel18 = 31,
	ecc_overlap_all_deprecated = 32,
	ecc_max = 33
};

enum class eares_outline_mode : uint8_t {
	none = 0,
	outline = 1,
	block = 2,
	enemy = 3,
	always_outline = 4,
	always_enemy = 5,
	eares_outline_mode_max = 6
};

struct fhealth_value {
	float life;
	float maximum_life;
};
static_assert(sizeof(fhealth_value) == 0x08);

struct fminimal_view_info {
	fvector location;
	frotator rotation;
	float fov;
	char pad_1c[0x684];
};
static_assert(sizeof(fminimal_view_info) == 0x6a0);

struct ftview_target {
	aactor* target;
	char pad_8[0x8];
	fminimal_view_info pov;
	aplayer_state* playerstate;
	char pad_6b8[0x8];
};
static_assert(sizeof(ftview_target) == 0x6c0);

struct fares_game_info {
	float display_remaining_time;
	float local_display_remaining_time;
	bool locally_predict_time_advancing;
	char pad_9[0x3];
	float game_phase_elapsed_time;
	bool is_store_enabled;
	bool is_infinite_abilities_enabled;
	bool is_infinite_ammo_enabled;
	bool is_infinite_reserve_enabled;
	bool is_infinite_money_enabled;
	bool is_ignore_shopping_restrictions_enabled;
	bool is_match_timer_paused;
	bool is_intention_enabled;
	float auth_gameplay_start_timestamp;
	float auth_gameplay_end_timestamp;
	float local_gameplay_start_timestamp;
	float local_gameplay_end_timestamp;
	bool gameplay_active;
	bool is_drop_in_enabled;
	char pad_2a[0x6];
	char pad_08[0x8];
	float match_timeout_expire_time_stamp;
	char pad_3d[0x4];
};
static_assert(sizeof(fares_game_info) == 0x40);

struct alignas(0x08) fhit_result final
{
public:
	uint8_t blocking_hit : 1;
	uint8_t start_penetrating : 1;
	uint8_t pad_1[0x3];
	int32_t face_index;
	float time;
	float distance;
	fvector location;
	fvector impact_point;
	fvector normal;
	fvector impact_normal;
	fvector trace_start;
	fvector trace_end;
	float penetration_depth;
	int32_t item;
	uint8_t element_index;
	uint8_t pad_61[0x3];
	uint64_t phys_material;
	uint64_t actor;
	uint64_t component;
	char bone_name[8];
	char my_bone_name[8];
	uint8_t pad_8c[8];
};