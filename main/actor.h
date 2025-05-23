#pragma once
#include <utils/includes.h>

struct entities
{
    uint64_t actor;
    uint64_t mesh;
    uint64_t player_state;
};
inline std::vector<entities> entity_list;

struct world_data_class
{
    uintptr_t uworld;
    uintptr_t game_instance;
    uintptr_t game_state;
    uintptr_t local_players;
    uintptr_t local_player;
    uintptr_t local_pawn;
    uintptr_t mesh;
    uintptr_t player_controller;
    uintptr_t local_player_state;
    uintptr_t player_state;
    uintptr_t player_array;
    uintptr_t pawn_private;

    int player_array_size;
};
inline const std::unique_ptr<world_data_class> world_data = std::make_unique<world_data_class>();
