#pragma once
#include <xmmintrin.h>
#include <immintrin.h>
#include <corecrt_math.h>
#include <utils/includes.h>
#include <utils/encryption/callstack.h>
#include <utils/var.hpp>

class aim_
{
public:
	inline float powf_(float _X, float _Y)
	{
		return (_mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y))));
	}

	inline double get_cross_distance(double x1, double y1, double z1)
	{
		return sqrtf(powf((monitor.width / 2 - x1), 2) + powf_((monitor.height / 2 - y1), 2));
	}

	float center_x = monitor.width / 2;
	float center_y = monitor.height / 2;

	inline void perform(float x, float y)
	{
		float target_x = 0;
		float target_y = 0;

		if (x != 0.f)
		{
			target_x = (x > center_x) ? -(center_x - x) : x - center_x;
			target_x /= config->aim.smooth;
			target_x = (x > center_x && target_x + center_x > center_x * 2) ? 0 : target_x;
			target_x = (x < center_x && target_x + center_x < 0) ? 0 : target_x;
		}

		if (y != 0.f)
		{
			target_y = (y > center_y) ? -(center_y - y) : y - center_y;
			target_y /= config->aim.smooth;
			target_y = (y > center_y && target_y + center_y > center_y * 2) ? 0 : target_y;
			target_y = (y < center_y && target_y + center_y < 0) ? 0 : target_y;
		}

		// memory for an extra 30 kek
	}
};
inline aim_ aim;