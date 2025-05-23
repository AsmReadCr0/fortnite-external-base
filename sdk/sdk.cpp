#include <sdk/sdk.h>
#include <main/actor.h>

void sdk::bone_manager::get_bone_array( uintptr_t mesh )
{
	uengine->bm.data.bone_array = efi.read<uintptr_t>( mesh + 0x5A8 );
	if ( !uengine->bm.data.bone_array )
		uengine->bm.data.bone_array = efi.read<uintptr_t>( mesh + 0x5A8 + 0x10 );

	uengine->bm.data.component_to_world = efi.read<FTransform>( mesh + offsets.componet_to_world );
}

Vector3 sdk::bone_manager::get_bone( int id )
{
	auto bone = efi.read<FTransform>( uengine->bm.data.bone_array + id * 0x60 );
	D3DMATRIX bone_matrix = MatrixMultiplication( bone.ToMatrixWithScale( ), uengine->bm.data.component_to_world.ToMatrixWithScale( ) );
	return Vector3( bone_matrix._41, bone_matrix._42, bone_matrix._43 );
}

std::unordered_map<int, Vector3> sdk::bone_manager::get_skeleton_bones( const std::vector<std::pair<int, int>>& bone_pairs )
{
	std::unordered_map<int, Vector3> bone_positions;
	bone_positions.reserve( bone_pairs.size( ) * 2 );

	D3DMATRIX componentToWorldMatrix = uengine->bm.data.component_to_world.ToMatrixWithScale( );

	for ( const auto& pair : bone_pairs )
	{
		for ( int id : {pair.first, pair.second} )
		{
			if ( bone_positions.find( id ) == bone_positions.end( ) )
			{
				FTransform bone = efi.read<FTransform>( uengine->bm.data.bone_array + ( id * 0x60 ) );
				D3DMATRIX boneMatrix = bone.ToMatrixWithScale( );
				D3DMATRIX matrix = MatrixMultiplication( boneMatrix, componentToWorldMatrix );
				bone_positions[ id ] = Vector3( matrix._41, matrix._42, matrix._43 );
			}
		}
	}

	return bone_positions;
}

void sdk::camera_manager::get_camera( )
{
	auto projection = efi.read<FMatrix>( this->view_state + 0x940 );

	this->camera.rotation.Pitch = asin( projection.z_plane.w ) * 180.0f / pi_;
	this->camera.rotation.Yaw = atan2( projection.y_plane.w, projection.x_plane.w ) * 180.0f / pi_;
	this->camera.rotation.Roll = 0.0f;

	this->camera.location.x = projection.m[ 3 ][ 0 ];
	this->camera.location.y = projection.m[ 3 ][ 1 ];
	this->camera.location.z = projection.m[ 3 ][ 2 ];

	this->camera.fov = efi.read<float>( world_data->player_controller + 0x3ac ) * 90.0f;
}

bool sdk::camera_manager::initialize( )
{
	auto gworld = efi.read<__int64>( offsets.uworld + /* image base goes here */ );
	auto game_instance = efi.read<uintptr_t>( gworld + offsets.game_instance );
	auto local_player = efi.read<uintptr_t>( efi.read<uintptr_t>( game_instance + offsets.local_player ) );
	auto view_matrix = efi.read<uintptr_t>( local_player + 0xd0 );

	this->view_state = efi.read<uintptr_t>( view_matrix + 0x8 );
}

inline float ratio = ( float )monitor.width / monitor.height;

Vector3 sdk::camera_manager::w2s( Vector3 pos )
{
	D3DMATRIX matrix = Matrix( camera.rotation, Vector3( 0, 0, 0 ) );

	Vector3 vAxisX = Vector3( matrix.m[ 0 ][ 0 ], matrix.m[ 0 ][ 1 ], matrix.m[ 0 ][ 2 ] );
	Vector3 vAxisY = Vector3( matrix.m[ 1 ][ 0 ], matrix.m[ 1 ][ 1 ], matrix.m[ 1 ][ 2 ] );
	Vector3 vAxisZ = Vector3( matrix.m[ 2 ][ 0 ], matrix.m[ 2 ][ 1 ], matrix.m[ 2 ][ 2 ] );

	Vector3 vDelta = pos - camera.location;

	Vector3 vTransformed = Vector3( vDelta.Dot( vAxisY ), vDelta.Dot( vAxisZ ), vDelta.Dot( vAxisX ) );

	if ( vTransformed.z < 1.f ) vTransformed.z = 1.f;

	if ( ratio < 4.0f / 3.0f ) // strech res compatibility
		ratio = 4.0f / 3.0f;

	float fov = ratio / ( 16.0f / 9.0f ) * ( float )tanf( camera.fov * pi_ / 360.0f );

	return Vector3( ( monitor.width / 2.0f ) + vTransformed.x * ( ( ( monitor.width / 2.0f ) / fov ) ) / vTransformed.z, ( monitor.height / 2.0f ) - vTransformed.y * ( ( ( monitor.width / 2.0f ) / fov ) ) / vTransformed.z, 0 );
}

Vector3 sdk::other_manager::predict( Vector3 target, Vector3 velocity, float projectile_speed, float gravity_scale, float distance )
{
	float time = distance / projectile_speed;

	target.addScaled( velocity, time );

	float gravity = std::fabs( -980.0f * gravity_scale ) * 0.5f * time * time;
	target.z += gravity;

	return target;
}

std::string sdk::other_manager::get_weapon_name( uintptr_t actor )
{
	uintptr_t current_weapon = efi.read<uintptr_t>( actor + 0xa80 );

	auto definition = efi.read<uintptr_t>( current_weapon + 0x690 );
	if ( !definition ) return { "" };

	auto fname_text = efi.read<std::uint64_t>( definition + 0x40 );
	if ( !fname_text ) return { "" };

	auto name_length = efi.read<int32_t>( fname_text + 0x20 );

	std::unique_ptr<wchar_t[]> weapon_name( new wchar_t[ name_length + 1 ] );

	if ( !efi.read_memory( reinterpret_cast< ULONG64 >( efi.read<PVOID>( fname_text + 0x18 ) ), weapon_name.get( ), name_length * sizeof( wchar_t ) ) )  return { "" };

	std::wstring w_buffer{ weapon_name.get( ) };

	return { w_buffer.begin( ), w_buffer.end( ) };
}

bool sdk::other_manager::is_visible( uintptr_t mesh )
{
	auto server_seconds = efi.read<double>( world_data->uworld + 0x148 );
	auto last_render_time = efi.read<float>( mesh + 0x30C );
	return server_seconds - last_render_time <= 0.06f;
}

std::string sdk::other_manager::decrypt_name( uintptr_t PlayerState )
{
	auto f_string = efi.read<__int64>( PlayerState + 0xAF8 );
	if ( !f_string ) return std::string( "" );

	auto length = efi.read<int>( f_string + 0x10 );
	if ( !length || length < 0 || length > 255 ) return std::string( "" );

	auto f_text = efi.read<__int64>( f_string + 0x8 );
	if ( !f_text ) return std::string( "" );

	wchar_t* buffer = new wchar_t[ length ];
	efi.read_memory( f_text, buffer, length * sizeof( wchar_t ) );

	auto v6 = ( __int64 )length;

	char v21;
	int v22;
	int i;

	int v25;
	unsigned short* v23;

	v21 = v6 - 1;
	if ( !( unsigned int )v6 )
		v21 = 0;
	v22 = 0;
	v23 = ( unsigned short* )buffer;
	for ( i = ( v21 ) & 3;; *v23++ += i & 7 )
	{
		v25 = v6 - 1;
		if ( !( unsigned int )v6 )
			v25 = 0;
		if ( v22 >= v25 )
			break;
		i += 3;
		++v22;
	}

	std::wstring w_buffer{ buffer };

	delete[] buffer;

	return std::string( w_buffer.begin( ), w_buffer.end( ) );
}