#pragma once

#include <impl/includes.hpp>

namespace global
{
	inline bool draw = { false };

	// for easy access to different chains
	inline SDK::UWorld * gworld = SDK::UWorld::GetWorld( );
	inline SDK::UGameInstance * game_instance = gworld->OwningGameInstance;
	inline SDK::AGameStateBase * game_state = gworld->GameState;
	inline SDK::UPlayer * localplayers = game_instance->LocalPlayers [ 0 ];
	inline SDK::APlayerController * my_controller = localplayers->PlayerController;
	inline SDK::APawn * my_pawn = my_controller->AcknowledgedPawn;
	inline SDK::APlayerCameraManager * cam_manager = my_controller->PlayerCameraManager;
	//inline SDK::APlayerState * parray = game_state->PlayerArray.Num( );

	inline bool movement_speed = { false };
	inline float speed_force = { 50.0f };
	inline bool godmode = { false };

	namespace visuals
	{
		inline bool enabled = { false };

		inline bool boxes = { false };
		inline bool health_bar = { false };

		inline bool chams = { false };
	}
}