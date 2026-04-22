#pragma once
#include <impl/includes.hpp>

#include "../drawing/drawing.hpp"

/*
   had chatgpt to the math for the boxes cuz i don't like doing that part !
   i put some non visual features (walkspeed and godmode) inside this file, sorry! was too lazy to move into a misc folder
*/

struct box_t
{
	float x , y , w , h;
};

inline bool get_screen_bone( SDK::USkeletalMeshComponent * mesh , SDK::APlayerController * controller , const wchar_t * name , ImVec2 & out )
{
    if ( !mesh || !controller )
        return false;

    SDK::FName bone = SDK::UKismetStringLibrary::Conv_StringToName( name );
    SDK::FVector world = mesh->GetSocketLocation( bone );

    SDK::FVector2D screen;
    if ( !controller->ProjectWorldLocationToScreen( world , &screen , true ) )
        return false;

    out = ImVec2( screen.X , screen.Y );
    return true;
}

inline void bone_line( SDK::USkeletalMeshComponent * mesh , SDK::APlayerController * controller , const wchar_t * a , const wchar_t * b , ImU32 col )
{
    ImVec2 p1 , p2;

    if ( get_screen_bone( mesh , controller , a , p1 ) &&
        get_screen_bone( mesh , controller , b , p2 ) )
    {
        ImGui::GetBackgroundDrawList( )->AddLine( p1 , p2 , col , 1.0f );
    }
}

inline bool build_box( SDK::ACharacter * actor , SDK::APlayerController * controller , box_t & box )
{
    if ( !actor || !actor->Mesh || !controller )
        return false;

    auto mesh = actor->Mesh;

    SDK::FName headName = SDK::UKismetStringLibrary::Conv_StringToName( L"head" );
    SDK::FName footLName = SDK::UKismetStringLibrary::Conv_StringToName( L"foot_l" );
    SDK::FName footRName = SDK::UKismetStringLibrary::Conv_StringToName( L"foot_r" );

    SDK::FVector headPos = mesh->GetSocketLocation( headName );
    SDK::FVector footLPos = mesh->GetSocketLocation( footLName );
    SDK::FVector footRPos = mesh->GetSocketLocation( footRName );

    SDK::FVector bottomPos;
    bottomPos.X = ( footLPos.X + footRPos.X ) / 2.f;
    bottomPos.Y = ( footLPos.Y + footRPos.Y ) / 2.f;
    bottomPos.Z = ( footLPos.Z + footRPos.Z ) / 2.f;

    headPos.Z += 15.f;
    bottomPos.Z -= 10.f;

    SDK::FVector2D screenHead , screenBottom;

    if ( !controller->ProjectWorldLocationToScreen( headPos , &screenHead , true ) )
        return false;

    if ( !controller->ProjectWorldLocationToScreen( bottomPos , &screenBottom , true ) )
        return false;

    float height = screenBottom.Y - screenHead.Y;
    float width = height * 0.55f;

    box.x = screenHead.X - ( width / 2.f );
    box.y = screenHead.Y;
    box.w = width;
    box.h = height;

    return true;
}

inline SDK::FName to_string(const wchar_t * str )
{
    return SDK::UKismetStringLibrary::Conv_StringToName( str );
}

SDK::UMaterial * WireframeMaterial = nullptr;
SDK::UMaterial * FlameMaterial = nullptr;
SDK::UMaterialInstanceDynamic * ChamsMat = nullptr;
SDK::UMaterialInstanceDynamic * ChamsMatVisible = nullptr;
SDK::UMaterialInstanceDynamic * ChamsMatNotVisible = nullptr;
SDK::UMaterialInstanceDynamic * WeaponChamsMat = nullptr;
SDK::UMaterialInstanceDynamic * EnemieWEaponMat = nullptr;

void init_materials( SDK::UWorld * World )
{
    WireframeMaterial = SDK::UObject::FindObject<SDK::UMaterial>( "Material WireframeMaterial.WireframeMaterial" );
    FlameMaterial = SDK::UObject::FindObject<SDK::UMaterial>( "Material M_FlameBig.M_FlameBig" );

    if ( WireframeMaterial )
    {
        WireframeMaterial->bDisableDepthTest = true;
        WireframeMaterial->Wireframe = true;
        WireframeMaterial->BlendMode = SDK::EBlendMode::BLEND_Additive;
        WireframeMaterial->MaterialDomain = SDK::EMaterialDomain::MD_Surface;
        WireframeMaterial->AllowTranslucentCustomDepthWrites = true;
        WireframeMaterial->bIsBlendable = true;
        WireframeMaterial->LightmassSettings.EmissiveBoost = 2;
        WireframeMaterial->LightmassSettings.DiffuseBoost = 0;
    }

    ChamsMat = SDK::UKismetMaterialLibrary::CreateDynamicMaterialInstance( World , WireframeMaterial , to_string( L"ChamsMaterial" ) );
    ChamsMatVisible = SDK::UKismetMaterialLibrary::CreateDynamicMaterialInstance( World , WireframeMaterial , to_string( L"ChamsVisibleMaterial" ) );
    ChamsMatNotVisible = SDK::UKismetMaterialLibrary::CreateDynamicMaterialInstance( World , WireframeMaterial , to_string( L"ChamsNotVisibleMaterial" )  );
    WeaponChamsMat = SDK::UKismetMaterialLibrary::CreateDynamicMaterialInstance( World , WireframeMaterial , to_string( L"WeaponMaterial" )  );
    EnemieWEaponMat = SDK::UKismetMaterialLibrary::CreateDynamicMaterialInstance( World , WireframeMaterial , to_string( L"EnemieWeaponMaterial" )  );
}

inline void apply_chams( SDK::USkeletalMeshComponent * mesh , SDK::UMaterialInstanceDynamic * material , SDK::FLinearColor color , bool bVisibilityCheck , bool isvisible )
{
    if ( !mesh || !material ) return;
    auto Mats = mesh->GetMaterials( );

    if ( bVisibilityCheck )
    {
        SDK::UMaterialInstanceDynamic * target = isvisible ? ChamsMatVisible : ChamsMatNotVisible;
        SDK::FLinearColor target_col = isvisible ? SDK::FLinearColor { 0.f, 1.f, 0.f, 1.f } : SDK::FLinearColor { 1.f, 0.f, 0.f, 1.f };

        target->SetVectorParameterValue( to_string( L"Color" ) , target_col );
        for ( int t = 0; t < Mats.Num( ); t++ )
        {
            if ( Mats [ t ] ) mesh->SetMaterial( t , target );
        }
    }
    else
    {
        material->SetVectorParameterValue( to_string( L"Color" ) , color );
        for ( int t = 0; t < Mats.Num( ); t++ )
        {
            if ( Mats [ t ] ) mesh->SetMaterial( t , material );
        }
    }
}

namespace visuals
{
    inline auto draw( ) -> void
    {
        for ( int i = 0; i < global::game_state->PlayerArray.Num( ); i++ )
        {
            auto player = global::game_state->PlayerArray [ i ];
            if ( !player || player->bIsSpectator )
                continue;

            auto character = static_cast< SDK::AHDPlayerCharacter * >( player->PawnPrivate );
            if ( !character )
                continue;

            if ( character == global::my_controller->Character )
                continue;

            box_t box;
            if ( !build_box( character , global::my_controller , box ) )
                continue;

            ImVec2 pos( box.x , box.y );
            ImVec2 size( box.w , box.h );

            float color [ 4 ] = { 1.f, 1.f, 1.f, 1.f };

            //init_materials( global::gworld );

            if ( global::visuals::boxes )
            {
                g_drawing->outlined_box( pos , size , IM_COL32( 255 , 255 , 255 , 255 ) , 1.0f , 0.f );
            }
            if ( global::visuals::health_bar )
            {
                g_drawing->healthbar( pos , size , character->Health , character->MaxHealth , color , 4.2f , 1.0f , IM_COL32( 0 , 0 , 0 , 255 ) );
            }

            if ( global::visuals::chams )
            {
                bool is_visible = global::my_controller->LineOfSightTo( character , SDK::FVector( ) , false );

                apply_chams( character->Mesh , ChamsMat , SDK::FLinearColor { 1.f, 1.f, 1.f, 1.f } , true , is_visible );
            }

            if ( true )
            {
				g_drawing->text_outline( ImGui::GetBackgroundDrawList( ) , pos , size , character->PlayerState->PlayerNamePrivate.ToString( ).c_str( ) , cheat::visuals::helper::sides_t::TOP , IM_COL32_WHITE , IM_COL32( 0 , 0 , 0 , 255 ) );
            }

			character->Mesh->SetRenderCustomDepth( true );
            character->Mesh->bForceWireframe = true;

            if ( global::movement_speed )
            {
                global::my_controller->Character->CharacterMovement->MaxWalkSpeed = global::speed_force;
                global::my_controller->Character->CharacterMovement->MaxAcceleration = global::speed_force;
            }

            if ( global::godmode )
            {
                character->Health = 9999.f;
				character->MaxHealth = 9999.f;
                global::my_controller->Character->bCanBeDamaged = false;
            }

            bone_line( character->Mesh , global::my_controller , L"head" , L"neck_01" , IM_COL32_WHITE );
            bone_line( character->Mesh , global::my_controller , L"neck_01" , L"spine_03" , IM_COL32_WHITE );
            bone_line( character->Mesh , global::my_controller , L"spine_03" , L"pelvis" , IM_COL32_WHITE );

            bone_line( character->Mesh , global::my_controller , L"spine_03" , L"upperarm_l" , IM_COL32_WHITE );
            bone_line( character->Mesh , global::my_controller , L"upperarm_l" , L"lowerarm_l" , IM_COL32_WHITE );
            bone_line( character->Mesh , global::my_controller , L"lowerarm_l" , L"hand_l" , IM_COL32_WHITE );

            bone_line( character->Mesh , global::my_controller , L"spine_03" , L"upperarm_r" , IM_COL32_WHITE );
            bone_line( character->Mesh , global::my_controller , L"upperarm_r" , L"lowerarm_r" , IM_COL32_WHITE );
            bone_line( character->Mesh , global::my_controller , L"lowerarm_r" , L"hand_r" , IM_COL32_WHITE );

            bone_line( character->Mesh , global::my_controller , L"pelvis" , L"thigh_l" , IM_COL32_WHITE );
            bone_line( character->Mesh , global::my_controller , L"thigh_l" , L"calf_l" , IM_COL32_WHITE );
            bone_line( character->Mesh , global::my_controller , L"calf_l" , L"foot_l" , IM_COL32_WHITE );

            bone_line( character->Mesh , global::my_controller , L"pelvis" , L"thigh_r" , IM_COL32_WHITE );
            bone_line( character->Mesh , global::my_controller , L"thigh_r" , L"calf_r" , IM_COL32_WHITE );
            bone_line( character->Mesh , global::my_controller , L"calf_r" , L"foot_r" , IM_COL32_WHITE );
        }
    }
}