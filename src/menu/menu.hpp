#pragma once
#include <cstdint>
#include <impl/includes.hpp>
#include "imgui_helper.hpp"
#include "fonts/fonts.hpp"
#include <src/cheat/visuals/visuals.hpp>

typedef HRESULT( __stdcall * Present )( IDXGISwapChain * pSwapChain , UINT SyncInterval , UINT Flags );
typedef LRESULT( CALLBACK * WNDPROC )( HWND , UINT , WPARAM , LPARAM );
typedef uintptr_t PTR;

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam );

class c_menu
{
public:
    Present o_present = nullptr;
    HWND window = NULL;

    WNDPROC o_wndproc = NULL;

    ID3D11Device * p_device = NULL;
    ID3D11DeviceContext * p_context = NULL;
    ID3D11RenderTargetView * main_render_target_view = NULL;

    bool init = false;

private:
    static inline c_menu * instance = nullptr;

public:

	enum Tabs {
		AIM = 0 ,
		VISUALS ,
		RADAR ,
		MISC ,
		DEV ,
		NUM_TABS
	};

	enum SubTabs {
		SETTINGS = 0 ,
		COLORS ,
	};

	inline static ImFont * m_pMainFont = nullptr;
	inline static ImFont * m_pBiggerMainFont = nullptr;
	inline static int m_nSelectedTab = 0;
	inline static int m_nSelectedSubTab = 0;
	inline static bool m_bDevTab = false;

    auto get( ) -> c_menu *
    {
        return instance;
	}

    c_menu( )
    {
        instance = this;
    }

    void init_imgui( )
    {
        ImGui::CreateContext( );

        ImGuiIO & io = ImGui::GetIO( );
        io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

		m_pMainFont = io.Fonts->AddFontFromMemoryTTF(
			&mainFontBytes , sizeof( mainFontBytes ) , 14 ,
			NULL , io.Fonts->GetGlyphRangesCyrillic( )
		);
		m_pBiggerMainFont = io.Fonts->AddFontFromMemoryTTF(
			&mainFontBytes , sizeof( mainFontBytes ) , 23 ,
			NULL , io.Fonts->GetGlyphRangesCyrillic( )
		);
		io.FontDefault = m_pMainFont;

        ImGui_ImplWin32_Init( window );
        ImGui_ImplDX11_Init( p_device , p_context );
    }

    void default_theme( ) /* the mentalist reference ?!?!?*/
    {
        ImVec4 * colors = ImGui::GetStyle( ).Colors;
        colors [ ImGuiCol_Text ] = ImVec4( 1.00f , 1.00f , 1.00f , 1.00f );
        colors [ ImGuiCol_TextDisabled ] = ImVec4( 0.50f , 0.50f , 0.50f , 1.00f );
        colors [ ImGuiCol_WindowBg ] = ImVec4( 0.10f , 0.10f , 0.10f , 0.90f );
        colors [ ImGuiCol_ChildBg ] = ImVec4( 0.00f , 0.00f , 0.00f , 0.00f );
        colors [ ImGuiCol_PopupBg ] = ImVec4( 0.19f , 0.19f , 0.19f , 0.92f );
        colors [ ImGuiCol_FrameBg ] = ImVec4( 0.05f , 0.05f , 0.05f , 0.54f );
        colors [ ImGuiCol_FrameBgHovered ] = ImVec4( 0.19f , 0.19f , 0.19f , 0.54f );
        colors [ ImGuiCol_FrameBgActive ] = ImVec4( 0.20f , 0.22f , 0.23f , 0.70f );
        colors [ ImGuiCol_TitleBg ] = ImVec4( 0.00f , 0.00f , 0.00f , 0.60f );
        colors [ ImGuiCol_TitleBgActive ] = ImVec4( 0.06f , 0.06f , 0.06f , 0.60f );
        colors [ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.00f , 0.00f , 0.00f , 0.30f );
        colors [ ImGuiCol_MenuBarBg ] = ImVec4( 0.14f , 0.14f , 0.14f , 0.60f );
        colors [ ImGuiCol_ScrollbarBg ] = ImVec4( 0.05f , 0.05f , 0.05f , 0.54f );
        colors [ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.34f , 0.34f , 0.34f , 0.54f );
        colors [ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.40f , 0.40f , 0.40f , 0.54f );
        colors [ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.56f , 0.56f , 0.56f , 0.54f );
        colors [ ImGuiCol_CheckMark ] = ImVec4( 0.33f , 0.67f , 0.86f , 1.00f );
        colors [ ImGuiCol_SliderGrab ] = ImVec4( 0.34f , 0.34f , 0.34f , 0.54f );
        colors [ ImGuiCol_SliderGrabActive ] = ImVec4( 0.56f , 0.56f , 0.56f , 0.54f );
        colors [ ImGuiCol_Button ] = ImVec4( 0.05f , 0.05f , 0.05f , 0.54f );
        colors [ ImGuiCol_ButtonHovered ] = ImVec4( 0.19f , 0.19f , 0.19f , 0.54f );
        colors [ ImGuiCol_ButtonActive ] = ImVec4( 0.20f , 0.22f , 0.23f , 1.00f );
        colors [ ImGuiCol_Header ] = ImVec4( 0.00f , 0.00f , 0.00f , 0.52f );
        colors [ ImGuiCol_HeaderHovered ] = ImVec4( 0.00f , 0.00f , 0.00f , 0.36f );
        colors [ ImGuiCol_HeaderActive ] = ImVec4( 0.20f , 0.22f , 0.23f , 0.33f );
        colors [ ImGuiCol_Separator ] = ImVec4( 0.28f , 0.28f , 0.28f , 0.29f );
        colors [ ImGuiCol_SeparatorHovered ] = ImVec4( 0.44f , 0.44f , 0.44f , 0.29f );
        colors [ ImGuiCol_SeparatorActive ] = ImVec4( 0.40f , 0.44f , 0.47f , 1.00f );
        colors [ ImGuiCol_ResizeGrip ] = ImVec4( 0.28f , 0.28f , 0.28f , 0.29f );
        colors [ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.44f , 0.44f , 0.44f , 0.29f );
        colors [ ImGuiCol_ResizeGripActive ] = ImVec4( 0.40f , 0.44f , 0.47f , 1.00f );
        colors [ ImGuiCol_PlotLines ] = ImVec4( 1.00f , 0.00f , 0.00f , 1.00f );
        colors [ ImGuiCol_PlotLinesHovered ] = ImVec4( 1.00f , 0.00f , 0.00f , 1.00f );
        colors [ ImGuiCol_PlotHistogram ] = ImVec4( 1.00f , 0.00f , 0.00f , 1.00f );
        colors [ ImGuiCol_PlotHistogramHovered ] = ImVec4( 1.00f , 0.00f , 0.00f , 1.00f );
        colors [ ImGuiCol_TextSelectedBg ] = ImVec4( 0.20f , 0.22f , 0.23f , 1.00f );
        colors [ ImGuiCol_DragDropTarget ] = ImVec4( 0.33f , 0.67f , 0.86f , 1.00f );

		bool rainbowBorders = false;
        if ( rainbowBorders )
        {
            //colors[ImGuiCol_Border] = ImVec4(Config::m_v4Rainbow.x, Config::m_v4Rainbow.y, Config::m_v4Rainbow.z, 0.5f);
        }
        else
        {
            colors [ ImGuiCol_Border ] = ImVec4( 0.19f , 0.19f , 0.19f , 0.29f );
        }

        colors [ ImGuiCol_BorderShadow ] = ImVec4( 0.00f , 0.00f , 0.00f , 0.4f );

        ImGuiStyle & style = ImGui::GetStyle( );
        style.WindowPadding = ImVec2( 15 , 5 );
        style.IndentSpacing = 25;
        style.GrabMinSize = 10;
        style.ChildBorderSize = 1;
        style.PopupBorderSize = 1;
        style.WindowRounding = 6;
        style.ChildRounding = 4;
        style.FrameRounding = 3;
        style.PopupRounding = 4;
        style.ScrollbarRounding = 9;
        style.ScrollbarSize = 10;
        style.GrabRounding = 3;
        style.WindowBorderSize = 3;
        style.WindowTitleAlign = ImVec2( 0.5 , 0.5 );
    }

    static LRESULT __stdcall wndproc( const HWND h_wnd , UINT u_msg , WPARAM w_param , LPARAM l_param )
    {
        if ( ImGui_ImplWin32_WndProcHandler( h_wnd , u_msg , w_param , l_param ) )
            return true;

        return CallWindowProc( instance ? instance->o_wndproc : nullptr ,
            h_wnd , u_msg , w_param , l_param );
    }

    HRESULT __stdcall hk_present( IDXGISwapChain * p_swap_chain , UINT sync_interval , UINT flags )
    {
        if ( !init )
        {
            if ( SUCCEEDED( p_swap_chain->GetDevice( __uuidof( ID3D11Device ) , ( void ** ) &p_device ) ) )
            {
                p_device->GetImmediateContext( &p_context );

                DXGI_SWAP_CHAIN_DESC sd;
                p_swap_chain->GetDesc( &sd );
                window = sd.OutputWindow;

                ID3D11Texture2D * p_back_buffer = nullptr;
                p_swap_chain->GetBuffer( 0 , __uuidof( ID3D11Texture2D ) , ( LPVOID * ) &p_back_buffer );

                p_device->CreateRenderTargetView( p_back_buffer , NULL , &main_render_target_view );
                p_back_buffer->Release( );

                o_wndproc = ( WNDPROC ) SetWindowLongPtr(
                    window ,
                    GWLP_WNDPROC ,
                    ( LONG_PTR ) wndproc
                );

                init_imgui( );
                init = true;
            }
            else
            {
                return o_present( p_swap_chain , sync_interval , flags );
            }
        }

		if ( GetAsyncKeyState( VK_INSERT ) & 1 )
			global::draw = !global::draw;

        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        if ( global::draw )
        {

			instance->default_theme( );
			ImColor m_cMenuBg = ImColor( 19 , 54 , 52 , 255 );
			ImColor m_cMenuAccent = ImColor( 24 , 173 , 122 , 255 );
			ImColor m_cCrosshair = ImColor( 255 , 255 , 255 , 255 );
			ImColor m_cMenuBgFor = ImColor( 20 , 77 , 73 , 255 );
			ImColor m_cMenuBgFor2 = ImColor( 23 , 97 , 92 , 255 );
			ImColor m_cMenuOff = ImColor( 16 , 115 , 81 , 255 );

			if ( ImGui::Begin( "Default Menu" , nullptr , ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground ) )
			{

				//sizing
				float windowWidth = 500;
				ImGui::SetWindowSize( ImVec2( windowWidth , 440 ) );

				//helpers def
				ImVec2 pos = ImGui::GetWindowPos( ) + ImVec2( 5 , 5 );
				std::string app_title = "ue4-internal | by daisy";
				//main rect
				ImGui::GetWindowDrawList( )->AddRectFilled( pos , ImVec2( pos.x + windowWidth - 10 , pos.y + 430 ) , m_cMenuBg , 6.f , ImDrawFlags_RoundCornersAll );

				//title
				ImGui::GetWindowDrawList( )->AddRectFilled( pos , ImVec2( pos.x + windowWidth - 10 , pos.y + 25 ) , m_cMenuBg , 6.f , ImDrawFlags_RoundCornersTop );
				ImGui::GetWindowDrawList( )->AddText( ImGuiHelper::CenterText( pos , ImVec2( pos.x + windowWidth - 10 , pos.y + 25 ) , app_title.c_str() ) , m_cMenuAccent , app_title.c_str() );

				//tabs
				ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing , ImVec2( 0 , 0 ) );
				ImGui::PushStyleColor( ImGuiCol_Button , ImVec4( 0.2f , 0.2f , 0.2f , 1.f ) );
				ImGui::PushStyleColor( ImGuiCol_ButtonHovered , ImVec4( 0.3f , 0.3f , 0.3f , 1.f ) );
				ImGui::PushStyleColor( ImGuiCol_ButtonActive , ImVec4( 0.4f , 0.4f , 0.4f , 1.f ) );

				ImGui::SetCursorPos( ImVec2( 5 , 35 ) );

				float width = m_bDevTab ? windowWidth / 5 : windowWidth / 4;

				float tabHeight = 20;
				ImGui::BeginGroup( );
				{
					if ( ImGui::Tabs( "Aim" , ImVec2( width , tabHeight ) , m_nSelectedTab == Tabs::AIM , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedTab = Tabs::AIM;
					if ( ImGui::Tabs( "Visuals" , ImVec2( width , tabHeight ) , m_nSelectedTab == Tabs::VISUALS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedTab = Tabs::VISUALS;
					if ( ImGui::Tabs( "Radar" , ImVec2( width , tabHeight ) , m_nSelectedTab == Tabs::RADAR , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedTab = Tabs::RADAR;
					if ( ImGui::Tabs( "Misc" , ImVec2( width , tabHeight ) , m_nSelectedTab == Tabs::MISC , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedTab = Tabs::MISC;

					if ( m_bDevTab )
					{
						if ( ImGui::Tabs( "Dev" , ImVec2( width , tabHeight ) , m_nSelectedTab == Tabs::DEV , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedTab = Tabs::DEV;
					}
				}
				ImGui::EndGroup( );

				// handle subtabs
				switch ( m_nSelectedTab )
				{
				case Tabs::AIM:
					ImGui::SetCursorPos( ImVec2( 5 , 55 ) );
					ImGui::BeginGroup( );
					{
						if ( ImGui::Tabs( "Settings" , ImVec2( windowWidth / 2 , tabHeight ) , m_nSelectedSubTab == SubTabs::SETTINGS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedSubTab = SubTabs::SETTINGS;
						if ( ImGui::Tabs( "Colors" , ImVec2( windowWidth / 2 , tabHeight ) , m_nSelectedSubTab == SubTabs::COLORS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedSubTab = SubTabs::COLORS;
					}
					ImGui::EndGroup( );
					break;
				case Tabs::VISUALS:
					ImGui::SetCursorPos( ImVec2( 5 , 55 ) );
					ImGui::BeginGroup( );
					{
						if ( ImGui::Tabs( "Settings" , ImVec2( windowWidth / 2 , tabHeight ) , m_nSelectedSubTab == SubTabs::SETTINGS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedSubTab = SubTabs::SETTINGS;
						if ( ImGui::Tabs( "Colors" , ImVec2( windowWidth / 2 , tabHeight ) , m_nSelectedSubTab == SubTabs::COLORS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedSubTab = SubTabs::COLORS;
					}
					ImGui::EndGroup( );
					break;
				case Tabs::RADAR:
					ImGui::SetCursorPos( ImVec2( 5 , 55 ) );
					ImGui::BeginGroup( );
					{
						if ( ImGui::Tabs( "Settings" , ImVec2( windowWidth / 2 , tabHeight ) , m_nSelectedSubTab == SubTabs::SETTINGS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedSubTab = SubTabs::SETTINGS;
						if ( ImGui::Tabs( "Colors" , ImVec2( windowWidth / 2 , tabHeight ) , m_nSelectedSubTab == SubTabs::COLORS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedSubTab = SubTabs::COLORS;
					}
					ImGui::EndGroup( );
					break;
				case Tabs::MISC:
					ImGui::SetCursorPos( ImVec2( 5 , 55 ) );
					ImGui::BeginGroup( );
					{
						if ( ImGui::Tabs( "Settings" , ImVec2( windowWidth / 2 , tabHeight ) , m_nSelectedSubTab == SubTabs::SETTINGS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedSubTab = SubTabs::SETTINGS;
						if ( ImGui::Tabs( "Colors" , ImVec2( windowWidth / 2 , tabHeight ) , m_nSelectedSubTab == SubTabs::COLORS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedSubTab = SubTabs::COLORS;
					}
					ImGui::EndGroup( );
					break;
				case Tabs::DEV:
					ImGui::SetCursorPos( ImVec2( 5 , 55 ) );
					ImGui::BeginGroup( );
					{
						if ( ImGui::Tabs( "Settings" , ImVec2( windowWidth / 2 , tabHeight ) , m_nSelectedSubTab == SubTabs::SETTINGS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedSubTab = SubTabs::SETTINGS;
						if ( ImGui::Tabs( "Colors" , ImVec2( windowWidth / 2 , tabHeight ) , m_nSelectedSubTab == SubTabs::COLORS , m_cMenuBgFor , m_cMenuBgFor2 , 0 ) ) m_nSelectedSubTab = SubTabs::COLORS;
					}
					ImGui::EndGroup( );
					break;
				}

				ImGui::PopStyleColor( 3 );
				ImGui::PopStyleVar( ); // this pops the itemspacing push

				ImGui::PushStyleColor( ImGuiCol_ChildBg , m_cMenuBgFor.Value );

				switch ( m_nSelectedTab )
				{
				case Tabs::AIM:

					switch ( m_nSelectedSubTab )
					{

					case SubTabs::SETTINGS:
					{
						ImGui::SetCursorPos( ImVec2( 10 , 80 ) );
						ImGui::BeginChild( "Aim Settings" , ImVec2( windowWidth - 20 , 350 ) );
						{
							//render funcs
							ImGui::SetCursorPos( ImVec2( 10 , 10 ) );
							ImGui::BeginGroup( );
							{
							

								
							}
							ImGui::EndGroup( );
						}
						ImGui::EndChild( );
					} break;
					case SubTabs::COLORS:
					{
						ImGui::SetCursorPos( ImVec2( 10 , 80 ) );
						ImGui::BeginChild( "Aim Colors" , ImVec2( windowWidth - 20 , 350 ) );
						{
							
						}
						ImGui::EndChild( );
					} break;
					}

					break;
				case Tabs::VISUALS:

					switch ( m_nSelectedSubTab )
					{

					case SubTabs::SETTINGS:
					{
						ImGui::SetCursorPos( ImVec2( 10 , 80 ) );
						ImGui::BeginChild( "Visuals Settings" , ImVec2( windowWidth - 20 , 350 ) );
						{
							//render funcs
							ImGui::SetCursorPos( ImVec2( 10 , 10 ) );
							ImGui::BeginGroup( );
							{
								ImGui::Checkbox( "draw box" , &global::visuals::boxes, m_cMenuBg , m_cMenuAccent, m_cMenuOff );
								ImGui::Checkbox( "draw health bar" , &global::visuals::health_bar , m_cMenuBg , m_cMenuAccent , m_cMenuOff );
								ImGui::Checkbox( "draw chams" , &global::visuals::chams , m_cMenuBg , m_cMenuAccent , m_cMenuOff );
							}
							ImGui::EndGroup( );

						}
						ImGui::EndChild( );
					} break;
					case SubTabs::COLORS:
					{
						ImGui::SetCursorPos( ImVec2( 10 , 80 ) );
						ImGui::BeginChild( "Visuals Colors" , ImVec2( windowWidth - 20 , 350 ) );
						{
							//render funcs
							ImGui::SetCursorPos( ImVec2( 10 , 10 ) );
							ImGui::BeginGroup( );
							{

							}
							ImGui::EndGroup( );
						}
						ImGui::EndChild( );
					} break;
					}

					break;
				case Tabs::RADAR:

					switch ( m_nSelectedSubTab )
					{
					case SubTabs::SETTINGS:
					{

						ImGui::SetCursorPos( ImVec2( 10 , 80 ) );
						ImGui::BeginChild( "Radar Settings" , ImVec2( windowWidth - 20 , 350 ) );
						{
							//render funcs
							ImGui::SetCursorPos( ImVec2( 10 , 10 ) );
							ImGui::BeginGroup( );
							{
							
							}
							ImGui::EndGroup( );
						}
						ImGui::EndChild( );
					} break;
					case SubTabs::COLORS:
					{
						ImGui::SetCursorPos( ImVec2( 10 , 80 ) );
						ImGui::BeginChild( "Radar Colors" , ImVec2( windowWidth - 20 , 350 ) );
						{
							//render funcs
							ImGui::SetCursorPos( ImVec2( 10 , 10 ) );
							ImGui::BeginGroup( );
							{
							}
							ImGui::EndGroup( );
						}
						ImGui::EndChild( );
					} break;
					}

					break;
				case Tabs::MISC:

					switch ( m_nSelectedSubTab )
					{

					case SubTabs::SETTINGS:
					{

						ImGui::SetCursorPos( ImVec2( 10 , 80 ) );
						ImGui::BeginChild( "Misc Settings" , ImVec2( windowWidth - 20 , 350 ) );
						{
							//render funcs
							ImGui::SetCursorPos( ImVec2( 10 , 10 ) );
							ImGui::BeginGroup( );
							{
								ImGui::Checkbox( "movement speed" , &global::movement_speed , m_cMenuBg , m_cMenuAccent , m_cMenuOff );
								ImGui::SliderFloat( "speed force" , &global::speed_force , 1.0f, 1000.0f );


							}
							ImGui::EndGroup( );
						}
						ImGui::EndChild( );
					} break;
					case SubTabs::COLORS:
					{
						ImGui::SetCursorPos( ImVec2( 10 , 80 ) );
						ImGui::BeginChild( "Misc Colors" , ImVec2( windowWidth - 20 , 350 ) );
						{
							//render funcs
							ImGui::SetCursorPos( ImVec2( 10 , 10 ) );
							ImGui::BeginGroup( );
							{
							}
							ImGui::EndGroup( );
						}
						ImGui::EndChild( );

					} break;
					}

					break;
				case Tabs::DEV:
					switch ( m_nSelectedSubTab )
					{

					case SubTabs::SETTINGS:
					{

						ImGui::SetCursorPos( ImVec2( 10 , 80 ) );
						ImGui::BeginChild( "Dev Settings" , ImVec2( windowWidth - 20 , 350 ) );
						{
							//render funcs
							ImGui::SetCursorPos( ImVec2( 10 , 10 ) );
							ImGui::BeginGroup( );
							{
							}
							ImGui::EndGroup( );
						}
						ImGui::EndChild( );
					} break;
					case SubTabs::COLORS:
					{
						ImGui::SetCursorPos( ImVec2( 10 , 80 ) );
						ImGui::BeginChild( "Dev Colors" , ImVec2( windowWidth - 20 , 350 ) );
						{
							//render funcs
							ImGui::SetCursorPos( ImVec2( 10 , 10 ) );
							ImGui::BeginGroup( );
							{
							}
							ImGui::EndGroup( );
						}
						ImGui::EndChild( );
					} break;
					}

					break;
				}

				ImGui::PopStyleColor( );
			}
			ImGui::End( );
        }
		visuals::draw( );

        ImGui::Render( );

        p_context->OMSetRenderTargets( 1 , &main_render_target_view , NULL );
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

        return o_present( p_swap_chain , sync_interval , flags );
    }

    static HRESULT __stdcall hk_present_trampoline( IDXGISwapChain * p_swap_chain , UINT sync_interval , UINT flags )
    {
        return instance->hk_present( p_swap_chain , sync_interval , flags );
    }

    bool hook( )
    {
        bool init_hook = false;

        do
        {
            if ( kiero::init( kiero::RenderType::D3D11 ) == kiero::Status::Success )
            {
                kiero::bind( 8 , ( void ** ) &o_present , &c_menu::hk_present_trampoline );

                init_hook = true;
            }
        } while ( !init_hook );

        return true;
    }
};
inline auto g_menu = std::make_shared<c_menu>( );