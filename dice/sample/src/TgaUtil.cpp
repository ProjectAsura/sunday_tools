//-------------------------------------------------------------------------------------------------
// File : TgaUtil.cpp
// Desc : Tga Utility
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include "TgaUtil.h"
#include <asdxResTexture.h>
#include <asdxResTGA.h>
#include <asdxMisc.h>
#include <asdxLogger.h>
#include <cassert>


//-------------------------------------------------------------------------------------------------
//      TGAからテクスチャを生成します.
//-------------------------------------------------------------------------------------------------
bool CreateTextureFromTga
(
    ID3D11Device*           pDevice,
    ID3D11DeviceContext*    pDeviceContext,
    const char16*           filename,
    asdx::Texture2D&        result
)
{
    std::wstring path;
    if ( !asdx::SearchFilePath( filename, path ) )
    {
        ELOG( "Error : File Not Found." );
        return false;
    }

    asdx::ResTGA value;
    if ( !value.Load( path.c_str() ) )
    {
        ELOG( "Error : File Load Failed. path = %s", path.c_str() );
        return false;
    }

    asdx::ResTexture res;
    res.Width        = value.GetWidth();
    res.Height       = value.GetHeight();
    res.Depth        = 0;
    res.MipMapCount  = 1;
    res.SurfaceCount = 1;

    auto bytePerPixel = value.GetBitPerPixel() / 8;
    auto isRGB = ( bytePerPixel == 3 );

    auto pSubResource = new asdx::SubResource();
    pSubResource->Width      = value.GetWidth();
    pSubResource->Height     = value.GetHeight();
    pSubResource->Pitch      = value.GetWidth() * ( (isRGB) ? 4 : bytePerPixel );
    pSubResource->SlicePitch = pSubResource->Pitch * value.GetHeight();
    pSubResource->pPixels    = new u8 [ pSubResource->SlicePitch ];
    assert( pSubResource->pPixels != nullptr );
    res.pResources = pSubResource;

    switch( value.GetFormat() )
    {
        case asdx::TGA_FORMAT_NONE:           { res.Format = DXGI_FORMAT_UNKNOWN; }          break;
        case asdx::TGA_FORMAT_FULLCOLOR:      { res.Format = DXGI_FORMAT_R8G8B8A8_UNORM; }   break;
        case asdx::TGA_FORMAT_GRAYSCALE:      { res.Format = DXGI_FORMAT_R8_UNORM; }         break;
        case asdx::TGA_FORMAT_RLE_FULLCOLOR:  { res.Format = DXGI_FORMAT_R8G8B8A8_UNORM; }   break;
        case asdx::TGA_FORMAT_RLE_GRAYSCALE:  { res.Format = DXGI_FORMAT_R8_UNORM; }         break;

        case asdx::TGA_FORMAT_INDEXCOLOR:
        case asdx::TGA_FORMAT_RLE_INDEXCOLOR:
            {
                switch( bytePerPixel )
                {
                case 1:
                    { res.Format = DXGI_FORMAT_R8_UNORM; }
                    break;

                case 2:
                    { res.Format = DXGI_FORMAT_B5G5R5A1_UNORM; }
                    break;

                default:
                    { ELOG( "Error : Unknown Format. bytePerPixel = %d", bytePerPixel ); }
                    break;
                }
            }
            break;
    }

    if ( isRGB )
    {
        auto pixel = value.GetPixels();

        for( u32 i=0; i<value.GetWidth() * value.GetHeight(); ++i )
        {
            pSubResource->pPixels[ i * 4 + 0 ] = pixel[ i * 3 + 0 ];
            pSubResource->pPixels[ i * 4 + 1 ] = pixel[ i * 3 + 1 ];
            pSubResource->pPixels[ i * 4 + 2 ] = pixel[ i * 3 + 2 ];
            pSubResource->pPixels[ i * 4 + 3 ] = 255;
        }
    }
    else
    {
        memcpy( pSubResource->pPixels, value.GetPixels(), pSubResource->SlicePitch );
    }

    value.Release();

    if ( !result.Create( pDevice, pDeviceContext, res ) )
    {
        ELOG( "Error : Create Texture Failed." );
        res.Release();
        return false;
    }

    res.Release();

    return true;
}
