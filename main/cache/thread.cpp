#include <main/cache/thread.h>
#include <main/actor.h>

void cache::thread()
{
	std::vector<entities> temp_stored;

	while (1)
	{
		world_data->uworld = efi.read<uint64_t>( efi.eprocess.image +offsets.uworld );

		world_data->game_instance = efi.read<uintptr_t>(world_data->uworld + offsets.game_instance);

		world_data->local_players = efi.read<uint64_t>(world_data->game_instance + offsets.local_player);

		world_data->local_player = efi.read<uint64_t>(world_data->local_players);

		world_data->local_player_state = efi.read<uintptr_t>(world_data->local_player + offsets.player_state);

		auto local_team_id = efi.read<int>(world_data->local_player_state + offsets.teamindex);

		world_data->player_controller = efi.read<uintptr_t>(world_data->local_player + offsets.player_controller);
		world_data->local_pawn = efi.read<uintptr_t>(world_data->player_controller + offsets.local_pawn);
		world_data->game_state = efi.read<uintptr_t>(world_data->uworld + offsets.game_state);
		world_data->player_array = efi.read<uintptr_t>(world_data->game_state + offsets.player_array);
		world_data->player_array_size = efi.read<int>(world_data->game_state + offsets.player_array + sizeof(uintptr_t));

		for (int i = 0; i < world_data->player_array_size; ++i)
		{
			world_data->player_state = efi.read<uintptr_t>(world_data->player_array + (i * sizeof(uintptr_t)));
			if (!world_data->player_state) continue;

			auto team_id = efi.read<int>(world_data->player_state + offsets.teamindex);

			if (team_id == local_team_id)
				continue;

			world_data->pawn_private = efi.read<uintptr_t>(world_data->player_state + offsets.pawn_private);
			if (world_data->pawn_private && world_data->pawn_private != world_data->local_pawn)
			{
				world_data->mesh = efi.read<uint64_t>(world_data->pawn_private + offsets.mesh);
				if (!world_data->mesh) continue;

				entities stored;
				stored.actor = world_data->pawn_private;
				stored.player_state = world_data->player_state;
				stored.mesh = world_data->mesh;
				temp_stored.push_back(stored);
			}
		}

		std::swap(entity_list, temp_stored);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		temp_stored.clear();
	}
}