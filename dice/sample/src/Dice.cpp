//-------------------------------------------------------------------------------------------------
// File : Dice.cpp
// Desc : Dice Simulator.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include "Dice.h"
#include "TgaUtil.h"
#include <asdxRenderState.h>
#include <asdxLogger.h>
#include <asdxMath.h>


namespace /* anonymous */ {

//--------------------------------------------------------------------------------------------------
// Constant Values
//--------------------------------------------------------------------------------------------------

// サイコロ位置算出用テーブル.
static const asdx::int2 g_PosTable[][Dice::NUM_DICE] = {
    // 真ん中.                     // 左下                       // 右上.                    // 右下                     // 左上.
    { asdx::int2( -23, -80 ),     asdx::int2( -23, -80 ),       asdx::int2( -23, -70 ),     asdx::int2( -23, -90 ),     asdx::int2( -23, -80 ) },
    { asdx::int2( -23, -60 ),     asdx::int2( -23, -80 ),       asdx::int2( -23, -60 ),     asdx::int2( -23, -80 ),     asdx::int2( -23, -80 ) },
    { asdx::int2( -23, -60 ),     asdx::int2( -23, -60 ),       asdx::int2( -23, -30 ),     asdx::int2( -23, -70 ),     asdx::int2( -23, -60 ) },
    { asdx::int2( -23, -40 ),     asdx::int2( -23, -40 ),       asdx::int2( -23, -30 ),     asdx::int2( -23, -60 ),     asdx::int2( -23, -30 ) },
    { asdx::int2( -23, -20 ),     asdx::int2( -23, -40 ),       asdx::int2( -23, -20 ),     asdx::int2( -23, -60 ),     asdx::int2( -23, -30 ) },
    { asdx::int2( -23, -20 ),     asdx::int2( -23, -20 ),       asdx::int2( -23, -10 ),     asdx::int2( -23, -20 ),     asdx::int2( -23, -20 ) },
    { asdx::int2( -23, 10 ),      asdx::int2( -23, -20 ),       asdx::int2( -23, 10 ),      asdx::int2( -23, 10 ),      asdx::int2( -23, 10 )  },
    { asdx::int2( -23, 20 ),      asdx::int2( -23, 10 ),        asdx::int2( -23, 30 ),      asdx::int2( -23, 20 ),      asdx::int2( -23, 30 )  },
    { asdx::int2( -23, 60 ),      asdx::int2( -23, 20 ),        asdx::int2( -23, 30 ),      asdx::int2( -23, 30 ),      asdx::int2( -23, 40 )  },
    { asdx::int2( -23, 60 ),      asdx::int2( -23, 20 ),        asdx::int2( -23, 60 ),      asdx::int2( -23, 30 ),      asdx::int2( -23, 50 )  },
    { asdx::int2( -23, 60 ),      asdx::int2( -23, 40 ),        asdx::int2( -23, -10 ),     asdx::int2( -23, 50 ),      asdx::int2( -23, 50 )  },
    { asdx::int2( -23, 10 ),      asdx::int2( -23, 40 ),        asdx::int2( -23, -50 ),     asdx::int2( -23, 60 ),      asdx::int2( -23, 60 )  },
    { asdx::int2( -24, -60 ),     asdx::int2( -24, 40 ),        asdx::int2( -24, -50 ),     asdx::int2( -24, 70 ),      asdx::int2( -24, -60 ) },
    { asdx::int2( -24, -60 ),     asdx::int2( -24, 40 ),        asdx::int2( -24, -30 ),     asdx::int2( -24, -60 ),     asdx::int2( -24, -60 ) },
    { asdx::int2( -24, -30 ),     asdx::int2( -24, -10 ),       asdx::int2( -24, -30 ),     asdx::int2( -24, -50 ),     asdx::int2( -24, -40 ) },
    { asdx::int2( -24, -30 ),     asdx::int2( -24, -60 ),       asdx::int2( -24, -20 ),     asdx::int2( -24, -40 ),     asdx::int2( -24, -30 ) },
    { asdx::int2( -24, -20 ),     asdx::int2( -24, -60 ),       asdx::int2( -24, -10 ),     asdx::int2( -24, -30 ),     asdx::int2( -24, -20 ) },
    { asdx::int2( -24, 10 ),      asdx::int2( -24, -40 ),       asdx::int2( -24, 10 ),      asdx::int2( -24, -20 ),     asdx::int2( -24, -10 ) },
    { asdx::int2( -24, 20 ),      asdx::int2( -24, -40 ),       asdx::int2( -24, 40 ),      asdx::int2( -24, -10 ),     asdx::int2( -24, 10 )  },
    { asdx::int2( -24, 20 ),      asdx::int2( -24, -20 ),       asdx::int2( -24, 40 ),      asdx::int2( -24, 10 ),      asdx::int2( -24, 20 )  },
    { asdx::int2( -24, 40 ),      asdx::int2( -24, -10 ),       asdx::int2( -24, 60 ),      asdx::int2( -24, 30 ),      asdx::int2( -24, 30 )  },
    { asdx::int2( -24, -10 ),     asdx::int2( -24, 20 ),        asdx::int2( -24, -40 ),     asdx::int2( -24, 40 ),      asdx::int2( -24, 40 )  },
    { asdx::int2( -24, -10 ),     asdx::int2( -24, 20 ),        asdx::int2( -24, -30 ),     asdx::int2( -24, 50 ),      asdx::int2( -24, -40 ) },
    { asdx::int2( -24, -10 ),     asdx::int2( -24, 20 ),        asdx::int2( -24, -20 ),     asdx::int2( -24, -10 ),     asdx::int2( -24, -20 ) },
    { asdx::int2( -24, -10 ),     asdx::int2( -24, 40 ),        asdx::int2( -24, 20 ),      asdx::int2( -24, -10 ),     asdx::int2( -24, -10 ) },
    { asdx::int2( -24, -10 ),     asdx::int2( -24, 50 ),        asdx::int2( -24, 10 ),      asdx::int2( -24, -10 ),     asdx::int2( -24, 10 )  },
    { asdx::int2( -24, -10 ),     asdx::int2( -24, -10 ),       asdx::int2( -24, -20 ),     asdx::int2( -24, -10 ),     asdx::int2( -24, 10 )  },
    { asdx::int2( -24, -10 ),     asdx::int2( -24, -10 ),       asdx::int2( -24, -20 ),     asdx::int2( -24, -10 ),     asdx::int2( -24, -10 ) },
};

// 描画順番.
s32 g_DrawTable[][Dice::NUM_DICE] = {
    { 0, -1, -1, -1, -1 },
    { 0,  1, -1, -1, -1 },
    { 2,  0,  1, -1, -1 },
    { 2,  0,  1,  3, -1 },
    { 4,  2,  0,  1,  3 }
};


} // namespace /* anonymous */


///////////////////////////////////////////////////////////////////////////////////////////////////
// Dice class
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//      コンストラクタです.
//-------------------------------------------------------------------------------------------------
Dice::Dice()
: m_DiceCount   ( 1 )
, m_PosIndex    ( 0 )
, m_State       ( State::Roulette )
, m_Width       ( 960 )
, m_Height      ( 540 )
, m_Size        ( 100 )
, m_Scale       ( 1.0f, 1.0f )
, m_Timer       ()
, m_Sprite      ()
{
    std::random_device rndDevice;
    m_Random = std::mt19937( rndDevice() );

    for( u32 i=0; i<NUM_DICE; ++i)
    {
        m_Param[i].m_TexIndex = ( i + 1 ) % NUM_ANIMATION_TEXTURE;
        m_Param[i].m_Roll = i;
    }
}

//-------------------------------------------------------------------------------------------------
//      デストラクタです.
//-------------------------------------------------------------------------------------------------
Dice::~Dice()
{
    Term();
}

//-------------------------------------------------------------------------------------------------
//      初期化処理です.
//-------------------------------------------------------------------------------------------------
bool Dice::Init( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, s32 width, s32 height )
{
    // スプライトの初期化.
    if ( !m_Sprite.Init( pDevice, f32( width ), f32( height ) ) )
    {
        ELOG( "Error : Sprite Initialize Failed." );
        return false;
    }

    // ルーレットテクスチャの読み込み.
    for( auto i=0; i<NUM_ANIMATION_TEXTURE; ++i )
    {
        char16 filename[256];
        swprintf_s( filename, L"res/dice_A/dice0%d.tga", i+1 );

        if ( !CreateTextureFromTga( pDevice, pDeviceContext, filename, m_Texture[0][i] ) )
        {
            ELOG( "Error : CreateTextureFromTga() Failed." );
            return false;
        }

        swprintf_s( filename, L"res/dice_B/dice0%d.tga", i+1 );
        if ( !CreateTextureFromTga( pDevice, pDeviceContext, filename, m_Texture[1][i] ) )
        {
            ELOG( "Error : CreateTextureFromTga() Failed." );
            return false;
        }
    }

    // 出目テクスチャの読み込み.
    for( auto i=0; i<NUM_PATTERN; ++i )
    {
        char16 filename[ 256 ];
        swprintf_s( filename, L"res/dice_result/dice_result%d.tga", i+1 );
        if ( !CreateTextureFromTga( pDevice, pDeviceContext, filename, m_Dice[i] ) )
        {
            ELOG( "Error : CreateTextureFromTga() Failed." );
            return false;
        }
    }

    // リサイズ処理.
    OnResize( width, height );

    // 念のために初期化.
    m_PosIndex  = 0;
    m_State     = State::Roulette;

    // タイマースタート.
    m_Timer.Start();

    // 正常終了.
    return true;
}

//-------------------------------------------------------------------------------------------------
//      終了処理です.
//-------------------------------------------------------------------------------------------------
void Dice::Term()
{
    // ルーレットテクスチャを解放.
    for( auto i=0; i<NUM_ANIMATION_TEXTURE; ++i )
    {
        for( auto j=0; j<NUM_ANIMATION_TYPES; ++j )
        { m_Texture[j][i].Release(); }
    }

    // 出目テクスチャを解放.
    for( auto i=0; i<NUM_PATTERN; ++i )
    { m_Dice[i].Release(); }

    // インデックスをリセット.
    m_PosIndex  = 0;
}

//-------------------------------------------------------------------------------------------------
//      ルーレット開始処理.
//-------------------------------------------------------------------------------------------------
void Dice::Start()
{
    if ( m_State == State::Fixed )
    {
        m_State = State::Roulette;
        m_Timer.Start();
        ResetPosition();

        for( u32 i=0; i<m_DiceCount; ++i)
        {
            switch( m_Param[i].m_Roll )
            {
                case 1:
                case 2:
                case 6:
                    { m_Param[i].m_Type = 0; }
                    break;

                case 3:
                case 4:
                case 5:
                    { m_Param[i].m_Type = 1; }
                    break;

                default:
                    { /* DO_NOTHING */ }
                    break;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
//      ルーレット停止処理.
//-------------------------------------------------------------------------------------------------
void Dice::Stop()
{
    if ( m_State == State::Roulette )
    { m_State = State::Transition; }
}

//-------------------------------------------------------------------------------------------------
//      サイコロの出目を決定する.
//-------------------------------------------------------------------------------------------------
u32 Dice::RollDice()
{
    u32 rnd = 0;
    while( true )
    {
        rnd = m_Random();

        // 6の倍数にするため4294967291以上は振り直し.
        if ( rnd < 4294967291 )
        { break; }
    }

    return (rnd % NUM_PATTERN) + 1;
}

//-------------------------------------------------------------------------------------------------
//      更新処理.
//-------------------------------------------------------------------------------------------------
void Dice::Update()
{
    // 現在の時刻を記録.
    m_Timer.End();

    // 30FPSとして更新処理を掛ける.
    f64 elapsedTime = m_Timer.GetElapsedTimeSec();
    if ( elapsedTime > 0.033 )
    {
        if ( m_State == State::Transition )
        {
            for( u32 i=0; i<m_DiceCount; ++i )
            {
                m_Param[ i ].m_PrevPos.x += static_cast<s32>( g_PosTable[ m_PosIndex ][ i ].x * m_Scale.x );
                m_Param[ i ].m_PrevPos.y += static_cast<s32>( g_PosTable[ m_PosIndex ][ i ].y * m_Scale.y );
            }

            m_PosIndex++;

            // 29パターン再生したら，出目確定状態に移行.
            if ( m_PosIndex >= 28 )
            {
                m_State = State::Fixed;
                m_PosIndex = 0;
            }
        }

        // ルーレット・アニメーション状態の場合はテクスチャインデックスを更新.
        if ( m_State <= State::Transition )
        {
            for( u32 i=0; i<m_DiceCount; ++i )
            { m_Param[ i ].m_TexIndex = ( m_Param[ i ].m_TexIndex + 1 ) % 8;  }
        }

        // タイマー開始時刻を再設定.
        m_Timer.Start();
    }

    // アニメーション状態.
    if ( m_State == State::Transition )
    {
        for( u32 i=0; i<m_DiceCount; ++i )
        {
            asdx::Vector2 prev( 
                f32( m_Param[ i ].m_PrevPos.x ),
                f32( m_Param[ i ].m_PrevPos.y ) );

            asdx::Vector2 next( 
                prev.x + g_PosTable[ m_PosIndex ][ i ].x * m_Scale.x,
                prev.y + g_PosTable[ m_PosIndex ][ i ].y * m_Scale.y );

            f32 amount = asdx::Saturate( f32( elapsedTime / 0.033 ) );

            // 線形補間でキーフレーム間を求める.
            asdx::Vector2 curr = asdx::Vector2::Lerp( prev, next, amount );

            m_Param[ i ].m_CurrPos.x = static_cast<s32>( curr.x );
            m_Param[ i ].m_CurrPos.y = static_cast<s32>( curr.y );
        }
    }

    // ルーレット状態の場合は毎回サイコロを振る.
    if ( m_State == State::Roulette )
    {
        for( u32 i=0; i<m_DiceCount; ++i )
        { m_Param[ i ].m_Roll = RollDice(); }
    }
}

//-------------------------------------------------------------------------------------------------
//      描画処理.
//-------------------------------------------------------------------------------------------------
void Dice::Draw( ID3D11DeviceContext* pDeviceContext )
{
    // 更新処理.
    Update();

    // レンダーステートを設定.
    f32 blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    pDeviceContext->OMSetBlendState( asdx::RenderState::GetInstance().GetBS( asdx::BlendType::AlphaBlend ), blendFactor, U32_MAX );
    pDeviceContext->OMSetDepthStencilState( asdx::RenderState::GetInstance().GetDSS( asdx::DepthType::None ), 0 );

    for( u32 i=0; i<m_DiceCount; ++i )
    {
        // サイコロの描画順番を取得.
        s32 idx = g_DrawTable[ m_DiceCount-1 ][ i ];

        // テクスチャ算出.
        auto pSRV = ( m_State <= State::Transition ) ?
            m_Texture[ m_Param[ idx ].m_Type ][ m_Param[ idx ].m_TexIndex ].GetSRV() : m_Dice[ m_Param[ idx ].m_Roll - 1 ].GetSRV();
        m_Sprite.SetTexture( pSRV, asdx::RenderState::GetInstance().GetSmp( asdx::SamplerType::LinearClamp ) );

        // 描画コマンド作成.
        m_Sprite.Begin( pDeviceContext, asdx::Sprite::SHADER_TYPE_TEXTURE2D );
        m_Sprite.Draw( m_Param[ idx ].m_CurrPos.x, m_Param[ idx ].m_CurrPos.y, m_Size, m_Size );
        m_Sprite.End( pDeviceContext );
    }
}

//-------------------------------------------------------------------------------------------------
//      リサイズ処理.
//-------------------------------------------------------------------------------------------------
void Dice::OnResize(s32 width, s32 height)
{
    m_Width  = width;
    m_Height = height;

    m_Scale.x = ( width  - m_Size * 3 ) / 660.0f;
    m_Scale.y = ( height - m_Size * 3 ) / 240.0f;
    ResetPosition();

    m_Sprite.SetScreenSize( f32( width ), f32( height ) );
}

//-------------------------------------------------------------------------------------------------
//      位置座標をリセット.
//-------------------------------------------------------------------------------------------------
void Dice::ResetPosition()
{
    m_Param[0].m_CurrPos.x = m_Width  - 2 * m_Size;
    m_Param[0].m_CurrPos.y = m_Height - 2 * m_Size;

    m_Param[1].m_CurrPos.x = m_Width  - 2 * m_Size - ( m_Size / 2 );
    m_Param[1].m_CurrPos.y = m_Height - 2 * m_Size + ( m_Size / 2 );

    m_Param[2].m_CurrPos.x = m_Width  - 2 * m_Size + ( m_Size / 2 );
    m_Param[2].m_CurrPos.y = m_Height - 2 * m_Size - ( m_Size / 2 );

    m_Param[3].m_CurrPos.x = m_Width  - m_Size - ( m_Size / 2 );
    m_Param[3].m_CurrPos.y = m_Height - m_Size - ( m_Size / 2 );

    m_Param[4].m_CurrPos.x = m_Width  - 2 * m_Size - ( m_Size / 2 );
    m_Param[4].m_CurrPos.y = m_Height - 2 * m_Size - ( m_Size / 2 );

    for( u32 i=0; i<NUM_DICE; ++i )
    {
        m_Param[i].m_PrevPos = m_Param[i].m_CurrPos;
    }
}

//-------------------------------------------------------------------------------------------------
//      サイコロの出目を取得します.
//-------------------------------------------------------------------------------------------------
u32 Dice::GetRoll( const u32 index ) const
{
    assert( index < NUM_DICE );
    return m_Param[index].m_Roll;
}

//-------------------------------------------------------------------------------------------------
//      サイコロの出目の合計を取得します.
//-------------------------------------------------------------------------------------------------
u32 Dice::GetRoll() const
{
    u32 result = 0;

    for( u32 i=0; i<m_DiceCount; ++i )
    { result += m_Param[i].m_Roll; }

    return result;
}

//-------------------------------------------------------------------------------------------------
//      現在の状態を取得します.
//-------------------------------------------------------------------------------------------------
Dice::State Dice::GetState() const
{ return m_State; }

//-------------------------------------------------------------------------------------------------
//      サイコロの数を設定します.
//-------------------------------------------------------------------------------------------------
void Dice::SetDiceCount( const u32 count )
{
    assert( 1 <= count && count <= NUM_DICE );
    m_DiceCount = count;
}

//-------------------------------------------------------------------------------------------------
//      サイコロの数を取得します.
//-------------------------------------------------------------------------------------------------
u32 Dice::GetDiceCount() const
{ return m_DiceCount; }

