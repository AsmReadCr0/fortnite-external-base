#pragma once
#include <utils/includes.h>
#include <imgui/drawing.h>

static class actor
{
public:
    static void loop();
    static void weapon();
};

struct projectile_properties
{
    int speed;
    double gravity;
};

inline std::unordered_map<std::string, projectile_properties> weapon_properties = {
    {"Striker AR", {80000, 3.5}},
    {"Nisha's Striker AR", {80000, 3.5}}, //mythic
    {"Nemesis AR", {80000, 3.5}},
    {"Montague's Nemesis AR", {80000, 3.5}}, //mythic
    {"Reaper Sniper Rifle", {60000, 3.5}},
    {"Enforcer AR", {65000, 2.5}},
    {"Ranger Pistol", {60000, 2}},
    {"Thunder Burst SMG", {70000, 3}},
    {"Hyper SMG", {70000, 3}},
    {"Valeria's Hyper SMG", {70000, 3}} //mythic
};

inline std::vector<std::pair<int, int>> bone_pairs = {
    {109, 66},
    {66, 9},
    {66, 38},
    {9, 10},
    {38, 39},
    {10, 11},
    {39, 40},
    {66, 2},
    {2, 78},
    {2, 71},
    {78, 79},
    {79, 82},
    {71, 72},
    {72, 75}
};

inline Vector3 velocity;
inline uintptr_t current_root;
inline std::string local_weapon_name;
inline ImColor visible_color;