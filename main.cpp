#include <main/cache/thread.h>
#include <main/actor/loop.h>

#include <overlay/overlay.h>
#include <sdk/sdk.h>

int main( )
{
	std::thread( cache::thread ).detach();
	std::thread( actor::weapon ).detach( );

	if ( uengine->cm.initialize( ) != true )
	{
		std::cout << "[</>] Failed To Initialize Camera (Press Any Key To Exit)" << std::endl;
		system( "pause >nul" );
		exit( 0 );
	}

	if ( !overlay::init( ) )
	{
		std::cout << "[</>] Failed To Initialize Window (Press Any Key To Exit)" << std::endl;
		system( "pause >nul" );
		exit( 0 );
	}

	if ( !overlay::init_dx11( ) )
	{
		std::cout << "[</>] Failed To Initialize Dx11 (Press Any Key To Exit)" << std::endl;
		system( "pause >nul" );
		exit( 0 );
	}

	ShowWindow( GetConsoleWindow( ), SW_HIDE );

	overlay::render_loop( );
}