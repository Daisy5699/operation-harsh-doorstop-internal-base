#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <impl/includes.hpp>
#include "../helper/helper.hpp"

#undef min
#undef max

struct drawing_t
{
    static void filled_box( const ImVec2 & pos , const ImVec2 & size , ImU32 col , float rounding )
    {
        ImVec2 rounded_pos( std::round( pos.x ) , std::round( pos.y ) );
        ImVec2 rounded_size( std::round( size.x ) , std::round( size.y ) );

        ImVec2 max(
            rounded_pos.x + rounded_size.x ,
            rounded_pos.y + rounded_size.y
        );

        auto draw = ImGui::GetBackgroundDrawList( );
        draw->AddRectFilled( rounded_pos , max ,
            IM_COL32( 0 , 0 , 0 , col >> 24 ) , rounding );
    }

    static void outlined_box( const ImVec2 & pos , const ImVec2 & size , ImU32 col , float thickness , float rounding )
    {
        ImVec2 rounded_pos( std::round( pos.x ) , std::round( pos.y ) );
        ImVec2 rounded_size( std::round( size.x ) , std::round( size.y ) );

        ImVec2 max(
            rounded_pos.x + rounded_size.x ,
            rounded_pos.y + rounded_size.y
        );

        auto draw = ImGui::GetBackgroundDrawList( );

        float max_rounding = std::min( rounded_size.x , rounded_size.y ) / 2.0f;
        rounding = std::min( rounding , max_rounding );

        ImU32 outline = IM_COL32( 0 , 0 , 0 , col >> 24 );

        draw->AddRect( rounded_pos , max , outline , rounding , 0 , thickness );

        draw->AddRect(
            ImVec2( rounded_pos.x - 2.f , rounded_pos.y - 2.f ) ,
            ImVec2( max.x + 2.f , max.y + 2.f ) ,
            outline , rounding , 0 , thickness );

        draw->AddRect(
            ImVec2( rounded_pos.x - 1.f , rounded_pos.y - 1.f ) ,
            ImVec2( max.x + 1.f , max.y + 1.f ) ,
            col , rounding , 0 , thickness );
    }

    static void corner( const ImVec2 & pos , const ImVec2 & size , ImU32 col , float thickness , float rounding )
    {
        auto draw = ImGui::GetBackgroundDrawList( );

        float X = pos.x;
        float Y = pos.y;
        float W = size.x;
        float H = size.y;

        float lineW = W / 4.f;
        float lineH = H / 4.f;
        float lineT = 1.f;
        float topOffset = 1.f;

        ImU32 outline = IM_COL32( 0 , 0 , 0 , col >> 24 );

        draw->AddLine(
            ImVec2( roundf( X ) , roundf( Y - topOffset ) ) ,
            ImVec2( roundf( X ) , roundf( Y + lineH ) ) , col );

        draw->AddLine(
            ImVec2( roundf( X ) , roundf( Y - topOffset ) ) ,
            ImVec2( roundf( X + lineW ) , roundf( Y - topOffset ) ) , col );

        draw->AddLine(
            ImVec2( roundf( X + W - lineW ) , roundf( Y - topOffset ) ) ,
            ImVec2( roundf( X + W ) , roundf( Y - topOffset ) ) , col );

        draw->AddLine(
            ImVec2( roundf( X + W ) , roundf( Y - topOffset ) ) ,
            ImVec2( roundf( X + W ) , roundf( Y + lineH ) ) , col );

        draw->AddLine(
            ImVec2( roundf( X ) , roundf( Y + H - lineH ) ) ,
            ImVec2( roundf( X ) , roundf( Y + H ) ) , col );

        draw->AddLine(
            ImVec2( roundf( X ) , roundf( Y + H ) ) ,
            ImVec2( roundf( X + lineW ) , roundf( Y + H ) ) , col );

        draw->AddLine(
            ImVec2( roundf( X + W - lineW ) , roundf( Y + H ) ) ,
            ImVec2( roundf( X + W ) , roundf( Y + H ) ) , col );

        draw->AddLine(
            ImVec2( roundf( X + W ) , roundf( Y + H - lineH ) ) ,
            ImVec2( roundf( X + W ) , roundf( Y + H ) ) , col );
    }

    static void healthbar( const ImVec2 & box_pos , const ImVec2 & box_size , float health , float max_health , const float color [ 4 ] , float gap = 4.0f , float thickness = 3.0f , ImU32 outline_col = IM_COL32( 0 , 0 , 0 , 255 ) )
    {
        using namespace cheat::visuals::helper;

        auto draw = ImGui::GetBackgroundDrawList( );
        draw->Flags &= ~ImDrawListFlags_AntiAliasedLines;

        float ratio = ( max_health > 0.f ) ? health / max_health : 0.f;
        ratio = std::fmax( 0.f , std::fmin( ratio , 1.f ) );

        sides_t left_side( sides_t::LEFT );
        ImVec2 bar_base_pos = left_side.get_position( box_pos , box_size , ImVec2( thickness , box_size.y ) , gap );

        float y_min = std::round( box_pos.y );
        float y_max = std::round( box_pos.y + box_size.y );
        float x_hp = std::round( bar_base_pos.x );

        ImVec2 bg_min( x_hp - 1.f , y_min - 2.f );
        ImVec2 bg_max( x_hp + thickness + 1.f , y_max + 2.f );
        draw->AddRectFilled( bg_min , bg_max , outline_col );

        ImVec2 empty_min( x_hp , y_min - 1.f );
        ImVec2 empty_max( x_hp + thickness , y_max + 1.f );
        draw->AddRectFilled( empty_min , empty_max , IM_COL32( 130 , 130 , 130 , 150 ) );

        float height = ( y_max - y_min ) * ratio;
        ImVec2 fg_min( x_hp , y_max - height - 1.f );
        ImVec2 fg_max( x_hp + thickness , y_max + 1.f );

        ImVec4 final_color;
        if ( true )
        {
            if ( ratio < 0.2f )
                final_color = ImVec4( 235.f / 255.f , 52.f / 255.f , 52.f / 255.f , color [ 3 ] );
            else if ( ratio < 0.4f )
                final_color = ImVec4( 235.f / 255.f , 168.f / 255.f , 52.f / 255.f , color [ 3 ] );
            else if ( ratio < 0.7f )
                final_color = ImVec4( 192.f / 255.f , 235.f / 255.f , 52.f / 255.f , color [ 3 ] );
            else
                final_color = ImVec4( 94.f / 255.f , 235.f / 255.f , 52.f / 255.f , color [ 3 ] );
        }
        else
        {
            final_color = ImVec4( color [ 0 ] , color [ 1 ] , color [ 2 ] , color [ 3 ] );
        }

        draw->AddRectFilled( fg_min , fg_max , ImGui::ColorConvertFloat4ToU32( final_color ) );
    }

    static void armorbar( const ImVec2 & box_pos , const ImVec2 & box_size , float health , float max_health , const float color [ 4 ] , float gap = 4.0f , float thickness = 3.0f , ImU32 outline_col = IM_COL32( 0 , 0 , 0 , 255 ) )
    {
        using namespace cheat::visuals::helper;

        auto draw = ImGui::GetBackgroundDrawList( );
        draw->Flags &= ~ImDrawListFlags_AntiAliasedLines;

        float ratio = ( max_health > 0.f ) ? health / max_health : 0.f;
        ratio = std::fmax( 0.f , std::fmin( ratio , 1.f ) );

        sides_t left_side( sides_t::LEFT );
        ImVec2 bar_base_pos = left_side.get_position( box_pos , box_size , ImVec2( thickness , box_size.y ) , gap );

        float y_min = std::round( box_pos.y );
        float y_max = std::round( box_pos.y + box_size.y );
        float x_hp = std::round( bar_base_pos.x );

        ImVec2 bg_min( x_hp - 1.f , y_min - 2.f );
        ImVec2 bg_max( x_hp + thickness + 1.f , y_max + 2.f );
        draw->AddRectFilled( bg_min , bg_max , outline_col );

        ImVec2 empty_min( x_hp , y_min - 1.f );
        ImVec2 empty_max( x_hp + thickness , y_max + 1.f );
        draw->AddRectFilled( empty_min , empty_max , IM_COL32( 130 , 130 , 130 , 150 ) );

        float height = ( y_max - y_min ) * ratio;
        ImVec2 fg_min( x_hp , y_max - height - 1.f );
        ImVec2 fg_max( x_hp + thickness , y_max + 1.f );

        ImVec4 final_color;
        if ( true )
        {
            if ( ratio < 0.2f )
                final_color = ImVec4( 235.f / 255.f , 52.f / 255.f , 52.f / 255.f , color [ 3 ] );
            else if ( ratio < 0.4f )
                final_color = ImVec4( 235.f / 255.f , 168.f / 255.f , 52.f / 255.f , color [ 3 ] );
            else if ( ratio < 0.7f )
                final_color = ImVec4( 192.f / 255.f , 235.f / 255.f , 52.f / 255.f , color [ 3 ] );
            else
                final_color = ImVec4( 94.f / 255.f , 235.f / 255.f , 52.f / 255.f , color [ 3 ] );
        }
        else
        {
            final_color = ImVec4( color [ 0 ] , color [ 1 ] , color [ 2 ] , color [ 3 ] );
        }

        draw->AddRectFilled( fg_min , fg_max , ImGui::ColorConvertFloat4ToU32( final_color ) );
    }

    inline void text_outline( ImDrawList * draw ,
        const ImVec2 & box_pos ,
        const ImVec2 & box_size ,
        const std::string & text , const cheat::visuals::helper::sides_t & side , ImU32 color , ImU32 outline_color , float gap = 3.0f , float offset = 0.0f )
    {
        ImVec2 text_size = ImGui::CalcTextSize( text.c_str( ) );
        ImVec2 pos = side.get_position( box_pos , box_size , text_size , gap , offset );

        for ( int x = -1; x <= 1; x++ )
        {
            for ( int y = -1; y <= 1; y++ )
            {
                if ( x || y )
                    draw->AddText( pos + ImVec2( ( float ) x , ( float ) y ) , outline_color , text.c_str( ) );
            }
        }

        draw->AddText( pos , color , text.c_str( ) );
    }
};

inline auto g_drawing = std::make_shared< drawing_t >( );