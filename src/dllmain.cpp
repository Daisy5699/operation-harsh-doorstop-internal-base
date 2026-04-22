#include <impl/includes.hpp>

/*
   credits to @daisy as i made this
   credits to @xenon for the menu design
   this was originally going to be a internal for splitgate, but game was dead so i didn't do it and decided todo operation harsh doorstop instead
   this can also be easily converted to work with any other unreal engine game just change the sdk and some of the code in visuals and it should work fine
*/

DWORD WINAPI main_thread( LPVOID )
{
	MessageBoxA( nullptr , "press ok once in lobby" , nullptr , MB_OK );

	g_menu->hook( );

	return 0;
}

BOOL APIENTRY DllMain( HMODULE h_module , DWORD ul_reason_for_call , LPVOID lp_reserved )
{
	switch ( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( h_module );
		CreateThread( nullptr , 0 , ( LPTHREAD_START_ROUTINE ) main_thread, h_module , 0 , nullptr );
		break;

		case DLL_PROCESS_DETACH:
			kiero::shutdown( );
			break;
	}

	return TRUE;
}