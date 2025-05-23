#include <utils/includes.h>

class sdk
{
public:
    class bone_manager
    {
    public:
        void get_bone_array(uintptr_t mesh);
        Vector3 get_bone(int id);
        std::unordered_map<int, Vector3> get_skeleton_bones(const std::vector<std::pair<int, int>>& bone_pairs);

        struct
        {
            uintptr_t bone_array;
            FTransform component_to_world;
        }data;

        struct
        {
            struct
            {
                Vector3 pos;
                Vector3 box;
                Vector3 screen_pos;
            }head;

            struct
            {
                Vector3 pos;
                Vector3 screen_pos;
            }root;
        }bone;
    }bm;


    class camera_manager
    {
    public:
        void get_camera();
        bool initialize( );
        Vector3 w2s(Vector3 pos);

        uintptr_t view_state;

        struct camera_data
        {
            Vector3 location;
            FRotator rotation;
            float fov;
        }camera;
    }cm;

    class other_manager
    {
    public:
        float projectile_speed = 0;
        float projectile_gravity = 0;
        Vector3 predict(Vector3 target, Vector3 velocity, float projectile_speed, float gravity_scale, float distance);
        std::string get_weapon_name(uintptr_t actor);
        std::string decrypt_name(uintptr_t state);
        bool is_visible(uintptr_t mesh);
    }om;
};

inline std::unique_ptr<sdk> uengine = std::make_unique<sdk>();

inline sdk::camera_manager camera_manager;
inline sdk::bone_manager bone_manager;
inline sdk::other_manager other_manager;