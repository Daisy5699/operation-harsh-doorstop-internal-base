#define IMGUI_DEFINE_MATH_OPERATORS
#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <impl/includes.hpp>

namespace cheat::visuals::helper
{
    struct sides_t {
        enum Enum {
            LEFT = 0 ,
            TOP ,
            RIGHT ,
            BOTTOM
        } side;

        sides_t( Enum s ) : side( s ) { }

        ImVec2 get_position( const ImVec2 & box_pos , const ImVec2 & box_size , const ImVec2 & text_size , float gap = 3.0f , float offset = 0.0f ) const
        {
            switch ( side )
            {
            case LEFT:
                return ImVec2(
                    std::round( box_pos.x - gap - text_size.x ) ,
                    std::round( box_pos.y + ( box_size.y * 0.5f ) - ( text_size.y * 0.5f ) )
                );
            case RIGHT:
                return ImVec2(
                    std::round( box_pos.x + box_size.x + gap ) ,
                    std::round( box_pos.y + offset )
                );
            case TOP:
                return ImVec2(
                    std::round( box_pos.x + ( box_size.x * 0.5f ) - ( text_size.x * 0.5f ) ) ,
                    std::round( box_pos.y - text_size.y - gap )
                );
            case BOTTOM:
                return ImVec2(
                    std::round( box_pos.x + ( box_size.x * 0.5f ) - ( text_size.x * 0.5f ) ) ,
                    std::round( box_pos.y + box_size.y + gap + offset )
                );
            }
            return box_pos;
        }
    };

    inline std::vector<sides_t> element_sides = {
        sides_t( sides_t::LEFT ),
        sides_t( sides_t::TOP ),
        sides_t( sides_t::RIGHT ),
        sides_t( sides_t::BOTTOM ),
        sides_t( sides_t::LEFT )
    };

    namespace colors {
        inline ImU32 ConvertFloatImVec4( const float color [ 4 ] )
        {
            return ImGui::ColorConvertFloat4ToU32( ImVec4( color [ 0 ] , color [ 1 ] , color [ 2 ] , color [ 3 ] ) );
        }

        inline ImU32 ConvertFloatImVec4( const float color [ 3 ] , float alpha = 1.0f )
        {
            return ImGui::ColorConvertFloat4ToU32( ImVec4( color [ 0 ] , color [ 1 ] , color [ 2 ] , alpha ) );
        }
    }
}