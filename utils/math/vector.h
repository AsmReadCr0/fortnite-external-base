#pragma once
#include <cstdint>
#include <vector>
#include <d3d9.h>
#include <iostream>
#include <sstream>
#include <random>
#include <iomanip>
#include <unordered_map>

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	double x;
	double y;
	double z;

	inline double Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline double Distance(Vector3 v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double Length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(double number) const
	{
		return Vector3(x * number, y * number, z * number);
	}

	void addScaled(const Vector3& v, float scale)
	{
		x += v.x * scale;
		y += v.y * scale;
		z += v.z * scale;
	}

	bool is_zero()
	{
		if (x == 0 && y == 0 && z == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

inline const double pi_ = 3.14159265358979323846;
inline const double pi_2 = 1.570796f;
inline const double pi_4 = 0.7853982f;

struct
{
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
}monitor;

//beware shit code below <>/<>
inline std::string xor_generator(size_t length) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 255);

	std::stringstream ss;
	for (size_t i = 0; i < length; ++i)
	{
		ss << std::hex << std::setw(2) << std::setfill('0') << distr(gen);
	}
	return ss.str();
}

inline double FastArcTan(double x) {
	return pi_4 * x - x * (fabs(x) - 1) * (0.2447 + 0.0663 * fabs(x));
}

inline double FastArcTan2(double y, double x) {
	if (x >= 0) {
		if (y >= 0) {
			if (y < x) {
				return FastArcTan(y / x);
			}
			else {
				return pi_2 - FastArcTan(x / y);
			}
		}
		else {
			if (-y < x) {
				return FastArcTan(y / x);
			}
			else {
				return -pi_2 - FastArcTan(x / y);
			}
		}
	}
	else {
		if (y >= 0) {
			if (y < -x) {
				return FastArcTan(y / x) + pi_;
			}
			else {
				return pi_2 - FastArcTan(x / y);
			}
		}
		else {
			if (-y < -x) {
				return FastArcTan(y / x) - pi_;
			}
			else {
				return -pi_2 - FastArcTan(x / y);
			}
		}
	}
}

inline std::unordered_map<float, float> asin_table;
inline std::unordered_map<float, float> atan_table;

inline void init_asin_table() {
	for (float i = -1.0; i <= 1.0; i += 0.001) {
		asin_table[i] = std::asin(i);
	}
}

inline void init_atan_table() {
	for (float i = -10.0; i <= 10.0; i += 0.001) {
		atan_table[i] = std::atan(i);
	}
}

inline float fast_asin(float x) {
	if (x <= -1.0) {
		return -pi_ / 2;
	}
	else if (x >= 1.0) {
		return pi_ / 2;
	}
	else {
		auto it = asin_table.find(x);
		if (it != asin_table.end()) {
			return it->second;
		}
		float x1 = floor(x * 1000) / 1000;
		float x2 = ceil(x * 1000) / 1000;
		float y1 = asin_table[x1];
		float y2 = asin_table[x2];
		return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
	}
}

inline float fast_atan(float x) {
	if (x <= -10.0) {
		return -pi_ / 2;
	}
	else if (x >= 10.0) {
		return pi_ / 2;
	}
	else {
		auto it = atan_table.find(x);
		if (it != atan_table.end()) {
			return it->second;
		}

		float x1 = floor(x * 1000) / 1000;
		float x2 = ceil(x * 1000) / 1000;
		float y1 = atan_table[x1];
		float y2 = atan_table[x2];
		return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
	}
}