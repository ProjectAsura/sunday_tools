//-------------------------------------------------------------------------------------------------
// File : main.cpp
// Desc : Application Main Entry Point.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include "Converter.h"
#include <Windows.h>


//-------------------------------------------------------------------------------------------------
//      �A�v���P�[�V�������C���G���g���[�|�C���g.
//-------------------------------------------------------------------------------------------------
int main( int argc, char** argv )
{
    if ( argc <= 1 )
    { return 0; }

    Converter::Config config;

    char outputFile[256];

    auto successCount = 0;
    auto failCount = 0;

    for( auto i=1; i<argc; ++i )
    {
        strcpy( outputFile, argv[i] );

        auto p = strrchr( outputFile, '.' );
        if ( p == nullptr )
        { continue; }

        if ( strcmp( p, ".c") == 0 )
        {
            config.Type = LANGUAGE_TYPE_CPP;
            strcpy( p, "_c.html" );
        }
        else if ( strcmp( p, ".h") == 0 )
        {
            config.Type = LANGUAGE_TYPE_CPP;
            strcpy( p, "_h.html" );
        }
        else if ( strcmp( p, ".cpp") == 0 )
        {
            config.Type = LANGUAGE_TYPE_CPP;
            strcpy( p, "_cpp.html" );
        }
        else if ( strcmp( p, ".hpp") == 0 )
        {
            config.Type = LANGUAGE_TYPE_CPP;
            strcpy( p, "_hpp.html" );
        }
        else if ( strcmp( p, ".inl" ) == 0 )
        {
            config.Type = LANGUAGE_TYPE_CPP;
            strcpy( p, "_inl.html" );
        }
        else if ( strcmp( p, ".glsl" ) == 0 )
        {
            config.Type = LANGUAGE_TYPE_GLSL;
            strcpy( p, "_glsl.html" );
        }
        else if ( strcmp( p, ".vert" ) == 0 )
        {
            config.Type = LANGUAGE_TYPE_GLSL;
            strcpy( p, "_vert.html" );
        }
        else if ( strcmp( p, ".frag") == 0 )
        {
            config.Type = LANGUAGE_TYPE_GLSL;
            strcpy( p, "_frag.html" );
        }
        else if ( strcmp( p, ".geom" )  == 0 )
        {
            config.Type = LANGUAGE_TYPE_GLSL;
            strcpy( p, "_geom.html" );
        }
        else if ( strcmp( p, ".hlsl" ) == 0 )
        {
            config.Type = LANGUAGE_TYPE_HLSL;
            strcpy( p, "_hlsl.html" );
        }
        else if ( strcmp( p, ".hlsli" ) == 0 )
        {
            config.Type = LANGUAGE_TYPE_HLSL;
            strcpy( p, "_hlsli.html" );
        }
        else if ( strcmp( p, ".fx" ) == 0 )
        {
            config.Type = LANGUAGE_TYPE_HLSL;
            strcpy( p, "_fx.html" );
        }
        else
        {
            config.Type = LANGUAGE_TYPE_CPP;
            strcpy( p, ".html" );
        }

        config.InputFile  = argv[i];
        config.OutputFile = outputFile;

        if ( Converter::Process( config ) )
        {
            printf_s( "�t�@�C���ϊ��ɐ������܂���\n" );
            printf_s( "�o�̓t�@�C���� : %s\n", config.OutputFile.c_str());
            successCount++;
        }
        else
        {
            printf_s( "�t�@�C���ϊ��Ɏ��s���܂���\n");
            printf_s( "���̓t�@�C���� : %s\n", config.InputFile.c_str());
            failCount++;
        }
    }

    if ( failCount == 0 )
    {
        MessageBoxA( nullptr, "�t�@�C���ϊ��ɐ������܂���.", "�t�@�C���ϊ�����", MB_OK | MB_ICONINFORMATION );
    }
    else if ( successCount == 0 )
    {
        MessageBoxA( nullptr, "�t�@�C���ϊ��Ɏ��s���܂���.", "�t�@�C���ϊ����s", MB_OK | MB_ICONERROR );
    }
    else
    {
        char message[256];
        sprintf_s( message, "�t�@�C���ϊ����܂���.\n ����I���F%d\n, �G���[:%d\n", successCount, failCount);
        MessageBoxA( nullptr, message, "�t�@�C���ϊ�", MB_OK | MB_ICONWARNING );
    }

    return 0;
}
