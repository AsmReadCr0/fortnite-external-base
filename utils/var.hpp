#pragma once

class config_
{
public:
	bool render_menu = true;
	struct
	{
		int aim_key = VK_RBUTTON;
		bool aimbot = true;
		bool prediction = true;
		bool held_weapon = false;
		int fov = 200;
		int smooth = 5;
		int aimbone = 0;

		struct
		{
			bool fov = true;
			bool line = false;
		}draw;
	}aim;

	struct
	{
		
		bool box = true;
		bool filled_box = false;
		bool skeleton = false;
		bool name = true;
		bool distance = true;
		bool fov = true;
		bool visible_check = true;
	}visual;

	struct
	{
		bool show_menu = true;
		bool vsync = true;
		bool water_mark = true;
		bool fps = true;
	}misc;
};
inline std::unique_ptr<config_> config = std::make_unique<config_>();