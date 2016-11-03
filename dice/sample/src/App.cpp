//-------------------------------------------------------------------------------------------------
// File : App.cpp
// Desc : Sample Application.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <asdxMisc.h>
#include <asdxLogger.h>
#include <asdxResTGA.h>
#include <asdxRenderState.h>
#include <App.h>
#include <cassert>
#include "TgaUtil.h"
#include <random>


///////////////////////////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//      コンストラクタです.
//-------------------------------------------------------------------------------------------------
App::App()
: asdx::Application( L"Dice", 960, 540, nullptr, nullptr, nullptr )
, m_Dice    ()
, m_Count   ( 1 )
{
}

//-------------------------------------------------------------------------------------------------
//      デストラクタです.
//-------------------------------------------------------------------------------------------------
App::~App()
{
    OnTerm();
}

//-------------------------------------------------------------------------------------------------
//      初期化処理です.
//-------------------------------------------------------------------------------------------------
bool App::OnInit()
{
    // レンダーステートの初期化.
    if ( !asdx::RenderState::GetInstance().Init( m_pDevice.GetPtr() ) )
    {
        ELOG( "Error : RenderState Initailize Failed. ");
        return false;
    }

    // サイコロ初期化.
    if ( !m_Dice.Init( m_pDevice.GetPtr(), m_pDeviceContext.GetPtr(), m_Width, m_Height ))
    {
        ELOG( "Error : Dice Initialize Failed." );
        return false;
    }

    // 正常終了.
    return true;
}

//-------------------------------------------------------------------------------------------------
//      終了処理です.
//-------------------------------------------------------------------------------------------------
void App::OnTerm()
{
    m_Dice.Term();

    // レンダーステートの終了処理.
    asdx::RenderState::GetInstance().Term();
}

//-------------------------------------------------------------------------------------------------
//      フレーム遷移処理です.
//-------------------------------------------------------------------------------------------------
void App::OnFrameMove(asdx::FrameEventArgs& param)
{
    ASDX_UNUSED_VAR( param );
}

//-------------------------------------------------------------------------------------------------
//      フレーム描画処理です.
//-------------------------------------------------------------------------------------------------
void App::OnFrameRender(asdx::FrameEventArgs& param)
{
    ASDX_UNUSED_VAR( param );

    {
        ID3D11RenderTargetView* pRTV = nullptr;
        ID3D11DepthStencilView* pDSV = nullptr;
        pRTV = m_ColorTarget2D.GetTargetView();
        pDSV = m_DepthTarget2D.GetTargetView();

        if ( pRTV == nullptr || pDSV == nullptr )
        { return; }

        m_pDeviceContext->OMSetRenderTargets( 1, &pRTV, pDSV );
        m_pDeviceContext->RSSetViewports( 1, &m_Viewport );

        m_pDeviceContext->ClearRenderTargetView( pRTV, m_ClearColor );
        m_pDeviceContext->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

        m_Dice.Draw( m_pDeviceContext.GetPtr() );
    }

    Present( 0 );
}

//-------------------------------------------------------------------------------------------------
//      リサイズ処理です.
//-------------------------------------------------------------------------------------------------
void App::OnResize(const asdx::ResizeEventArgs& param)
{
    m_Dice.OnResize( param.Width, param.Height );
}

//-------------------------------------------------------------------------------------------------
//      キーイベントの処理です.
//-------------------------------------------------------------------------------------------------
void App::OnKey(const asdx::KeyEventArgs& param)
{
    if ( param.IsKeyDown )
    {
        switch( param.KeyCode )
        {
            // Enterキー.
            case 13:
                {
                    if ( m_Dice.GetState() == Dice::State::Roulette )
                    { m_Dice.Stop(); }
                    else if ( m_Dice.GetState() == Dice::State::Fixed )
                    {
                        m_Dice.SetDiceCount( m_Count );
                        m_Dice.Start();
                    }
                }
                break;

            case '1':
                { SetDiceCount( 1 ); }
                break;

            case '2':
                { SetDiceCount( 2 ); }
                break;

            case '3':
                { SetDiceCount( 3 ); }
                break;

            case '4':
                { SetDiceCount( 4 ); }
                break;

            case '5':
                { SetDiceCount( 5 ); }
                break;

            default:
                break;
        }
    }
}

//-------------------------------------------------------------------------------------------------
//      サイコロの数を設定します.
//-------------------------------------------------------------------------------------------------
void App::SetDiceCount( u32 count )
{
    m_Count = count;

    if ( m_Dice.GetState() == Dice::State::Roulette )
    { m_Dice.SetDiceCount( m_Count ); }
}

//-------------------------------------------------------------------------------------------------
//      マウスイベントの処理です.
//-------------------------------------------------------------------------------------------------
void App::OnMouse(const asdx::MouseEventArgs& param)
{
}